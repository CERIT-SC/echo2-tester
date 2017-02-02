//
//  SamLoader.cpp
//  Corrector Tester
//
//  Created by Miloš Šimek on 20/01/2017.
//
//

#include "SamLoader.hpp"

#include <iostream>
SamLoader::SamLoader(ifstream& file) : file(file) {}

SamEntry SamLoader::loadNextEntry() {
    string line;
    getline(file, line);
    
    //ignoring unimportant lines
    while (!lineIsValid(line)) {
        line = "";
        if (file.good()) getline(file, line);
        else break;
    }
    
    //check file
    if (file.bad()) throw SamEntryException("file-loading-failed");
    
    //detect end of file
    if (line == "") {
        eof = true;
        return SamEntry();
    }

    return parseLine(line);
}

bool SamLoader::endOfFile() {
    return eof;
}

bool SamLoader::lineIsValid(string line) {
    boost::trim(line);
    
    //ignore empty lines
    if (line.length() == 0) return false;
    
    //ignore info about fragments
    if (line[0] == '@') return false;
    
    return true;
}

SamEntry SamLoader::parseLine(const string& line) {
    SamEntry entry;
    string ignore;
    istringstream stream(line);
    
    stream >> ignore >> ignore >> entry.fragmentName;
    stream >> entry.position >> ignore >> entry.cigar;
    
    //test
    if (stream.fail()) throw SamEntryException("bad-format");
    if (entry.fragmentName != "*" && entry.position < 1) throw SamEntryException("bad-format");
    
    return entry;
}
