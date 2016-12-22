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
    array<char, 8> alowedChar = {'A', 'C', 'G', 'T', 'a', 'c', 'g', 't'};
    
    for (char ch: seq) {
        if (count(alowedChar.begin(), alowedChar.end(), ch) == 0) {
            throw runtime_error("seq-bad-data");
        }
    }
    
    return seq;
    
    //this function may not detect file corruption and may return end of file
    //(string "") when file corruption happens
    //shouldn't be a problem: number of loaded sequences should be seen in program output
}

pair<unsigned, ULL> loadNextMapEntry(ifstream& mapFile) {
    unsigned fragment;
    ULL position;
    mapFile >> fragment >> position;
    
    if (mapFile.fail()) throw runtime_error("file-bad");
    
    return make_pair(fragment, position);
}
