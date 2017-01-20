//
//  main.cpp
//  samtomap
//
//  Created by Miloš Šimek on 17/01/2017.
//
//

#include <iostream>
#include <fstream>
#include <limits>
#include <stdexcept>

#include "SamToMapOptions.hpp"
#include "../global/Fasta.hpp"
typedef unsigned long long ULL;

void convert(ifstream& samFile, Fasta& genome, ofstream& mapFile);
int positionOffset(string cigar);


int main(int argc, const char * argv[]) {
    
    //load options
    SamToMapOptions options(argc, argv);
    if (options.optionsState() == OPS_HELP) return 0;
    if (options.optionsState() == OPS_ERR)  return 1;
    
    cout << "Loading files" << endl;
    
    //open sam file
    ifstream samFile(options.getSamFileName());
    if (!samFile.is_open()) {
        cerr << "Cannot open sam file" << endl << endl;
        return 1;
    }
    
    //load genome
    Fasta genome;
    try {
        genome.loadFromFile(options.getGenomeFileName());
    } catch (exception &e) {
        if (string(e.what()) == "cannot-open-file") {
            cerr << "Cannot open genome file" << endl << endl;
            return 1;
        }
        
        cerr << "Problem while loading genome file" << endl << endl;
        return 1;
    }
    
    //open map file for writing
    ofstream mapFile(options.getMapFileName());
    if (!mapFile.is_open()) {
        cerr << "Cannot create/open map file for writing" << endl << endl;
        return 1;
    }

    cout << "Converting" << endl;
    convert(samFile, genome, mapFile);
    
    cout << "Done" << endl;
    cout << endl;
    
    //přidat do compile.sh kompilaci samtomap
    //vytvořit makefile pro samtomap
    
    return 0;
}

void convert(ifstream& samFile, Fasta& genome, ofstream& mapFile) {
    
}

int positionOffset(string cigar) {
    //get position of M
    size_t Mpos = cigar.find("M");
    if (Mpos == string::npos) return 0;
    
    string cigarPart = cigar.substr(0, Mpos); //get part before M
    
    size_t Spos = cigarPart.find("S");
    if (Spos == string::npos) return 0;
    
    string offsetStr = cigarPart.substr(0, Spos);
    
    int offset;
    try {
        offset = stoi(offsetStr);
        if (offset < 0) throw 0;
    } catch (...) {
        throw runtime_error("cigar-format-error");
    }
    
    return offset;
}
