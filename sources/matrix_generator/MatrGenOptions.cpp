//
//  MatrGenOptions.cpp
//  testing system
//
//  Created by Miloš Šimek on 03.06.15.
//  Copyright (c) 2015 Miloš Šimek. All rights reserved.
//

#include "MatrGenOptions.h"

MatrGenOptions::MatrGenOptions(int argc, const char * argv[]) {
    //define options
    po::options_description options("Options");
    options.add_options()("help", "Produce help message");
    
    po::options_description required("Required");
    required.add_options()
        ("length,l", po::value<int>(), "Matrix length")
        ("mean,m", po::value<float>(), "Error mean (%)");
    
    po::options_description optional("Optional");
    optional.add_options()
        ("file-name,f", po::value<string>()->default_value("matrix.txt"), "Matrix file name")
        ("seed,s", po::value<unsigned>(), "Random generation seed");
    
    options.add(required).add(optional);
    
    
    //load options
    try {
        po::store(po::parse_command_line(argc, argv, options), optionMap);
    } catch (exception &e) {
        opState = OPS_ERR;
        cerr << "Incorrect input: " << e.what() << endl << endl;
        return;
    }
    
    //print help
    if (optionMap.count("help")) {
        opState = OPS_HELP;
        cout << options << endl;
        return;
    }
    
    checkOptionValidity();
}

unsigned MatrGenOptions::getMatrixLength() {
    return optionMap["length"].as<int>();
}

float MatrGenOptions::getMatrixErrorMean() {
    return optionMap["mean"].as<float>();
}

string MatrGenOptions::getFileName() {
    return optionMap["file-name"].as<string>();
}

Optional<unsigned> MatrGenOptions::seed() {
    if (!optionMap.count("seed")) return Opt::NoValue;
    
    return optionMap["seed"].as<unsigned>();
}

OptionsState MatrGenOptions::optionsState() {
    return opState;
}


//PRIVATE
void MatrGenOptions::checkOptionValidity() {
    
    //check if options are specified
    checkForExistence("length", "Matrix length must be specified");
    if(opState != OPS_OK) return;
    
    checkForExistence("mean", "Error mean must be specified");
    if(opState != OPS_OK) return;
    
    
    //check values
    if (optionMap["length"].as<int>() < 0) {
        setOptionError("Matrix length must be non-negative value");
        return;
    }
    
    if (optionMap["mean"].as<float>() < 0) {
        setOptionError("Error mean must be non-negative value");
        return;
    }
}

void MatrGenOptions::checkForExistence(const char * option, const char * errOutput) {
    if (!optionMap.count(option)) {
        setOptionError(errOutput);
        return;
    }
}

void MatrGenOptions::setOptionError(const char * message) {
    cerr << message << endl << endl;
    opState = OPS_ERR;
}