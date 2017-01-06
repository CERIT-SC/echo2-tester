//
//  DnaGenOptions.cpp
//  Corrector Tester
//
//  Created by Miloš Šimek on 10/12/2016.
//
//

#include "DnaGenOptions.hpp"

DnaGenOptions::DnaGenOptions(int argc, const char * argv[]) {
    po::options_description optionDescription("Options");
    
    //setup options
    optionDescription.add_options()
    ("help", "Produce help message\n")
    
    ("length,l", po::value<string>(), "Length of generated fragments (required)\n"
     "Length can have suffix k (kilo), m (mega)\ng (giga) t (terra)")
    ("num,n", po::value<int>()->default_value(1), "Number of generated fragments")
    ("seed,s", po::value<unsigned>(), "Sets seed for pseudo-random generation")
    ("file,f", po::value<string>()->default_value("genome.fa"),
     "Name of file for saving generated fragments\n")
    
    ("prob-a,a", po::value<int>()->default_value(25), "Sets probalibity of base A")
    ("prob-c,c", po::value<int>()->default_value(25), "Sets probalibity of base C")
    ("prob-g,g", po::value<int>()->default_value(25), "Sets probability of base G")
    ("prob-t,t", po::value<int>()->default_value(25), "Sets probability of base T");
    
    //load options
    try {
        po::store(po::parse_command_line(argc, argv, optionDescription), optionMap);
    } catch (exception &e) {
        setOptionError(string("Incorrect input: ") + e.what());
        return;
    }
    po::notify(optionMap);
    
    //print help
    if (optionMap.count("help")) {
        opState = OPS_HELP;
        
        cout << endl;
        cout << "Genome Generator" << endl;
        cout << "Version: " << VERSION_STRING << endl;
        cout << "This tool generates pseudo-random genome fragments in fasta format." << endl;
        cout << "Program will generate -n fragments of length -l." << endl;
        cout << "Only parameter length is required." << endl;
        cout << "Length can have suffix k (kilo), m (mega) g (giga) t (tera)" << endl;
        cout << "For example: 4, 4k, 7M, 9.4G" << endl;
        cout << endl;
        cout << optionDescription << endl;
        return;
    }
    
    //parse
    if (!parseLength())    return;
    if (!parseNum())       return;
    parseSeed();
    if (!parseFileName())  return;
    if (!parseProbabilities()) return;
}


ULL DnaGenOptions::getLenthg() { return length; }
unsigned DnaGenOptions::getNum() { return num; }
Optional<unsigned> DnaGenOptions::getSeed() { return seed; }
string DnaGenOptions::getFileName() { return fileName; }
Probabilities DnaGenOptions::getProbabilities() { return prob; }
OptionsState DnaGenOptions::optionsState() { return opState; }


//private
bool DnaGenOptions::parseLength() {
    checkForExistence("length", "Fragment length must be specified");
    if (opState != OPS_OK) return false;
    
    //load length in standard numeric format or with suffix (k,m,g,t,K,M,G,T)
    //i.e. 4, 7k, 9.4M
    //load length
    double length;
    
    istringstream lengthStream(optionMap["length"].as<string>());
    lengthStream >> length;
    
    if(lengthStream.fail()) {
        setOptionError("Fragment length was entered incorrectly");
        return false;
    }
    
    if (length <= 0) {
        setOptionError("Fragment length must have non-zero value");
        return false;
    }
    
    //incorporate suffix
    string suffix;
    if(!lengthStream.eof()) lengthStream >> suffix;
    
    if(suffix.length()) {
        transform(suffix.begin(), suffix.end(), suffix.begin(), ::tolower);
        
        if (suffix == "k")      length *= 1000ull;
        else if(suffix == "m")  length *= 1000000ull;
        else if(suffix == "g")  length *= 1000000000ull;
        else if(suffix == "t")  length *= 1000000000000ull;
        else {
            setOptionError("Fragment length suffix was entered incorrectly");
            return false;
        }
    }
    
    //whole number test
    if(floor(length) < length) {
        setOptionError("Fragment length must be whole number");
        return false;
    }
    
    this->length = length;
    return true;
}

bool DnaGenOptions::parseNum() {
    int fragNum = optionMap["num"].as<int>();
    
    if(fragNum < 1) {
        setOptionError("Number of generated fragments must be 1 or higher");
        return false;
    }
    
    num = fragNum;
    return true;
}

void DnaGenOptions::parseSeed() {
    if(optionMap.count("seed")) seed = optionMap["seed"].as<unsigned>();
    else seed = Opt::NoValue;
}

bool DnaGenOptions::parseFileName() {
    if(optionMap["file"].as<string>() == "") {
        setOptionError("File name cannot be empty");
        return false;
    }
    
    fileName = optionMap["file"].as<string>();
    return true;
}

bool DnaGenOptions::parseProbabilities() {
    prob.A = getParsedProbability("prob-a");
    if (opState != OPS_OK) return false;
    
    prob.C = getParsedProbability("prob-c");
    if (opState != OPS_OK) return false;
    
    prob.G = getParsedProbability("prob-g");
    if (opState != OPS_OK) return false;
    
    prob.T = getParsedProbability("prob-t");
    if (opState != OPS_OK) return false;
    
    if((prob.A + prob.C + prob.G + prob.T) != 100) {
        setOptionError("Sum of probabilities must be 100");
        return false;
    }
    return true;
}

unsigned DnaGenOptions::getParsedProbability(const char * optionName) {
    int probability;
    
    try {
        probability = optionMap[optionName].as<int>();
    } catch (exception &e) {
        setOptionError("Probability value entered incorrectly");
        return 0;
    }
    
    if (probability < 0 || probability > 100) {
        setOptionError("Incorrect probability value");
        return 0;
    }
    
    return probability;
}

void DnaGenOptions::checkForExistence(const char * option, const char * errOutput) {
    if (!optionMap.count(option)) {
        setOptionError(errOutput);
        return;
    }
}

void DnaGenOptions::setOptionError(string message) {
    cerr << message << endl;
    cerr << "For help, run with --help" << endl;
    cerr << endl;
    opState = OPS_ERR;
}
