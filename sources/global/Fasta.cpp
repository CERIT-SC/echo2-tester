//
//  FastaFile.cpp
//  Corrector Tester
//
//  Created by Miloš Šimek on 13/12/2016.
//
//

#include "Fasta.hpp"

ULL Fasta::getFragmentCount() {
    return fragments.size();
}

string& Fasta::getIdentifier(ULL index) {
    if (index >= fragments.size()) throw FastaException("out-of-bounds");
    
    return fragments[index].identifier;
}

string& Fasta::getData(ULL index) {
    if (index >= fragments.size()) throw FastaException("out-of-bounds");
    
    return fragments[index].data;
}

void Fasta::loadFromFile(string fileName) {
    ifstream file(fileName);
    
    if (!file.is_open()) {
        throw FastaException("cannot-open-file");
    }
    
    string line;
    while(file.good()) {
        getline(file, line);
        boost::algorithm::trim(line);
        
        //ignoring empty lines
        if (line.empty()) continue;
        
        //ignoring comments
        if (line[0] == ';') continue;

        //reading sequence descriptor
        if (line[0] == '>') {
            fragments.push_back(Fragment());
            fragments.back().identifier = line.substr(1);
            
        //reading sequence data
        } else {
            if (fragments.empty()) throw FastaException("fasta-format-error");
            
            fragments.back().data.append(line);
        }
    }
    
    if (file.bad()) throw FastaException("error-reading-file");
    
    //loaded data test
    for (Fragment &f: fragments) {
        if (f.identifier.empty() || f.data.empty()) {
            throw FastaException("fasta-format-error");
        }
    }
}
