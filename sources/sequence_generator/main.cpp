//
//  main.cpp
//  sequence_generator
//
//  Created by Miloš Šimek on 10.03.15.
//  Copyright (c) 2015 Miloš Šimek. All rights reserved.
//

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include "SeqGenOptions.h"
#include "ProbMatrixGetter.h"
#include "CorruptedSeqGenerator.h"
#include "../Global/N44Matrix.h"
using namespace std;

N44Matrix getMatrix(SeqGenOptions& options);
string loadGenome(string genomeFileName);

int main(int argc, const char * argv[]) {
    
    //load options
    SeqGenOptions options(argc, argv);
    
    if(options.optionsState() == OPS_HELP) return 0;
    if(options.optionsState() == OPS_ERR)  return 1;
    
    //load input data
    N44Matrix probMatrix = getMatrix(options);
    string genome = loadGenome(options.getGenomeFilePath());
    
    if (genome.size() < options.getSeqLength()) {
        cerr << "Sequences cannot be longer than genome" << endl << endl;
        exit(1);
    }
    
    cout << "Genome length: " << genome.size() << endl;
    
    //create output files
    ofstream outputSeqFile(options.getOutputSequenceFileName());
    ofstream outputMapFile(options.getOutputMapFileName());
    
    if (!outputSeqFile.is_open()) {
        cerr << "Cannot open output sequence file" << endl << endl;
        exit(1);
    }
  
    if (!outputMapFile.is_open()) {
        cerr << "Cannot open output map file" << endl << endl;
        exit(1);
    }
    
    cout << probMatrix << endl;
    
    //setup random generator seed
    unsigned randGenSeed;
    if(options.randGenSeedIsSet()) randGenSeed = options.randGenSeed();
    else randGenSeed = static_cast<unsigned>(time(nullptr));
    
    //generate corrupted sequences and save results to files
    generateCorrSequences(genome, options.getSeqNum(), options.getSeqLength(),
                          probMatrix, randGenSeed, outputSeqFile, outputMapFile);
    
    endl(cout);
    
    return 0;
}

//generates/loads probability matrix
N44Matrix getMatrix(SeqGenOptions& options) {
    N44Matrix probMatrix;
    
    if (options.isProbabilityUniform()) {
        probMatrix = generateProbMatrix(options.getUniformProbability(), options.getSeqLength());
        
    } else {
        ProbMatrixLoader loader(options.getProbabilityFileName(), options.getSeqLength());
        if (!loader.loadedCorrectly()) {
            cerr << "Probability matrix has not been loaded correctly." << endl << endl;
            exit(1);
        }
        probMatrix = loader.getMatrix();
    }
    
    return probMatrix;
}

string loadGenome(string genomeFileName) {
    ifstream genomeFile(genomeFileName);
    string genome;
    
    if (!genomeFile.is_open()) {
        cerr << "Cannot open genome file" << endl << endl;
        exit(1);
    }
    
    genomeFile >> genome;
    
    if (genomeFile.fail()) {
        cerr << "Cannot load genome from file" << endl << endl;
        exit(1);
    }
    
    transform(genome.begin(), genome.end(), genome.begin(), ::toupper);
    
    //check if genome has only A C G T characters
    for_each(genome.begin(), genome.end(), [](char c){
        if (c != 'A' && c != 'C' && c != 'G' && c != 'T') {
            cerr << "Bad data in genome file" << endl << endl;
            exit(1);
        }
    });
    
    return genome;
}


