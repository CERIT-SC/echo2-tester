//
//  Measuring.hpp
//  Corrector Tester
//
//  Created by Miloš Šimek on 22/12/2016.
//
//

#ifndef Measuring_hpp
#define Measuring_hpp

#include <iostream>
#include <string>
#include <stdexcept>
#include <tuple>
#include <utility>
#include <boost/algorithm/string.hpp>

#include "FileLoaders.hpp"
#include "../global/Fasta.hpp"
#include "../global/Optional.hpp"
using namespace std;
typedef unsigned long long ULL;

enum BaseState {
    BS_TruePositive  = 0,
    BS_FalsePositive = 1,
    BS_TrueNegative  = 2,
    BS_FalseNegative = 3,
};

struct MeasuredData {
    ULL counter[4] = {0,0,0,0};
    ULL originalErrors = 0;
    ULL unalteredSeqCount = 0;
    ULL seqCount = 0;
    ULL skippedSeqCount = 0;
};

MeasuredData measure(ifstream& corruptedSeqFile,
                     ifstream& correctedSeqFile,
                     ifstream& mapFile,
                     Fasta& genome);


#endif /* Measuring_hpp */
