//
//  DnaGenOptions.h
//  testing system
//
//  Created by Miloš Šimek on 27.03.15.
//  Copyright (c) 2015 Miloš Šimek. All rights reserved.
//

#ifndef testing_system_dnagenoptions_h
#define testing_system_dnagenoptions_h

#include <iostream>
#include <exception>
#include <string>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <boost/program_options.hpp>

namespace po = boost::program_options;
using namespace std;

//function prototypes
po::variables_map loadOptions(int argc, const char * argv[]);
unsigned long long getValidLength(po::variables_map options);
unsigned getValidProbability(po::variables_map options, const char * optionName);



//implementation
po::variables_map loadOptions(int argc, const char * argv[]) {
    
    po::options_description optionDescription("Options");
    
    //setup options
    optionDescription.add_options()
    ("help", "Produce help message\n")
    
    ("length,l", po::value<string>(), "Length of generated genome (required)")
    ("seed,s", po::value<unsigned>(), "Sets seed for pseudo-random generation (optional)")
    ("file,f", po::value<string>()->default_value("genome.txt"), "Name of file for saving generated genome\n")
    
    ("prob-a,a", po::value<int>()->default_value(25), "Sets probalibity of base A")
    ("prob-c,c", po::value<int>()->default_value(25), "Sets probalibity of base C")
    ("prob-g,g", po::value<int>()->default_value(25), "Sets probability of base G")
    ("prob-t,t", po::value<int>()->default_value(25), "Sets probability of base T");
    
    //load options
    po::variables_map options;
    try {
        po::store(po::parse_command_line(argc, argv, optionDescription), options);
    } catch (exception &e) {
        cerr << "Incorrect input: " << e.what() << endl << endl;
        exit(1);
    }
    po::notify(options);
    
    
    //print help
    if (options.count("help")) {
        cout << optionDescription << endl;
        exit(0);
    }
    
    return options;
}

unsigned long long getValidLength(po::variables_map options) {
    if (!options.count("length")) {
        cerr << "Genome length is required." << endl << endl;
        exit(1);
    }
    
    //load length in standard numeric format or with suffix (k,m,g,t,K,M,G,T)
    //load length
    double length;
    
    istringstream lengthStream(options["length"].as<string>());
    lengthStream >> length;
    
    if(lengthStream.fail()) {
        cerr << "Genome length was entered incorrectly." << endl << endl;
        exit(1);
    }
    
    if (length <= 0) {
        cerr << "Genome length must have non-zero value." << endl << endl;
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
            cerr << "Genome length suffix was entered incorrectly." << endl << endl;
            exit(1);
        }
    }
    
    //whole number test
    unsigned long long finalLenght = length;
    if(finalLenght < length) {
        cerr << "Genome length must be whole number." << endl << endl;
        exit(1);
    }
    
    return finalLenght;
}

unsigned getValidProbability(po::variables_map options, const char * optionName) {
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


#endif
