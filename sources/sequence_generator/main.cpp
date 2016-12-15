//
//  main.cpp
//  Corrector Tester
//  sequence_generator
//
//  Created by Miloš Šimek on 15/12/2016.
//
//

#include <iostream>

#include "SeqGenOptions.hpp"
using namespace std;


int main(int argc, const char * argv[]) {
    
    //load options
    SeqGenOptions options(argc, argv);
    if (options.optionsState() == OPS_HELP) return 0;
    if (options.optionsState() == OPS_ERR)  return 1;
    
    
    
}
