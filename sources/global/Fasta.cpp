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
        
        //použít algorithm::all_of
    }
}
