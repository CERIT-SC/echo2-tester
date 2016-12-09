//
//  ProbMatrixGetter.h
//  testing system
//
//  Created by Miloš Šimek on 18.04.15.
//  Copyright (c) 2015 Miloš Šimek. All rights reserved.
//

#ifndef __testing_system__ProbMatrixGetter__
#define __testing_system__ProbMatrixGetter__

#include <string>
#include <fstream>
#include <memory>
#include <cctype>
#include "../global/N44Matrix.h"

//probability is in %
N44Matrix generateProbMatrix(unsigned uniformProbability, unsigned matrixLength);

class ProbMatrixLoader {
    bool loadingIsOk = true;
    unique_ptr<N44Matrix> matrixPtr;
    
public:
    ProbMatrixLoader(string matrixFileName, unsigned matrixLength);
    bool loadedCorrectly();
    N44Matrix getMatrix();
};

#endif /* defined(__testing_system__ProbMatrixGetter__) */
