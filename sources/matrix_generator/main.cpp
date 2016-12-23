//
//  main.cpp
//  matrix_generator
//
//  Created by Miloš Šimek on 03.06.15.
//  Copyright (c) 2015 Miloš Šimek. All rights reserved.
//

#include <iostream>
#include <ctime>
#include <fstream>
#include <cstdlib>

#include "MatrGenOptions.hpp"
#include "../global/N44Matrix.hpp"
#include "../global/Optional.hpp"
using namespace std;

//function declarations
N44Matrix generateRandMatrix(unsigned length, float mean);
double getRandDouble(double rangeMin, double rangeMax, int numOfValues);


int main(int argc, const char * argv[]) {
    
    //load parameters
    MatrGenOptions options(argc, argv);
    if(options.optionsState() == OPS_HELP) return 0;
    if(options.optionsState() == OPS_ERR)  return 1;
    
    //setup random generator seed
    if (Optional<unsigned> seed = options.seed()) srand(*seed);
    else srand(static_cast<unsigned>(time(nullptr)));
    
    //generate matrix
    N44Matrix matrix = generateRandMatrix(options.getMatrixLength(),
                                          options.getMatrixErrorMean()*0.01);
    
    //safe matrix into file
    ofstream file(options.getFileName());
    if(!file.is_open()) {
        cerr << "Cannot safe to specified file" << endl << endl;
        return 1;
    }
    file << matrix;
    
    return 0;
}

N44Matrix generateRandMatrix(unsigned length, float mean) {
    N44Matrix matrix(length);
    float partialErrorMax = (mean/3) * 2; //divided between 3 positions
    
    for (unsigned pos=0; pos < length; pos++) {
        for (int column=0; column < 4; column++) {
            
            //fill row
            double rowSum = 0;
            for (int row=0; row < 4; row++) {
                if (column != row) {
                    rowSum += matrix(pos, row, column) = getRandDouble(0, partialErrorMax, 10000);
                }
            }
            
            //compute diagonal
            matrix(pos, column, column) = 1.0 - rowSum;
        }

    }
    
    return matrix;
}

double getRandDouble(double rangeMin, double rangeMax, int numOfValues) {
    double step = (rangeMax - rangeMin)/numOfValues;
    
    return rangeMin + (rand() % numOfValues+1)*step;
}
