//
//  N44Matrix.cpp
//  testing system
//
//  Created by Miloš Šimek on 17.04.15.
//  Copyright (c) 2015 Miloš Šimek. All rights reserved.
//

#include "N44Matrix.hpp"

N44Matrix::N44Matrix(unsigned length) {
    matrix.resize(length);
    
    for(double*& el: matrix) {
        el = new double[16];
        
        for (short i1=0; i1<4; i1++) {
            for (short i2=0; i2<4; i2++) {
                el[i1*4 + i2] = 0;
            }
        }
    }
}

N44Matrix::N44Matrix(const N44Matrix& mat2) {
    matrix.resize(mat2.matrix.size());
    
    for (unsigned index=0; index < matrix.size(); index++) {
        matrix[index] = new double[16];
        
        for (unsigned pos=0; pos < 16; pos++) {
            matrix[index][pos] = mat2.matrix[index][pos];
        }
    }
}

N44Matrix::~N44Matrix() {
    for(double*& el: matrix) {
        delete [] el;
    }
}

N44Matrix& N44Matrix::operator=(const N44Matrix& mat2) {
    if (matrix.size() != mat2.matrix.size()) {
        for(double*& el: matrix) delete [] el;
        
        matrix.resize(mat2.matrix.size());
        for (double*& el: matrix) el = new double[16];
    }
    
    for (unsigned index=0; index < matrix.size(); index++) {
        for (unsigned pos=0; pos < 16; pos++) {
            matrix[index][pos] = mat2.matrix[index][pos];
        }
    }
    
    return *this;
}

N44Matrix& N44Matrix::operator+=(const N44Matrix& mat2) {
    assert(matrix.size() == mat2.matrix.size());
    
    for (unsigned index=0; index<matrix.size(); index++) {
        for (short pos=0; pos < 16; pos++) {
            matrix[index][pos] += mat2.matrix[index][pos];
        }
    }
    
    return *this;
}

double &N44Matrix::operator()(unsigned indexN, unsigned row, unsigned column) {
    assert(indexN < matrix.size());
    assert(row < 4);
    assert(column < 4);
    
    return matrix[indexN][row*4 + column];
}

const double& N44Matrix::operator()(unsigned indexN, unsigned row, unsigned column) const {
    assert(indexN < matrix.size());
    assert(row < 4);
    assert(column < 4);
    
    return matrix[indexN][row*4 + column];
}

unsigned N44Matrix::getSize() const {
    return static_cast<unsigned>(matrix.size());
}


//ostream function
ostream& operator<<(ostream& os, N44Matrix& matrix) {
    for (vector<double*>::size_type index = 0; index < matrix.matrix.size(); index++) {
        for (int x=0; x<4; x++) {
            for (int y=0; y<4; y++) {
                os << matrix.matrix[index][x*4+y];
                if (y<3) os << " ";
            }
            if(x < 3) os << "\n";
        }
        if(index+1 < matrix.matrix.size()) os << "\n\n";
    }
    
    return os;
}
