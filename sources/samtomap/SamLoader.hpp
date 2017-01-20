//
//  SamLoader.hpp
//  Corrector Tester
//
//  Created by Miloš Šimek on 20/01/2017.
//
//

#ifndef SamLoader_hpp
#define SamLoader_hpp

#include <fstream>
#include <string>
#include <limits>
#include <stdexcept>
using namespace std;

struct SamEntry {
    string fragmentName;
    long   position;
    string cigar;
};


class SamLoader {
    ifstream& file;
    
public:
    SamLoader(ifstream& file);
    
    SamEntry loadNextEntry();
    bool endOfFile();
};

class SamEntryException : public std::runtime_error {
public:
    SamEntryException(string m) : runtime_error(m) {}
};

#endif /* SamLoader_hpp */
