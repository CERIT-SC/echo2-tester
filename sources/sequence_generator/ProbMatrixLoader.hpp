//
//  ProbMatrixLoader.hpp
//  Corrector Tester
//
//  Created by Miloš Šimek on 15/12/2016.
//
//

#ifndef ProbMatrixLoader_hpp
#define ProbMatrixLoader_hpp

#include <string>
#include <fstream>
#include <memory>
#include <cctype>

#include "../global/N44Matrix.hpp"

class ProbMatrixLoader {
    bool loadingIsOk = true;
    unique_ptr<N44Matrix> matrixPtr;
    
public:
    ProbMatrixLoader(string matrixFileName, unsigned matrixLength);
    bool loadedCorrectly();
    N44Matrix getMatrix();
};

#endif /* ProbMatrixLoader_hpp */
