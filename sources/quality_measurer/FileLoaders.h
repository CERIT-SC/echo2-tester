//
//  FileLoaders.h
//  testing system
//
//  Created by Miloš Šimek on 30.04.15.
//  Copyright (c) 2015 Miloš Šimek. All rights reserved.
//

#ifndef testing_system_FileLoaders_h
#define testing_system_FileLoaders_h

#include <string>
#include <fstream>
#include <limits>
#include <algorithm>
#include <stdexcept>
#include <cctype>
using namespace std;
typedef unsigned long long ULL;

string loadNextSeq(ifstream& inputFile);
ULL loadNextSeqPos(ifstream& mapFile);

#endif
