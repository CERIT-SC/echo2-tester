//
//  DnaGenOptions.hpp
//  Genome Generator
//
//  Created by Miloš Šimek on 10/12/2016.
//
//

#ifndef DnaGenOptions_hpp
#define DnaGenOptions_hpp

#include <iostream>
#include <exception>
#include <string>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <cmath>
#include <boost/program_options.hpp>

#include "../global/Optional.hpp"
#include "../global/GlobalSetup.hpp"

namespace po = boost::program_options;
using namespace std;
typedef unsigned long long ULL;

struct Probabilities {
    unsigned A, C, G, T;
};

enum OptionsState {
    OPS_OK, OPS_HELP, OPS_ERR
};

class DnaGenOptions {
    OptionsState opState = OPS_OK;
    po::variables_map optionMap;
    
    ULL length;
    unsigned num;
    Optional<unsigned> seed;
    string fileName;
    Probabilities prob;
    
public:
    DnaGenOptions(int argc, const char * argv[]);
    
    ULL       getLenthg();
    unsigned  getNum();
    Optional<unsigned>  getSeed();
    string    getFileName();
    Probabilities getProbabilities();
    
    OptionsState optionsState();
    
private:
    bool parseLength();
    bool parseNum();
    void parseSeed();
    bool parseFileName();
    bool parseProbabilities();
    unsigned getParsedProbability(const char * optionName);
    
    void checkForExistence(const char * option, const char * errOutput);
    void setOptionError(string message);
};

//possible improvement - instead of using exit directly, options can
//be altered to return status as with other programs

#endif /* DnaGenOptions_hpp */
