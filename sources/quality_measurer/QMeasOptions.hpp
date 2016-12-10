//
//  QMeasOptions.h
//  testing system
//
//  Created by Miloš Šimek on 30.04.15.
//  Copyright (c) 2015 Miloš Šimek. All rights reserved.
//

#ifndef __testing_system__QMeasOptions__
#define __testing_system__QMeasOptions__

#include <string>
#include <iostream>
#include <boost/program_options.hpp>

#include "../global/Optional.hpp"

namespace po = boost::program_options;
using namespace std;

enum OptionsState {
    OPS_OK, OPS_HELP, OPS_ERR
};

class QMeasOptions {
    OptionsState opState = OPS_OK;
    po::variables_map optionMap;
    
public:
    QMeasOptions(int argc, const char * argv[]);
    
    string getGenomeFName();
    string getCorruptedSeqFName();
    string getCorrectedSeqFName();
    string getSeqMapFName();
    Optional<string> getOutputFile();
    
    OptionsState optionsState();
};


#endif /* defined(__testing_system__QMeasOptions__) */
