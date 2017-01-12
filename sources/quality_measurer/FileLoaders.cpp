//
//  FileLoaders.cpp
//  testing system
//
//  Created by Miloš Šimek on 30.04.15.
//  Copyright (c) 2015 Miloš Šimek. All rights reserved.
//

#include "FileLoaders.hpp"

string loadNextSeq(ifstream& inputFile) {
    string seq;
    streamsize unlimited = numeric_limits<streamsize>::max();
    
    //load from file
    //line 1
    inputFile.ignore(unlimited, '\n');
    
    //line 2
    inputFile >> seq;
    if (!seq.size()) return "";
    inputFile.ignore(unlimited, '\n');
    
    //line 3
    char ch = inputFile.get();
    if (ch != '+') throw runtime_error("incorrect-fastq-signature");
    inputFile.ignore(unlimited, '\n');
    
    //line 4
    inputFile.ignore(unlimited, '\n');
    
    
    //check sequence correctness
    for (char ch: seq) {
        if (count(allowedChar.begin(), allowedChar.end(), ch) == 0) {
            throw runtime_error("seq-bad-data");
        }
    }
    
    return seq;
    
    //this function may not detect file corruption and may return end of file
    //(string "") when file corruption happens
    //shouldn't be a problem: number of loaded sequences should be seen in program output
}

Optional<pair<unsigned, ULL>> loadNextMapEntry(ifstream& mapFile) {
    try {
        string firstEntry;
        mapFile >> firstEntry;
        
        //find out if sequence is mapped to genome
        if (firstEntry == string("notMapped")) return Opt::NoValue;
        
        //load fragment
        int fragment = stoi(firstEntry); //throws exeption if can't convert
        if (fragment < 0) throw;
        
        //load position
        ULL position;
        mapFile >> position;
        
        //check file
        if (mapFile.fail()) throw;
        
        return make_pair(static_cast<unsigned>(fragment), position);
        
    } catch (...) {
        throw runtime_error("corrupted-map-file");
    }
}

