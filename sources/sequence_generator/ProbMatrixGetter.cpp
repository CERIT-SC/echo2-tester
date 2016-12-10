//
//  ProbMatrixGetter.cpp
//  testing system
//
//  Created by Miloš Šimek on 18.04.15.
//  Copyright (c) 2015 Miloš Šimek. All rights reserved.
//

#include "ProbMatrixGetter.hpp"

N44Matrix generateProbMatrix(unsigned uniformProbability, unsigned matrixLength) {
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

ProbMatrixLoader::ProbMatrixLoader(string matrixFileName, unsigned matrixLength) {
    //prepare input file and matrix
    ifstream matrixFile(matrixFileName);
    
    if(!matrixFile.is_open()) {
        loadingIsOk = false;
        return;
    }
    
    matrixPtr = unique_ptr<N44Matrix>(new N44Matrix(matrixLength));
    N44Matrix& matrix = *matrixPtr;
    
    //load data from file
    unsigned index = 0;
    for (; index < matrixLength; index++) {
        for (unsigned row = 0; row < 4; row++) {
            for (unsigned column = 0; column < 4; column++) {
                
                matrixFile >> matrix(index, row, column);
            }
        }
        
        if (matrixFile.fail()) {
            loadingIsOk = false;
            return;
        }
        
        //checks if all data has been loaded
        while (isspace(matrixFile.peek())) matrixFile.get();
        if (matrixFile.eof()) break;
    }
    
    //fill remainder
        //if matrix in file is shorter, the last 4x4 matrix is copied
        //to remaining positions
    if (index < matrixLength) {
        unsigned lastLoadedIndex = index;
        
        for (; index < matrixLength; index++) {
            for (unsigned row = 0; row < 4; row++) {
                for (unsigned column = 0; column < 4; column++) {
                    matrix(index, row, column) = matrix(lastLoadedIndex, row, column);
                }
            }
        }
    }
}

bool ProbMatrixLoader::loadedCorrectly() {
    return loadingIsOk;
}

N44Matrix ProbMatrixLoader::getMatrix() {
    return *matrixPtr;
}
