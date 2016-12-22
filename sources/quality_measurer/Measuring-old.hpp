//
//  Measuring.h
//  testing system
//
//  Created by Miloš Šimek on 30.04.15.
//  Copyright (c) 2015 Miloš Šimek. All rights reserved.
//

#ifndef __testing_system__Measuring__
#define __testing_system__Measuring__

#include <string>
#include <fstream>
#include <iostream>
#include <tuple>
#include <cassert>

#include "FileLoaders.hpp"
using namespace std;
typedef unsigned long long ULL;

enum BaseState {
    BS_TP,	// true positive
    BS_FP,	// false positive
    BS_TN,	// true negative
    BS_FN,	// false negative

    BS_MAX
};

struct MeasuredData {
    ULL counter[BS_MAX] = {0,0,0,0};
    ULL originalErrors = 0;
    ULL unalteredSeqCount = 0;
    ULL seqCount = 0;
};

MeasuredData measure(string& genome,
             ifstream& corruptedSeqFile, ifstream& correctedSeqFile,
             ifstream& mapFile);

unsigned countDifferences(string& first, string& second);
void countStates(string& genomeSeq, string& corruptedSeq, string& correctedSeq,
                 MeasuredData& measured);
bool isUnaltered(string& genomeSeq, string& corruptedSeq, string& correctedSeq);

BaseState getState(char genBase, char corruptedBase, char correctedBase);
tuple<string, string, string> loadSequences(ifstream& corruptedSeqFile,
                                            ifstream& correctedSeqFile,
                                            ifstream& mapFile,
                                            string& genome);

#endif /* defined(__testing_system__Measuring__) */
