//
//  MatrGenOptions.h
//  testing system
//
//  Created by Miloš Šimek on 03.06.15.
//  Copyright (c) 2015 Miloš Šimek. All rights reserved.
//

#ifndef __testing_system__MatrGenOptions__
#define __testing_system__MatrGenOptions__

#include <string>
#include <iostream>
#include <boost/program_options.hpp>
#include "../Global/Optional.h"

using namespace std;
namespace po = boost::program_options;

enum OptionsState {
    OPS_OK, OPS_HELP, OPS_ERR
};

class MatrGenOptions {
    OptionsState opState = OPS_OK;
    po::variables_map optionMap;
    
public:
    MatrGenOptions(int argc, const char * argv[]);
    
    unsigned getMatrixLength();
    float    getMatrixErrorMean();
    string   getFileName();
    
    Optional<unsigned> seed();
    
    OptionsState optionsState();
    
private:
    void checkOptionValidity();
    void checkForExistence(const char * option, const char * errOutput);
    void setOptionError(const char * message);
};

#endif /* defined(__testing_system__MatrGenOptions__) */
