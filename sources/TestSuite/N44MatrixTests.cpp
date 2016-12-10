//
//  N44MatrixTests.cpp
//  testing system
//
//  Created by Miloš Šimek on 17.04.15.
//  Copyright (c) 2015 Miloš Šimek. All rights reserved.
//

#include "N44MatrixTests.hpp"

CPPUNIT_TEST_SUITE_REGISTRATION(N44MatrixTests);

void N44MatrixTests::setUp() {
    matrix = new N44Matrix(10);
}

void N44MatrixTests::tearDown() {
    delete matrix;
}

void N44MatrixTests::emptyMatrixTest() {
    //tested via assert in implementation
}

void N44MatrixTests::emptyValueTest() {
    CPPUNIT_ASSERT_EQUAL(10u, matrix->getSize());
    
    for (int i=0; i<10; i++) {
        for (int b1=0; b1 < 4; b1++) {
            for (int b2 = 0; b2 < 4; b2++) {
                CPPUNIT_ASSERT_EQUAL(0.0, matrix->operator()(i, b1, b2));
            }
        }
    }
}

//tests if matrix retains values
void N44MatrixTests::saveValuesTest() {
    matrix->operator()(0, 0, 0) = 4;
    matrix->operator()(5, 1, 2) = 5;
    matrix->operator()(9, 3, 3) = 6;
    
    CPPUNIT_ASSERT_EQUAL(4.0, matrix->operator()(0, 0, 0));
    CPPUNIT_ASSERT_EQUAL(5.0, matrix->operator()(5, 1, 2));
    CPPUNIT_ASSERT_EQUAL(6.0, matrix->operator()(9, 3, 3));
}

void N44MatrixTests::copyTest() {
    matrix->operator()(0, 0, 0) = 4;
    matrix->operator()(5, 1, 2) = 5;
    matrix->operator()(9, 3, 3) = 6;
    
    N44Matrix mat1 = *matrix;
    N44Matrix mat2;
    mat2 = *matrix;
    
    CPPUNIT_ASSERT_EQUAL(10u, mat1.getSize());
    CPPUNIT_ASSERT_EQUAL(10u, mat2.getSize());
    
    CPPUNIT_ASSERT_EQUAL(4.0, mat1(0, 0, 0));
    CPPUNIT_ASSERT_EQUAL(5.0, mat1(5, 1, 2));
    CPPUNIT_ASSERT_EQUAL(6.0, mat1(9, 3, 3));
    
    CPPUNIT_ASSERT_EQUAL(4.0, mat2(0, 0, 0));
    CPPUNIT_ASSERT_EQUAL(5.0, mat2(5, 1, 2));
    CPPUNIT_ASSERT_EQUAL(6.0, mat2(9, 3, 3));
    
    mat1(0, 0, 0) = mat1(5, 1, 2) = mat1(9, 3, 3) = 0;
    mat2(0, 0, 0) = mat2(5, 1, 2) = mat2(9, 3, 3) = 0;
    
    for (int i=0; i<10; i++) {
        for (int b1=0; b1 < 4; b1++) {
            for (int b2 = 0; b2 < 4; b2++) {
                CPPUNIT_ASSERT_EQUAL(0.0, mat1(i, b1, b2));
                CPPUNIT_ASSERT_EQUAL(0.0, mat2(i, b1, b2));
            }
        }
    }
}

void N44MatrixTests::addOperatorTest() {
    N44Matrix mat2(10);
    
    mat2(0,0,0) = 2;
    mat2(5,2,1) = 3;
    mat2(9,3,3) = 4;
    
    matrix->operator()(0, 0, 0) = 4;
    matrix->operator()(5, 2, 1) = 5;
    matrix->operator()(9, 3, 3) = 6;
    
    mat2 += *matrix;
    
    CPPUNIT_ASSERT_EQUAL(6.0, mat2(0,0,0));
    CPPUNIT_ASSERT_EQUAL(8.0, mat2(5,2,1));
    CPPUNIT_ASSERT_EQUAL(10.0, mat2(9,3,3));
}
