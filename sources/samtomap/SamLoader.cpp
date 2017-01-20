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
    //fail() guarantees, that last loaded entry is invalid
    //using eof() shows the end of file, but it's uncertain
    //whether last entry is valid or not
    return file.fail();
}
