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
#include <sstream>
#include <boost/algorithm/string.hpp>
using namespace std;

struct SamEntry {
    string fragmentName = "";
    long   position = 1;
    string cigar = "";
};


class SamLoader {
    ifstream& file;
    bool eof = false;
    
public:
    SamLoader(ifstream& file);
    
    SamEntry loadNextEntry();
    bool endOfFile();
    
private:
    bool lineIsValid(string line);
    SamEntry parseLine(const string& line);
};

class SamEntryException : public std::runtime_error {
public:
    SamEntryException(string m) : runtime_error(m) {}
};

#endif /* SamLoader_hpp */
