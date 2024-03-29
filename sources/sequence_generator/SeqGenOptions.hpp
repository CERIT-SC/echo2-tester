//
//  SeqGenOptions.h
//  testing system
//
//  Created by Miloš Šimek on 14.04.15.
//  Copyright (c) 2015 Miloš Šimek. All rights reserved.
//

#ifndef testing_system_SeqGenOptions_h
#define testing_system_SeqGenOptions_h

#include <string>
#include <iostream>
#include <exception>
#include <boost/program_options.hpp>

#include "../global/Optional.hpp"
#include "../global/GlobalSetup.hpp"

using namespace std;
namespace po = boost::program_options;
typedef unsigned long long ULL;


enum OptionsState {
    OPS_OK, OPS_HELP, OPS_ERR
};

class SeqGenOptions {
    OptionsState opState = OPS_OK;
    po::variables_map optionMap;
    
public:
    SeqGenOptions(int argc, const char * argv[]);
    
    string      getGenomeFileName();
    float       getCoverage();
    unsigned    getSeqLength();
    string      getOutputSequenceFileName();
    string      getOutputMapFileName();
    Optional<unsigned> randGenSeed();
    
    //only one way to retrieve probability is valid in given run
    Optional<float>  uniformProbability();
    Optional<string> probabilityFileName();
    
    OptionsState optionsState();
    
private:
    void checkOptionValidity();
    void checkForExistence(const char * option, const char * errOutput);
    void setOptionError(string message);
};

#endif
