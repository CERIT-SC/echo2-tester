//
//  N44Matrix.h
//  testing system
//
//  Created by Miloš Šimek on 17.04.15.
//  Copyright (c) 2015 Miloš Šimek. All rights reserved.
//

#ifndef __testing_system__N44Matrix__
#define __testing_system__N44Matrix__

#include <vector>
#include <cassert>
#include <ostream>
#include <stdexcept>
using namespace std;

class N44Matrix {
    vector<double*> matrix;
    
public:
    N44Matrix() {}
    N44Matrix(unsigned length);
    N44Matrix(const N44Matrix&);
    ~N44Matrix();
    
    N44Matrix& operator=(const N44Matrix&);
    N44Matrix& operator+=(const N44Matrix&);
    double &operator()(unsigned indexN, unsigned row, unsigned column);
    const double& operator()(unsigned indexN, unsigned row, unsigned column) const;
    unsigned getSize() const;
    
    friend ostream& operator<<(ostream& os, N44Matrix &);
};

#endif /* defined(__testing_system__N44Matrix__) */
