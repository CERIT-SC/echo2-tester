//
//  QMeasOptions.cpp
//  testing system
//
//  Created by Miloš Šimek on 30.04.15.
//  Copyright (c) 2015 Miloš Šimek. All rights reserved.
//

#include "QMeasOptions.hpp"

QMeasOptions::QMeasOptions(int argc, const char * argv[]) {
    //define options
    po::options_description options("Options");
    options.add_options()
    ("help", "Produce help message\n")
    ("genome,g",po::value<string>()->default_value("genome.fa"),
        "Specifies genome file")
    ("corrupted,c", po::value<string>()->default_value("corrupted.fastq"),
        "Fastq file of corrupted sequences")
    ("corrected,r", po::value<string>()->default_value("corrected.fastq"),
        "Fastq file of corrected sequences")
    ("map,m",po::value<string>()->default_value("seq-map.map"),
        "Sequence mapping file");
    
    po::options_description optional("Optional");
    optional.add_options()
    ("file,f", po::value<string>(), "File for results");
    
    options.add(optional);
    
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
        cout << "Quality Measurer" << endl;
        cout << "Version: " << VERSION_STRING << endl;
        cout << "This tool takes genome file (in fasta format), corrupted and" << endl;
        cout << "corrected sequences (in fastq format) and sequence mapping file." << endl;
        cout << "Program produces statistics about quality of corrections." << endl;
        cout << "Not mapped sequences are ignored." << endl;
        cout << endl;
        cout << options << endl;
        return;
    }
    
    checkForOptionValidity();
}

OptionsState QMeasOptions::optionsState() {
    return opState;
}

string QMeasOptions::getGenomeFName() {
    return optionMap["genome"].as<string>();
}

string QMeasOptions::getCorruptedSeqFName() {
    return optionMap["corrupted"].as<string>();
}

string QMeasOptions::getCorrectedSeqFName() {
    return optionMap["corrected"].as<string>();
}

string QMeasOptions::getSeqMapFName() {
    return optionMap["map"].as<string>();
}

Optional<string> QMeasOptions::getOutputFile() {
    if (!optionMap.count("file")) {
        return Opt::NoValue;
    }
    return optionMap["file"].as<string>();
}

//private
void QMeasOptions::checkForOptionValidity() {
    //genome file
    if (optionMap["genome"].as<string>() == "") {
        setOptionError("Genome file name cannot be empty");
        return;
    }
    
    //corrupted sequences
    if (optionMap["corrupted"].as<string>() == "") {
        setOptionError("Corrupted sequences file name cannot be empty");
        return;
    }
    
    //corrected sequences
    if (optionMap["corrected"].as<string>() == "") {
        setOptionError("Corrected sequences file name cannot be empty");
        return;
    }
    
    //map file
    if (optionMap["map"].as<string>() == "") {
        setOptionError("Map file name cannot be empty");
        return;
    }
    
    //output file
    if (optionMap.count("file") && optionMap["file"].as<string>() == "") {
        setOptionError("Output file name cannot be empty");
        return;
    }
}

void QMeasOptions::setOptionError(string message) {
    cerr << message << endl;
    cerr << "For help, run with --help" << endl;
    cerr << endl;
    opState = OPS_ERR;
}
