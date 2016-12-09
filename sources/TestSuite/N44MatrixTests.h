//
//  N44MatrixTests.h
//  testing system
//
//  Created by Miloš Šimek on 17.04.15.
//  Copyright (c) 2015 Miloš Šimek. All rights reserved.
//

#ifndef __testing_system__N44MatrixTests__
#define __testing_system__N44MatrixTests__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../global/N44Matrix.h"

class N44MatrixTests : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(N44MatrixTests);
    CPPUNIT_TEST(emptyMatrixTest);
    CPPUNIT_TEST(emptyValueTest);
    CPPUNIT_TEST(saveValuesTest);
    CPPUNIT_TEST(copyTest);
    CPPUNIT_TEST(addOperatorTest);
    CPPUNIT_TEST_SUITE_END();
    
    N44Matrix emptyMatrix, *matrix;
public:
    void setUp();
    void tearDown();
    
    void emptyMatrixTest();
    void emptyValueTest();
    void saveValuesTest();
    void copyTest();
    void addOperatorTest();
};

#endif /* defined(__testing_system__N44MatrixTests__) */
