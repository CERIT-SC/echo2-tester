//
//  main.cpp
//  Corrector Tester
//  sequence_generator
//
//  Created by Miloš Šimek on 15/12/2016.
//
//

#include <iostream>
#include <random>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cctype>

#include "SeqGenOptions.hpp"
#include "ProbMatrixLoader.hpp"
#include "../global/N44Matrix.hpp"
#include "../global/Fasta.hpp"
using namespace std;
typedef unsigned long long ULL;


struct Sequence {
    string sequence;
    unsigned fragmentIndex;
    ULL position;
};


//function declarations
N44Matrix getMatrix(SeqGenOptions& options);
N44Matrix generateUniformProb(float uniformProbability, unsigned matrixLength);
void loadGenome(Fasta& genome, SeqGenOptions& options);
void testOutputFiles(ofstream& seqFile, ofstream& mapFile);

vector<Sequence> generateSequences(Fasta& genome,
                                   float coverage,
                                   unsigned seqLength,
                                   mt19937_64& randGen);
void generateErrors(vector<Sequence>& sequences,
                    N44Matrix probMatrix,
                    mt19937_64& randGen);
void saveSequences(vector<Sequence>& sequences, ofstream& seqFile);
void saveMapInfo(vector<Sequence>& sequences, ofstream& mapFile);



int main(int argc, const char * argv[]) {
    
    //load options
    SeqGenOptions options(argc, argv);
    if (options.optionsState() == OPS_HELP) return 0;
    if (options.optionsState() == OPS_ERR)  return 1;
    
    //load input data
    cout << "Loading imput files" << endl;
    N44Matrix probMatrix = getMatrix(options);
    Fasta genome;
    loadGenome(genome, options);
    
    //create output files
    ofstream outputSeqFile(options.getOutputSequenceFileName());
    ofstream outputMapFile(options.getOutputMapFileName());
    testOutputFiles(outputSeqFile, outputMapFile);
    
    //setup random generator
    mt19937_64 randGenerator;
    if (options.randGenSeed()) randGenerator.seed(*options.randGenSeed());
    else randGenerator.seed(static_cast<unsigned>(time(nullptr)));
    
    //generate sequences
    cout << "Generating sequences" << endl;
    vector<Sequence> sequences = generateSequences(genome,
                                                   options.getCoverage(),
                                                   options.getSeqLength(),
                                                   randGenerator);
    cout << "    " << sequences.size() << " sequences generated" << endl;
 
    //introduce errors
    cout << "Introducing errors" << endl;
    generateErrors(sequences, probMatrix, randGenerator);
   
    
    //shuffle sequences
    cout << "Shuffling" << endl;
    shuffle(sequences.begin(), sequences.end(), randGenerator);
    
    //safe to files
    cout << "Saving" << endl;
    saveSequences(sequences, outputSeqFile);
    saveMapInfo(sequences, outputMapFile);
    
    cout << "Done" << endl;
    cout << endl;
    
}


//generates/loads probability matrix
N44Matrix getMatrix(SeqGenOptions& options) {
    N44Matrix probMatrix;
    
    if (options.uniformProbability()) {     //returned as Optional
        probMatrix = generateUniformProb(*options.uniformProbability(), options.getSeqLength());
        
    } else {
        ProbMatrixLoader loader(*options.probabilityFileName(), options.getSeqLength());
        if (!loader.loadedCorrectly()) {
            cerr << "Probability matrix has not been loaded correctly." << endl << endl;
            exit(1);
        }
        probMatrix = loader.getMatrix();
    }
    
    return probMatrix;
}

N44Matrix generateUniformProb(float uniformProbability, unsigned matrixLength) {
    N44Matrix probMatrix(matrixLength);
    
    double primaryProb = static_cast<double>(100-uniformProbability) / 100;
    double secondProb = static_cast<double>(uniformProbability) / (3*100);
    
    for (unsigned index = 0; index < matrixLength; index++) {
        for (unsigned row = 0; row < 4; row++) {
            for (unsigned column = 0; column < 4; column++) {
                
                if(row == column) probMatrix(index,row,column) = primaryProb;
                else probMatrix(index,row,column) = secondProb;
            }
        }
    }
    
    return probMatrix;
}

void loadGenome(Fasta& genome, SeqGenOptions& options) {
    try {
        genome.loadFromFile(options.getGenomeFileName());
    } catch (...) {
        cerr << "Could not load genome file" << endl;
        cerr << endl;
        exit(1);
    }
    
    if (genome.getFragmentCount() == 0) {
        cerr << "No genome data loaded" << endl;
        cerr << endl;
        exit(1);
    }
}

void testOutputFiles(ofstream& seqFile, ofstream& mapFile) {
    if (!seqFile.is_open()) {
        cerr << "Cannot open output sequence file" << endl << endl;
        exit(1);
    }
    
    if (!mapFile.is_open()) {
        cerr << "Cannot open output map file" << endl << endl;
        exit(1);
    }
}

vector<Sequence> generateSequences(Fasta& genome,
                                   float coverage,
                                   unsigned seqLength,
                                   mt19937_64& randGen) {
    vector<Sequence> sequences;
    
    for (unsigned fragIndex = 0; fragIndex < genome.getFragmentCount(); fragIndex++) {
        string& fragment = genome.getData(fragIndex);
        
        //ignoring fragments shorter than sequence
        if (fragment.length() < seqLength) continue;
        
        //compute number of sequences from coverage
        ULL seqCount = round((fragment.length() / seqLength) * coverage);
        
        size_t maxPosition = fragment.length() - seqLength;
        
        //generate sequences
        for (ULL seqNum = 0; seqNum < seqCount; seqNum++) {
            size_t position = randGen() % (maxPosition + 1);
            
            Sequence seq;
            seq.sequence = fragment.substr(position, seqLength);
            seq.fragmentIndex = fragIndex;
            seq.position = position;
            
            sequences.push_back(seq);
        }
    }
    
    return sequences;
}


#define END_A probA
#define END_C probA + probC
#define END_G probA + probC + probG

void generateErrors(vector<Sequence>& sequences,
                    N44Matrix probMatrix,
                    mt19937_64& randGen) {
    
    for (Sequence& seq: sequences) {
        string& sequence = seq.sequence;
        
        for (unsigned seqPos = 0; seqPos < sequence.length(); seqPos++) {
            char& base = sequence[seqPos];
            if (base == 'N') continue;
            
            //get row of probability matrix
            char row = 0;
            switch (base) {
                case 'A': case 'a': row = 0; break;
                case 'C': case 'c': row = 1; break;
                case 'G': case 'g': row = 2; break;
                case 'T': case 't': row = 3; break;
                default: {
                    cerr << "Corrupted input data" << endl;
                    cerr << "Aborting generation" << endl;
                    cerr << endl;
                    exit(1);
                }
            }
            
            //get probabilities as whole numbers
            unsigned probA = probMatrix(seqPos, row, 0) * 10000;
            unsigned probC = probMatrix(seqPos, row, 1) * 10000;
            unsigned probG = probMatrix(seqPos, row, 2) * 10000;
            unsigned probT = probMatrix(seqPos, row, 3) * 10000;
            
            unsigned fullProbabilityNum = probA + probC + probG + probT;
            unsigned randNumber = randGen() % fullProbabilityNum;
            bool upper = isupper(base);
            
            //put errors in sequence
            if (randNumber <= END_A) base = upper ? 'A': 'a';
            if (randNumber > END_A && randNumber <= END_C) base = upper ? 'C': 'c';
            if (randNumber > END_C && randNumber <= END_G) base = upper ? 'G': 'g';
            if (randNumber > END_G) base = upper ? 'T': 't';
        }
    }
}

void saveSequences(vector<Sequence>& sequences, ofstream& seqFile) {
    for (ULL index = 0; index < sequences.size(); index++) {
        string fastqEntry;
        
        fastqEntry += string("@") + to_string(index) + "\n";
        fastqEntry += sequences[index].sequence + "\n";
        fastqEntry += "+\n";
        fastqEntry += string(sequences[index].sequence.length(), '!') + "\n";
        
        seqFile << fastqEntry;
    }
}

void saveMapInfo(vector<Sequence>& sequences, ofstream& mapFile) {
    for (ULL index = 0; index < sequences.size(); index++) {
        Sequence& seq = sequences[index];
        
        mapFile << seq.fragmentIndex << " " << seq.position << "\n";
    }
}

