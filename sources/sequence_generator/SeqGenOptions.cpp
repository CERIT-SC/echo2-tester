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
    ("seq-num,n", po::value<long long>(), "Number of sequences generated")
    ("seq-len,l", po::value<int>(), "Sequence length");
    
    //definovat další objekt description pro pravděpodobnost (vždy jen jedna je povolena)
    po::options_description probabilityOptions("Probability specification (exactly one is required)");
    probabilityOptions.add_options()
    ("prob-uniform,u", po::value<int>(), "Uniform error probability (in %)")
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
        cout << "This tool randomly generates seuences from genome file (-g)." << endl;
        cout << "Genome must be in fasta format. There will be -n sequences" << endl;
        cout << "generated of length -l. Tool introduces errors to sequences" << endl;
        cout << "using pseudo-random generator. How errors should be introduced" << endl;
        cout << "must be specified either using -u or -f." << endl;
        cout << "There are 2 files as a result: sequences with errors in fastq" << endl;
        cout << "format and mapping file that maps sequences to their original" << endl;
        cout << "positions in genome." << endl;
        cout << endl;
        cout << allOptions << endl;
        return;
    }
    
    checkOptionValidity();
}

string SeqGenOptions::getGenomeFilePath() {
    return optionMap["genome"].as<string>();
}

ULL SeqGenOptions::getSeqNum() {
    return optionMap["seq-num"].as<long long>();
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

Optional<unsigned> SeqGenOptions::uniformProbability() {
    if (optionMap.count("prob-uniform")) {
        return optionMap["prob-uniform"].as<int>();
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
    checkForExistence("genome", "Genome file must be specified");
    if(opState != OPS_OK) return;
    
    checkForExistence("seq-num", "Number of sequences must be specified");
    if(opState != OPS_OK) return;
    
    checkForExistence("seq-len", "Sequence length must be specified");
    if(opState != OPS_OK) return;
    
    if (optionMap.count("prob-uniform") + optionMap.count("prob-file") != 1) {
        
        setOptionError("Exactly one probability specification must be specified");
        return;
    }
    
    //check values
    try {
        //file path is not checked
        
        if (optionMap["seq-num"].as<long long>() < 0) {
            setOptionError("Number of sequences must be non-negative");
            return;
        }
        
        if (optionMap["seq-len"].as<int>() < 0) {
            setOptionError("Sequence length must be non-negative");
            return;
        }
        
        
        if (optionMap.count("prob-uniform")) {
            int value = optionMap["prob-uniform"].as<int>();
            if (value < 0 || value > 100) {
                setOptionError("Uniform error probability has incorrect value");
                return;
            }
        }
        
    } catch (exception &e) {
        setOptionError((string("Incorrect value: ") + e.what()).c_str());
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
