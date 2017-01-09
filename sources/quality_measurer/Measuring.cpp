//
//  Measuring.cpp
//  Corrector Tester
//
//  Created by Miloš Šimek on 22/12/2016.
//
//

#include "Measuring.hpp"

//function declarations
//tuple: corrupted sequence, corrected sequence, genome sequence
tuple<string, string, string> loadSequences(ifstream& corruptedSeqFile,
                                            ifstream& correctedSeqFile,
                                            ifstream& mapFile,
                                            Fasta& genome);
unsigned countDifferences(string& first, string& second);
bool isUnaltered(string& genomeSeq, string& corruptedSeq, string& correctedSeq);
void countStates(string& genomeSeq, string& corruptedSeq, string& correctedSeq,
                 MeasuredData& measured);
BaseState getState(char genBase, char corruptedBase, char correctedBase);




//function definitions
MeasuredData measure(ifstream& corruptedSeqFile,
                     ifstream& correctedSeqFile,
                     ifstream& mapFile,
                     Fasta& genome) {
    
    MeasuredData measured;
    tuple<string, string, string> sequences;
    sequences = loadSequences(corruptedSeqFile, correctedSeqFile, mapFile, genome);
    
    while (get<0>(sequences).size()) {
        string& corruptedSeq = get<0>(sequences);
        string& correctedSeq = get<1>(sequences);
        string& genomeSeq = get<2>(sequences);
        
        //to upper all bases to treat i.e. C and c as same base
        boost::to_upper(corruptedSeq);
        boost::to_upper(correctedSeq);
        boost::to_upper(genomeSeq);
        
        //4 parts:
        //count original errors
        measured.originalErrors += countDifferences(genomeSeq, corruptedSeq);
        
        //count unaltered sequences
        if (isUnaltered(genomeSeq, corruptedSeq, correctedSeq)) {
            measured.unalteredSeqCount++;
        }
        
        //count states
        countStates(genomeSeq, corruptedSeq, correctedSeq, measured);
        
        //count sequences
        measured.seqCount++;
        
        sequences = loadSequences(corruptedSeqFile, correctedSeqFile, mapFile, genome);
    }
    
    return measured;
}

tuple<string, string, string> loadSequences(ifstream& corruptedSeqFile,
                                            ifstream& correctedSeqFile,
                                            ifstream& mapFile,
                                            Fasta& genome) {
    string corruptedSeq, correctedSeq, genomeSeq;
    
    try {
        corruptedSeq = loadNextSeq(corruptedSeqFile);
        correctedSeq = loadNextSeq(correctedSeqFile);
        
        //check files consistency
        bool corruptedEmpty = correctedSeq.empty();
        bool correctedEmpty = correctedSeq.empty();
        
            //if both empty, treat like end of file
        if (corruptedEmpty && correctedEmpty) return make_tuple(string(), string(), string());
        
        if ((!corruptedEmpty && correctedEmpty) || (corruptedEmpty && !correctedEmpty)) {
            cerr << "Inconsistency in input files" << endl << endl;
            exit(1);
        }
        
        if (corruptedSeq.size() != correctedSeq.size()) {
            cerr << "Inconsistency in input files" << endl << endl;
            exit(1);
        }
        
        //get matching genome part
        //pair: fragment, position
        pair<unsigned, ULL> seqMapEntry = loadNextMapEntry(mapFile);
        
            //check consistency
        if (seqMapEntry.first >= genome.getFragmentCount() ||
            seqMapEntry.second > (genome.getData(seqMapEntry.first).size() -
            corruptedSeq.size())) {
            
            cerr << "Mismatch between map file and genome file" << endl;
            cerr << endl;
            exit(1);
        }
        
        genomeSeq = genome.getData(seqMapEntry.first).
            substr(seqMapEntry.second, corruptedSeq.size());
        
        assert(corruptedSeq.length() == correctedSeq.length());
        assert(genomeSeq.length() == corruptedSeq.length());
        
    } catch (exception &e) {
        cerr << "Problem while loading files" << endl << endl;
        exit(1);
    }
    
    return make_tuple(corruptedSeq, correctedSeq, genomeSeq);
}

unsigned countDifferences(string& first, string& second) {
    unsigned count = 0;
    
    for (unsigned pos=0; pos < first.size(); pos++) {
        if (first[pos] != second[pos]) count++;
    }
    
    return count;
}

bool isUnaltered(string& genomeSeq, string& corruptedSeq, string& correctedSeq) {
    //to concider as unaltered:
    //must have errors
    bool haveErrors = countDifferences(genomeSeq, corruptedSeq);
    if (!haveErrors) return false;
    
    //must be unchanged
    bool isUnchanged = !countDifferences(corruptedSeq, correctedSeq);
    if (isUnchanged) return true;
    return false;
}

void countStates(string& genomeSeq, string& corruptedSeq, string& correctedSeq,
                 MeasuredData& measured) {
    
    for (unsigned pos=0; pos < corruptedSeq.size(); pos++) {
        BaseState state = getState(genomeSeq[pos], corruptedSeq[pos], correctedSeq[pos]);
        
        measured.counter[state]++;
    }
}

BaseState getState(char genBase, char corruptedBase, char correctedBase) {
    if (genBase == corruptedBase) {
        if (corruptedBase == correctedBase) return BS_TrueNegative;
        else return BS_FalsePositive;
    } else {
        if (genBase == correctedBase) return BS_TruePositive;
        else return BS_FalseNegative;
    }
}
