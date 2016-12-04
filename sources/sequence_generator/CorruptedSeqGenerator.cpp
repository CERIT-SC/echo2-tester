//
//  CorruptedSeqGenerator.cpp
//  testing system
//
//  Created by Miloš Šimek on 21.04.15.
//  Copyright (c) 2015 Miloš Šimek. All rights reserved.
//

#include "CorruptedSeqGenerator.h"


void generateCorrSequences(const string & genome, ULL seqCount,
                           unsigned seqLen, const N44Matrix& probMatrix, unsigned randGenSeed,
                           ofstream& fastqFile, ofstream& mapFile) {
    
    mt19937_64 randNumGenerator(randGenSeed);
    
    ULL usableLength = genome.length() - seqLen + 1;
    string sequence(seqLen, ' '), quality(seqLen, '!');
    
    for (ULL seqIndex = 0; seqIndex < seqCount; seqIndex++) {
        ULL genomePos = randNumGenerator() % usableLength;
        
        sequence.clear();
        sequence = genome.substr(genomePos, seqLen);
        
        generateErrors(sequence, probMatrix, randNumGenerator);
        
        fastqFile << createFastqEntry(seqIndex, sequence, quality) << "\n";
        mapFile << genomePos << " ";
    }
}

#define END_A probA
#define END_C probA + probC
#define END_G probA + probC + probG

void generateErrors(string & sequence, const N44Matrix& probMatrix, mt19937_64& randNumGenerator){
    for (unsigned seqPos = 0; seqPos < sequence.length(); seqPos++) {
        
        //get row of probability matrix
        char row = 0;
        switch (sequence[seqPos]) {
            case 'A': row = 0; break;
            case 'C': row = 1; break;
            case 'G': row = 2; break;
            case 'T': row = 3; break;
                
            default: throw logic_error("bad_base"); break;
        }
        
        //get probabilities as whole numbers
        unsigned probA = probMatrix(seqPos, row, 0) * 10000;
        unsigned probC = probMatrix(seqPos, row, 1) * 10000;
        unsigned probG = probMatrix(seqPos, row, 2) * 10000;
        unsigned probT = probMatrix(seqPos, row, 3) * 10000;
        
        unsigned fulProbabilityNum = probA + probC + probG + probT;
        unsigned randNumber = randNumGenerator() % fulProbabilityNum;
        
        //put errors in sequence
        if (randNumber <= END_A) sequence[seqPos] = 'A';
        if (randNumber > END_A && randNumber <= END_C) sequence[seqPos] = 'C';
        if (randNumber > END_C && randNumber <= END_G) sequence[seqPos] = 'G';
        if (randNumber > END_G)  sequence[seqPos] = 'T';
    }
}

string createFastqEntry(ULL seqId, string sequence, string quality) {
    assert(sequence.length() == quality.length());
    string fastqEntry;
    
    fastqEntry += string("@") + to_string(seqId) + "\n";
    fastqEntry += sequence + "\n";
    fastqEntry += "+\n";
    fastqEntry += quality;
    
    return fastqEntry;
}