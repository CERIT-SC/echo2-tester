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
#include <stdexcept>
#include <utility>
#include <algorithm>

#include "Setup.hpp"
#include "../global/Optional.hpp"

using namespace std;
typedef unsigned long long ULL;

//returns empty string when end of file occurs
string loadNextSeq(ifstream& inputFile);

//pair - fragment, position
//returns NoValue if sequence isn't mapped
Optional<pair<unsigned, ULL>> loadNextMapEntry(ifstream& mapFile);

#endif
