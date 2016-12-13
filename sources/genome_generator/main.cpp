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

#include "DnaGenOptions.hpp"

namespace po = boost::program_options;
using namespace std;
typedef unsigned long long ULL;

char generateBase(Probabilities prob);



int main(int argc, const char * argv[]) {
  
    //load options
    DnaGenOptions options(argc, argv);
    if (options.optionsState() == OPS_HELP) return 0;
    if (options.optionsState() == OPS_ERR)  return 1;
    
    //set random generator seed
    if (options.getSeed()) srand(*options.getSeed());
    else                   srand(static_cast<unsigned>(time(nullptr)));
    
    //init output file
    ofstream genomeFile(options.getFileName());
    if (!genomeFile.is_open()) {
        cerr << "Could not open output file" << endl << endl;
        exit(1);
    }
    
    //GENERATING FRAGMENTS
    cout << "Generating" << endl;
    Probabilities probs = options.getProbabilities();
    string line(80, '_');   //line length set up to 80
    
    for (unsigned fragment = 0; fragment < options.getNum(); fragment++) {
        genomeFile << ">Randomly generated fragment number " << fragment + 1 << "\n";
        ULL baseCount = options.getLenthg();
        
        //generate lines
        while (baseCount > 80) {
            for (char& base: line) base = generateBase(probs);
            genomeFile << line << "\n";
            baseCount -= 80;
        }
        
        //generate last reminder line
        if (baseCount > 0) {
            while (baseCount > 0) {
                genomeFile << generateBase(probs);
                baseCount--;
            }
            genomeFile << "\n";
        }
        
        if (fragment + 1 < options.getNum()) genomeFile << "\n";
    }
    
    cout << "Done" << endl;
    return 0;
}

#define END_A prob.A
#define END_C prob.A + prob.C
#define END_G prob.A + prob.C + prob.G

char generateBase(Probabilities prob) {
    int randNumber = (rand() % 100) + 1;
    
    if (randNumber <= END_A) return 'A';
    if (randNumber <= END_C) return 'C';
    if (randNumber <= END_G) return 'G';
    return 'T';
}
