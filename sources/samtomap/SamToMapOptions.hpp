//
//  SamToMapOptions.hpp
//  Corrector Tester
//
//  Created by Miloš Šimek on 18/01/2017.
//
//

#ifndef SamToMapOptions_hpp
#define SamToMapOptions_hpp

#include <iostream>
#include <string>
#include <stdexcept>
#include <boost/program_options.hpp>

#include "../global/GlobalSetup.hpp"

using namespace std;
namespace po = boost::program_options;

enum OptionsState {
    OPS_OK, OPS_HELP, OPS_ERR
};

class SamToMapOptions {
    OptionsState opState = OPS_OK;
    po::variables_map optionMap;
    
public:
    SamToMapOptions(int argc, const char * argv[]);
    
    string getSamFileName();
    string getGenomeFileName();
    string getMapFileName();
    
    OptionsState optionsState();
    
private:
    void checkOptionValidity();
    void checkForExistence(const char * option, const char * errOutput);
    void setOptionError(string message);
};

#endif /* SamToMapOptions_hpp */
