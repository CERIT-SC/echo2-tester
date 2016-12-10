//
//  QMeasOptions.cpp
//  testing system
//
//  Created by Miloš Šimek on 30.04.15.
//  Copyright (c) 2015 Miloš Šimek. All rights reserved.
//

#include "QMeasOptions.hpp"

QMeasOptions::QMeasOptions(int argc, const char * argv[]) {
    //define parameters
    po::options_description options("Options");
    options.add_options()
    ("help", "Produce help message\n")
    ("genome,g",po::value<string>()->default_value("genome.txt"),
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
