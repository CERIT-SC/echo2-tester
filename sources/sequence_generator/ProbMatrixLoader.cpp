//
//  ProbMatrixLoader.cpp
//  Corrector Tester
//
//  Created by Miloš Šimek on 15/12/2016.
//
//

#include "ProbMatrixLoader.hpp"

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
