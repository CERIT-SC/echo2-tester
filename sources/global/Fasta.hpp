//
//  FastaFile.hpp
//  Corrector Tester
//
//  Created by Miloš Šimek on 13/12/2016.
//
//

//Fasta loads and stores fasta files
//Fasta objects can throw exception of type FastaException
//getIndentifier and getData can throw exeption if index is
//out of bounds
//loadFromFile can throw an excpeption if problem during
//file loading occurs


#ifndef FastaFile_hpp
#define FastaFile_hpp

#include <vector>
#include <string>
#include <stdexcept>
#include <fstream>
#include <cctype>
#include <algorithm>
#include <boost/algorithm/string/trim.hpp>
using namespace std;
typedef unsigned long long ULL;

class Fasta {
    struct Fragment { string identifier, data; };
    
    vector<Fragment> fragments;
    
public:
    ULL getFragmentCount();
    string& getIdentifier(ULL index);
    string& getData(ULL index);
    
    void loadFromFile(string fileName);
};

class FastaException : public std::runtime_error {
public:
    FastaException(string m) : runtime_error(m) {}
};

#endif /* FastaFile_hpp */
