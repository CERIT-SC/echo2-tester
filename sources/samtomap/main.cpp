//
//  main.cpp
//  samtomap
//
//  Created by Miloš Šimek on 17/01/2017.
//
//

#include <iostream>
#include <fstream>
#include <stdexcept>

#include "SamToMapOptions.hpp"
#include "SamLoader.hpp"
#include "../global/Fasta.hpp"
typedef unsigned long long ULL;

void convert(ifstream& samFile, Fasta& genome, ofstream& mapFile);
void loadSamEntry(ifstream& samFile);
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
    
    return 0;
}

void convert(ifstream& samFile, Fasta& genome, ofstream& mapFile) {
    SamLoader samLoader(samFile);
    
    while (true) {
        //load info from sam file
        SamEntry entry = samLoader.loadNextEntry();
        if (samLoader.endOfFile()) break;
        
        //if sequence has no mapping, mark as notMapped
        if (entry.fragmentName == "*") {
            mapFile << "notMapped\n";
            continue;
        }
        
        //find fragment index
        ULL index = 0;
        for (; index < genome.getFragmentCount(); index++) {
            if (genome.getIdentifier(index).find(entry.fragmentName) != string::npos) break;
        }
        
        if (index == genome.getFragmentCount()) {
            cerr << "Corresponding genome fragment not found" << endl;
            cerr << endl;
            exit(1);
        }
        
        //compute sequence position using cigar string
            //numbering in sam is from 1 while in map from 0
        long position = entry.position - positionOffset(entry.cigar) - 1;
        
        if (position < 0) {
            cerr << "Corrupted data in sam file" << endl;
            cerr << endl;
            exit(1);
        }
        
        //write to map file
        mapFile << index << " " << position << "\n";
    }
    
    //test if map file is ok
    if (!mapFile.good()) {
        cerr << "Error while writing into map file" << endl;
        cerr << endl;
        exit(1);
    }
}


int positionOffset(string cigar) {
    //get position of M
    size_t Mpos = cigar.find("M");
    if (Mpos == string::npos) return 0;
    
    //get part before M
    string cigarPart = cigar.substr(0, Mpos);
    
    //get position of S
    size_t Spos = cigarPart.find("S");
    if (Spos == string::npos) return 0;
    
    //get offset number as string
    string offsetStr = cigarPart.substr(0, Spos);
    
    //extract offset
    int offset;
    try {
        offset = stoi(offsetStr);
        if (offset < 0) throw 0;
    } catch (...) {
        throw runtime_error("cigar-format-error");
    }
    
    return offset;
}
