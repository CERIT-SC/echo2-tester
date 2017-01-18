//
//  SamToMapOptions.hpp
//  Corrector Tester
//
//  Created by Miloš Šimek on 18/01/2017.
//
//

#ifndef SamToMapOptions_hpp
#define SamToMapOptions_hpp

#include <string>
#include <boost/program_options.hpp>

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
};

#endif /* SamToMapOptions_hpp */
