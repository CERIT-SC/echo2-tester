//
//  SeqGenOptions.cpp
//  testing system
//
//  Created by Miloš Šimek on 14.04.15.
//  Copyright (c) 2015 Miloš Šimek. All rights reserved.
//

#include "SeqGenOptions.hpp"

SeqGenOptions::SeqGenOptions(int argc, const char * argv[]) {
    //define parameters
    po::options_description requiredOptions("Required options");
    requiredOptions.add_options()
    ("genome,g", po::value<string>()->default_value("genome.fa"),
     "Specifies genome file")
    ("coverage,c", po::value<float>(), "Sequence to genome coverage")
    ("seq-len,l", po::value<int>(), "Sequence length");
    
    po::options_description probabilityOptions("Probability specification (exactly one is required)");
    probabilityOptions.add_options()
    ("prob-uniform,u", po::value<float>(), "Uniform error probability (in %, can be decimal)")
    ("prob-file,f", po::value<string>(),
     "File with 4x4xN probability matrix with probabilities for every base in sequence.\n"
     "    \tThere must be at least one 4x4 matrix specified in a file. Last 4x4 matrix from "
     "file will be used for all remaining base positions.");
    
    po::options_description other("Other (optional)");
    other.add_options()
    ("seq-file,q", po::value<string>()->default_value("corrupted.fastq"),
        "Output sequence file")
    ("map-file,m", po::value<string>()->default_value("seq-map.map"),
        "Sequence mapping file")
    ("seed,s", po::value<unsigned>(), "Sets seed for pseudo-random generation");
    
    po::options_description allOptions("Options");
    allOptions.add_options() ("help", "Produce help message");
    allOptions.add(requiredOptions).add(probabilityOptions).add(other);
    
    //load options
    try {
        po::store(po::parse_command_line(argc, argv, allOptions), optionMap);
    } catch (exception &e) {
        setOptionError(string("Incorrect input: ") + e.what());
        return;
    }
    
    //print help
    if (optionMap.count("help")) {
        opState = OPS_HELP;
        
        cout << endl;
        cout << "Sequence Generator" << endl;
        cout << "Version: " << VERSION_STRING << endl;
        cout << "This tool randomly generates seuences from genome file (-g)." << endl;
        cout << "Genome must be in fasta format. Amount of sequences generated" << endl;
        cout << "is specified by coverage (-c). I.e. coverage 2.5 means, that there" << endl;
        cout << "are enough sequences to cover the genome two and a half times." << endl;
        cout << "Sequences have length -l." << endl;
        cout << "Tool introduces errors to sequences using pseudo-random generator." << endl;
        cout << "How errors should be introduced must be specified either using -u or -f." << endl;
        cout << "There are 2 files as a result: sequences with errors in fastq" << endl;
        cout << "format and mapping file that maps sequences to their original" << endl;
        cout << "positions in genome." << endl;
        cout << endl;
        cout << allOptions << endl;
        return;
    }
    
    checkOptionValidity();
}

string SeqGenOptions::getGenomeFileName() {
    return optionMap["genome"].as<string>();
}

float SeqGenOptions::getCoverage() {
    return optionMap["coverage"].as<float>();
}

unsigned SeqGenOptions::getSeqLength() {
    return optionMap["seq-len"].as<int>();
}

string SeqGenOptions::getOutputSequenceFileName() {
    return optionMap["seq-file"].as<string>();
}

string SeqGenOptions::getOutputMapFileName() {
    return optionMap["map-file"].as<string>();
}

Optional<unsigned> SeqGenOptions::randGenSeed() {
    if (optionMap.count("seed")) return optionMap["seed"].as<unsigned>();
    else return Opt::NoValue;
}

Optional<float> SeqGenOptions::uniformProbability() {
    if (optionMap.count("prob-uniform")) {
        return optionMap["prob-uniform"].as<float>();
    }
    return Opt::NoValue;
}

Optional<string> SeqGenOptions::probabilityFileName() {
    if (optionMap.count("prob-file")) {
        return optionMap["prob-file"].as<string>();
    }
    return Opt::NoValue;
}


OptionsState SeqGenOptions::optionsState() {
    return opState;
}


//PRIVATE
void SeqGenOptions::checkOptionValidity() {

    //check if everything is specified
    checkForExistence("coverage", "Coverage must be specified");
    if(opState != OPS_OK) return;
    
    checkForExistence("seq-len", "Sequence length must be specified");
    if(opState != OPS_OK) return;
    
    if (optionMap.count("prob-uniform") + optionMap.count("prob-file") != 1) {
        
        setOptionError("Exactly one probability specification must be specified");
        return;
    }
    
    //check values
    if (optionMap["genome"].as<string>() == "") {
        setOptionError("Genome file name cannot be empty");
        return;
    }
        
    if (optionMap["coverage"].as<float>() < 0) {
        setOptionError("Coverage must be non-negative");
        return;
    }
        
    if (optionMap["seq-len"].as<int>() < 0) {
        setOptionError("Sequence length must be non-negative");
        return;
    }
    
    if (optionMap["seq-file"].as<string>() == "") {
        setOptionError("Sequence file name cannot be empty");
        return;
    }
    
    if (optionMap["map-file"].as<string>() == "") {
        setOptionError("Map file name cannot be empty");
        return;
    }
    
    if (optionMap.count("prob-uniform")) {
        float value = optionMap["prob-uniform"].as<float>();
        if (value < 0.0 || value > 100.0) {
            setOptionError("Uniform error probability has incorrect value");
            return;
        }
    }
    
    if (optionMap.count("prob-file") &&
        optionMap["prob-file"].as<string>() == "") {
        
        setOptionError("Probability file name cannot be empty");
        return;
    }
}

void SeqGenOptions::checkForExistence(const char * option, const char * errOutput) {
    if (!optionMap.count(option)) {
        setOptionError(errOutput);
        return;
    }
}

void SeqGenOptions::setOptionError(string message) {
    cerr << message << endl;
    cerr << "For help, run with --help" << endl;
    cerr << endl;
    opState = OPS_ERR;
}
