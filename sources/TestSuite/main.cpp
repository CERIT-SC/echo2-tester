//
//  main.cpp
//  TestSuite
//
//  Created by Miloš Šimek on 17.04.15.
//  Copyright (c) 2015 Miloš Šimek. All rights reserved.
//

#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include <ctime>
#include <iostream>
using namespace std;

int main(int argc, const char * argv[]) {
    clock_t start = clock();
    
    // Get the top level suite from the registry
    CppUnit::Test *suite = CppUnit::TestFactoryRegistry::getRegistry().makeTest();
    
    // Adds the test to the list of test to run
    CppUnit::TextUi::TestRunner runner;
    runner.addTest( suite );
    
    // Change the default outputter to a compiler error format outputter
    runner.setOutputter( new CppUnit::CompilerOutputter( &runner.result(),
                                                        std::cerr ) );
    // Run the tests.
    bool wasSucessful = runner.run();
    
    cout << static_cast<double>(clock() - start)/1000 << " ms" << endl;
    
    // Return error code 1 if the one of test failed.
    return wasSucessful ? 0 : 1;
}

