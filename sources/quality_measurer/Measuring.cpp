//
//  Measuring.cpp
//  testing system
//
//  Created by Miloš Šimek on 30.04.15.
//  Copyright (c) 2015 Miloš Šimek. All rights reserved.
//

#include "Measuring.hpp"

MeasuredData measure(string& genome,
             ifstream& corruptedSeqFile, ifstream& correctedSeqFile,
             ifstream& mapFile) {
    
    MeasuredData measured;
    tuple<string, string, string> sequences;
    
    sequences = loadSequences(corruptedSeqFile, correctedSeqFile, mapFile, genome);
    
    while (get<0>(sequences).size()) {
        string& corruptedSeq = get<0>(sequences);
        string& correctedSeq = get<1>(sequences);
        string& genomeSeq = get<2>(sequences);
        
        //4 parts:
        //count original errors
        measured.originalErrors += countDifferences(genomeSeq, corruptedSeq);
        
        //count unaltered sequences
        bool isUnalt = isUnaltered(genomeSeq, corruptedSeq, correctedSeq);
        if (isUnalt) measured.unalteredSeqCount++;
        
        //count states
        countStates(genomeSeq, corruptedSeq, correctedSeq, measured);
        
        //cout sequences
        measured.seqCount++;
        
        sequences = loadSequences(corruptedSeqFile, correctedSeqFile, mapFile, genome);
    }
    
    return measured;
}

unsigned countDifferences(string& first, string& second) {
    unsigned count = 0;
    
    for (unsigned pos=0; pos < first.size(); pos++) {
        if (first[pos] != second[pos]) count++;
    }
    
    return count;
}

void countStates(string& genomeSeq, string& corruptedSeq, string& correctedSeq,
                 MeasuredData& measured) {
    
    for (unsigned pos=0; pos < corruptedSeq.size(); pos++) {
        BaseState state = getState(genomeSeq[pos], corruptedSeq[pos], correctedSeq[pos]);
        
	measured.counter[state]++;
    }
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

BaseState getState(char genBase, char corruptedBase, char correctedBase) {
    if (genBase == corruptedBase) {
        if (corruptedBase == correctedBase) return BS_TN;
        else return BS_FP;
    } else {
        if (genBase == correctedBase) return BS_TP;
	else return BS_FN;
    }
}

tuple<string, string, string> loadSequences(ifstream& corruptedSeqFile,
                                           ifstream& correctedSeqFile,
                                           ifstream& mapFile,
                                           string& genome) {
    string corruptedSeq, correctedSeq, genomeSeq;
    
    try {
        corruptedSeq = loadNextSeq(corruptedSeqFile);
        correctedSeq = loadNextSeq(correctedSeqFile);
        
        //check files
        bool corruptedEmpty = correctedSeq.empty();
        bool correctedEmpty = correctedSeq.empty();
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
        ULL genomePos = loadNextSeqPos(mapFile);
        ULL maxGenomePos = genome.size() - corruptedSeq.size();
        if (genomePos > maxGenomePos) {
            cerr << "Corrupted map file" << endl;
            exit(1);
        }
        genomeSeq = genome.substr(genomePos,corruptedSeq.size());
        assert(genomeSeq.length() == corruptedSeq.length());
        
    } catch (exception &e) {
        cerr << "Problem while loading files" << endl << endl;
        exit(1);
    }
    
    return make_tuple(corruptedSeq, correctedSeq, genomeSeq);
}
