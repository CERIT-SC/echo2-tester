//
//  main.cpp
//  Corrector Tester
//  sequence_generator
//
//  Created by Miloš Šimek on 15/12/2016.
//
//

#include <iostream>

#include "SeqGenOptions.hpp"
#include "ProbMatrixLoader.hpp"
#include "../global/N44Matrix.hpp"
#include "../global/Fasta.hpp"
using namespace std;


N44Matrix getMatrix(SeqGenOptions& options);
N44Matrix generateUniformProb(float uniformProbability, unsigned matrixLength);
void loadGenome(Fasta& genome, SeqGenOptions& options);



int main(int argc, const char * argv[]) {
    
    //load options
    SeqGenOptions options(argc, argv);
    if (options.optionsState() == OPS_HELP) return 0;
    if (options.optionsState() == OPS_ERR)  return 1;
    
    //load input data
    N44Matrix probMatrix = getMatrix(options);
    Fasta genome;
    loadGenome(genome, options);
    
    
    //algoritmus generování sequencí musí počítat s tím
    //že fragment může být kratší než sekvence (i délky 0)
    //v té chvíli by měl algoritmus daný fragment ignorovat
    
}


//generates/loads probability matrix
N44Matrix getMatrix(SeqGenOptions& options) {
    N44Matrix probMatrix;
    
    if (options.uniformProbability()) {     //returned as Optional
        probMatrix = generateUniformProb(*options.uniformProbability(), options.getSeqLength());
        
    } else {
        ProbMatrixLoader loader(*options.probabilityFileName(), options.getSeqLength());
        if (!loader.loadedCorrectly()) {
            cerr << "Probability matrix has not been loaded correctly." << endl << endl;
            exit(1);
        }
        probMatrix = loader.getMatrix();
    }
    
    return probMatrix;
}

void loadGenome(Fasta& genome, SeqGenOptions& options) {
    try {
        genome.loadFromFile(options.getGenomeFileName());
    } catch (...) {
        cout << "Could not load genome file" << endl;
        cout << endl;
        exit(1);
    }
    
    if (genome.getFragmentCount() == 0) {
        cout << "No genome data loaded" << endl;
        cout << endl;
        exit(1);
    }
}

N44Matrix generateUniformProb(float uniformProbability, unsigned matrixLength) {
    N44Matrix probMatrix(matrixLength);
    
    double primaryProb = static_cast<double>(100-uniformProbability) / 100;
    double secondProb = static_cast<double>(uniformProbability) / (3*100);
    
    for (unsigned index = 0; index < matrixLength; index++) {
        for (unsigned row = 0; row < 4; row++) {
            for (unsigned column = 0; column < 4; column++) {
                
                if(row == column) probMatrix(index,row,column) = primaryProb;
                else probMatrix(index,row,column) = secondProb;
            }
        }
    }
    
    return probMatrix;
}
