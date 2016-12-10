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
        po::store(po::parse_command_line(argc, argv, optionDescription), options);
    } catch (exception &e) {
        cerr << "Incorrect input: " << e.what() << endl << endl;
        exit(1);
    }
    po::notify(options);
    
    //print help
    if (options.count("help")) {
        cout << "Genome Generator" << endl;
        cout << "This program generates pseudo-random genome fragments in fasta format." << endl;
        cout << "Program will generate -n fragments of length -l." << endl << endl;
        
        cout << "Only length of fragments is required." << endl;
        cout << "Length can have suffix k (kilo), m (mega) g (giga) t (tera)" << endl;
        cout << "For example: 4, 4k, 7M, 9.4G" << endl << endl;
        
        cout << optionDescription << endl;
        exit(0);
    }
    
    //parse
    parseLength();
    parseNum();
    parseSeed();
    parseFileName();
    parseProbabilities();
}


ULL DnaGenOptions::getLenthg() { return length; }
unsigned DnaGenOptions::getNum() { return num; }
Optional<unsigned> DnaGenOptions::getSeed() { return seed; }
string DnaGenOptions::getFileName() { return fileName; }
Probabilities DnaGenOptions::getProbabilities() { return prob; }


//private
void DnaGenOptions::parseLength() {
    if (!options.count("length")) {
        cerr << "Fragment length must be specified." << endl;
        cerr << "For help, run with: --help" << endl << endl;
        exit(1);
    }
    
    //load length in standard numeric format or with suffix (k,m,g,t,K,M,G,T)
    //i.e. 4, 7k, 9.4M
    //load length
    double length;
    
    istringstream lengthStream(options["length"].as<string>());
    lengthStream >> length;
    
    if(lengthStream.fail()) {
        cerr << "Fragment length was entered incorrectly." << endl << endl;
        exit(1);
    }
    
    if (length <= 0) {
        cerr << "Fragment length must have non-zero value." << endl << endl;
        exit(1);
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
            cerr << "Fragment length suffix was entered incorrectly." << endl << endl;
            exit(1);
        }
    }
    
    //whole number test
    if(floor(length) < length) {
        cerr << "Fragment length must be whole number." << endl << endl;
        exit(1);
    }
    
    this->length = length;
}

void DnaGenOptions::parseNum() {
    int fragNum = options["num"].as<int>();
    
    if(fragNum < 1) {
        cerr << "Number of generated fragments must be 1 or higher." << endl << endl;
        exit(1);
    }
    
    num = fragNum;
}

void DnaGenOptions::parseSeed() {
    if(options.count("seed")) seed = options["seed"].as<unsigned>();
    else seed = Opt::NoValue;
}

void DnaGenOptions::parseFileName() {
    fileName = options["file"].as<string>();
}

void DnaGenOptions::parseProbabilities() {
    prob.A = getParsedProbability("prob-a");
    prob.C = getParsedProbability("prob-c");
    prob.G = getParsedProbability("prob-g");
    prob.T = getParsedProbability("prob-t");
    
    if((prob.A + prob.C + prob.G + prob.T) != 100) {
        cerr << "Sum of probabilities must be 100." << endl << endl;
        exit(1);
    }
}

unsigned DnaGenOptions::getParsedProbability(const char * optionName) {
    int probability;
    
    try {
        probability = options[optionName].as<int>();
    } catch (exception &e) {
        cerr << "Probability value entered incorrectly." << endl << endl;
        exit(1);
    }
    
    if (probability < 0 || probability > 100) {
        cerr << "Incorrect probability value." << endl << endl;
        exit(1);
    }
    
    return probability;
}
