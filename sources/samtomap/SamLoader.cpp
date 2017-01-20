//
//  SamLoader.cpp
//  Corrector Tester
//
//  Created by Miloš Šimek on 20/01/2017.
//
//

#include "SamLoader.hpp"

SamLoader::SamLoader(ifstream& file) : file(file) {}

SamEntry SamLoader::loadNextEntry() {
    string ignore;
    SamEntry entry;
    
    //read line
    file >> ignore >> ignore >> entry.fragmentName;
    file >> entry.position >> ignore >> entry.cigar;
    file.ignore(numeric_limits<streamsize>::max(), '\n');
    
    //check
    if (file.bad()) throw SamEntryException("problem-with-file");
    if (!file.fail() && entry.position < 0) throw SamEntryException("bad-format");

    return entry;
}

bool SamLoader::endOfFile() {
    return file.fail();
}
