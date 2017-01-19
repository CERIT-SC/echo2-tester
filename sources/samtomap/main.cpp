//
//  main.cpp
//  samtomap
//
//  Created by Miloš Šimek on 17/01/2017.
//
//

#include <iostream>
#include <fstream>

#include "SamToMapOptions.hpp"
#include "../global/Fasta.hpp"

void convert(ifstream& samFile, Fasta& genome, ofstream& mapFile);


int main(int argc, const char * argv[]) {
    
    //load options
    SamToMapOptions options(argc, argv);
    if (options.optionsState() == OPS_HELP) return 0;
    if (options.optionsState() == OPS_ERR)  return 1;
    
    cout << "Loading files" << endl;
    
    //open sam file
    ifstream samFile(options.getSamFileName());
    if (!samFile.is_open()) {
        cerr << "Cannot open sam file" << endl << endl;
        return 1;
    }
    
    //load genome
    Fasta genome;
    try {
        genome.loadFromFile(options.getGenomeFileName());
    } catch (exception &e) {
        if (string(e.what()) == "cannot-open-file") {
            cerr << "Cannot open genome file" << endl << endl;
            return 1;
        }
        
        cerr << "Problem while loading genome file" << endl << endl;
        return 1;
    }
    
    //open map file for writing
    ofstream mapFile(options.getMapFileName());
    if (!mapFile.is_open()) {
        cerr << "Cannot create/open map file for writing" << endl << endl;
        return 1;
    }

    cout << "Converting" << endl;
    convert(samFile, genome, mapFile);
    
    cout << "Done" << endl;
    cout << endl;
    
    //přidat do compile.sh kompilaci samtomap
    //vytvořit makefile pro samtomap
    
    return 0;
}

void convert(ifstream& samFile, Fasta& genome, ofstream& mapFile) {
    //pro každou sekvenci
    //získat ze sam formátu jméno fragmentu (3tí sloupec)
    //najít fragment v fa a získat index
    //zapsat index do map souboru
    //pokud *, zapsat notMapped a pokračovat na další sekvenci
    
    //získat pozici v fragmentu 4tý sloupec (číslování od 1)
    //získat posunutí z 6tého sloupce (vyparsovat z řetězce)
    //upravit pozici o posunutí
    //zapsat pozici do map souboru
}
