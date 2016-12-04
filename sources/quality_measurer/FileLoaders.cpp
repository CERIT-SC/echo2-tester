//
//  FileLoaders.cpp
//  testing system
//
//  Created by Miloš Šimek on 30.04.15.
//  Copyright (c) 2015 Miloš Šimek. All rights reserved.
//

#include "FileLoaders.h"

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
    if (ch != '+') throw runtime_error("incorrect_fastq_signature");
    inputFile.ignore(unlimited, '\n');
    
    //line 4
    inputFile.ignore(unlimited, '\n');
    
    
    //check sequence correctness
    transform(seq.begin(), seq.end(), seq.begin(), ::toupper);
    for_each(seq.begin(), seq.end(), [](char c){
        if (c != 'A' && c != 'C' && c != 'G' && c != 'T') {
            throw runtime_error("seq_bad_data");
        }
    });
    
    return seq;
    
    //this function may not detect file corruption and may return end of file
    //(string "") when file corruption happens
    //shouldn't be a problem: number of loaded sequences should be seen in program output
}

ULL loadNextSeqPos(ifstream& mapFile) {
    if (!mapFile.good()) throw runtime_error("file_not_good");
    
    ULL position;
    mapFile >> position;
    if (mapFile.fail()) throw runtime_error("file_bad");
    
    return position;
}