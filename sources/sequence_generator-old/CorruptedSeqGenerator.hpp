//
//  CorruptedSeqGenerator.h
//  testing system
//
//  Created by Miloš Šimek on 21.04.15.
//  Copyright (c) 2015 Miloš Šimek. All rights reserved.
//

#ifndef __testing_system__CorruptedSeqGenerator__
#define __testing_system__CorruptedSeqGenerator__

#include <string>
#include <cassert>
#include <fstream>
#include <random>

#include "../global/N44Matrix.hpp"
using namespace std;
typedef unsigned long long ULL;

//inputs: genome, sequence count, sequence lenght, error probability matrix
//outputs: fastq file of corrupted sequences, sequence to genome map file

void generateCorrSequences(const string & genome, ULL seqCount,
                           unsigned seqLen, const N44Matrix& probMatrix, unsigned randGenSeed,
                           ofstream& fastqFile, ofstream& mapFile);

void generateErrors(string & sequence, const N44Matrix& probMatrix, mt19937_64& randNumGenerator);

string createFastqEntry(ULL seqId, string sequence, string quality);

#endif /* defined(__testing_system__CorruptedSeqGenerator__) */
