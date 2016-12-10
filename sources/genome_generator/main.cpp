//
//  main.cpp
//  genome_generator
//
//  Created by Miloš Šimek on 10.03.15.
//  Copyright (c) 2015 Miloš Šimek. All rights reserved.
//

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <exception>
#include <fstream>
#include <boost/program_options.hpp>

#include "DnaGenOptions.h"

namespace po = boost::program_options;
using namespace std;


struct Probabilities {
    unsigned A, C, G, T;
};

void genomeGenerator(ostream & os, Probabilities prob, unsigned long long length);



int main(int argc, const char * argv[]) {
  
    //load options
    po::variables_map options = loadOptions(argc, argv);
    unsigned long long length = getValidLength(options);
    
    Probabilities prob;
    prob.A = getValidProbability(options, "prob-a");
    prob.C = getValidProbability(options, "prob-c");
    prob.G = getValidProbability(options, "prob-g");
    prob.T = getValidProbability(options, "prob-t");
    
    if((prob.A + prob.C + prob.G + prob.T) != 100) {
        cerr << "Sum of probabilities must be 100." << endl << endl;
        exit(1);
    }
    
    //set random generator seed
    if (options.count("seed")) { srand(options["seed"].as<unsigned>()); }
    else                       { srand(static_cast<unsigned>(time(nullptr))); }
    
    //generate and safe to text file
    ofstream genomeFile(options["file"].as<string>());
    if(!genomeFile.is_open()) {
        cerr << "Could not open output file." << endl << endl;
    }
    genomeGenerator(genomeFile, prob, length);
    
    return 0;
}

#define END_A prob.A
#define END_C prob.A + prob.C
#define END_G prob.A + prob.C + prob.G

//possible speed improvement - use string as a buffer
void genomeGenerator(ostream & os, Probabilities prob, unsigned long long length) {
    
    for(unsigned long long index = 0; index < length; index++) {
        int randNumber = rand() % 100 + 1;
        
        if (randNumber <= END_A) os << 'A';
        if (randNumber > END_A && randNumber <= END_C) os << 'C';
        if (randNumber > END_C && randNumber <= END_G) os << 'G';
        if (randNumber > END_G)  os << 'T';
    }
}
