//
//  SamToMapOptions.cpp
//  Corrector Tester
//
//  Created by Miloš Šimek on 18/01/2017.
//
//

#include "SamToMapOptions.hpp"

SamToMapOptions::SamToMapOptions(int argc, const char * argv[]) {
    //define options
    po::options_description options("Options");
    options.add_options()
    ("help", "Produce help message")
    ("sam,s", po::value<string>(), "Sam file")
    ("genome,g", po::value<string>(), "Genome file")
    ("map,m", po::value<string>()->default_value("seq-map.map"), "Map file");
    
    //parse options
    try {
        po::store(po::parse_command_line(argc, argv, options), optionMap);
    } catch (exception &e) {
        setOptionError(string("Incorrect input: ") + e.what());
        return;
    }
    po::notify(optionMap);
    
    //print help
    if (optionMap.count("help")) {
        opState = OPS_HELP;
        
        cout << endl;
        cout << "Sam to map converter" << endl;
        cout << "Version: " << VERSION_STRING << endl;
        cout << "This tool extracts sequence-to-genome mapping from sam file." << endl;
        cout << "Sam file (-s) and corresponding genome file (-g) are needed." << endl;
        cout << "Genome file must be in fasta format." << endl;
        cout << endl;
        cout << options << endl;
        return;
    }
    
    checkOptionValidity();
}

string SamToMapOptions::getSamFileName() {
    return optionMap["sam"].as<string>();
}

string SamToMapOptions::getGenomeFileName() {
    return optionMap["genome"].as<string>();
}

string SamToMapOptions::getMapFileName() {
    return optionMap["map"].as<string>();
}

OptionsState SamToMapOptions::optionsState() {
    return opState;
}

//PRIVATE
void SamToMapOptions::checkOptionValidity() {
    
    //check if everything is specified
    checkForExistence("sam", "Sam file must be specified");
    if (opState != OPS_OK) return;
    
    checkForExistence("genome", "Genome file must be specified");
    if (opState != OPS_OK) return;
    
    
    //check values
    if (optionMap["sam"].as<string>() == "") {
        setOptionError("Sam file name cannot be empty");
        return;
    }
    
    if (optionMap["genome"].as<string>() == "") {
        setOptionError("Genome file name cannot be empty");
        return;
    }
    
    if (optionMap["map"].as<string>() == "") {
        setOptionError("Map file name cannot be empty");
        return;
    }
    
}

void SamToMapOptions::checkForExistence(const char * option, const char * errOutput) {
    if (!optionMap.count(option)) {
        setOptionError(errOutput);
        return;
    }
}

void SamToMapOptions::setOptionError(string message) {
    cerr << message << endl;
    cerr << "For help, run with --help" << endl;
    cerr << endl;
    opState = OPS_ERR;
}
