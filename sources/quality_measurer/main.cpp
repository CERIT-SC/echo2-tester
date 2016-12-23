//
//  main.cpp
//  quality_measurer
//
//  Created by Miloš Šimek on 10.03.15.
//  Copyright (c) 2015 Miloš Šimek. All rights reserved.
//

#include <fstream>
#include <exception>
#include <sstream>
#include <iomanip>

#include "QMeasOptions.hpp"
#include "Measuring.hpp"
#include "../global/Optional.hpp"
#include "../global/Fasta.hpp"
#include "Setup.hpp"
using namespace std;

//function declarations
Fasta loadGenome(string genomeFileName);
void  checkFiles(ifstream& corruptedSeqFile, ifstream& correctedSeqFile, ifstream& mapFile);
string getStatistics(MeasuredData data);


int main(int argc, const char * argv[]) {
    
    //load options
    QMeasOptions options(argc, argv);
    if(options.optionsState() == OPS_HELP) return 0;
    if(options.optionsState() == OPS_ERR)  return 1;
    
    //opening/loading input files
    cout << "Loading files" << endl;
    Fasta genome = loadGenome(options.getGenomeFName());
    ifstream corruptedSeqFile(options.getCorruptedSeqFName());
    ifstream correctedSeqFile(options.getCorrectedSeqFName());
    ifstream mapFile(options.getSeqMapFName());
    checkFiles(corruptedSeqFile, correctedSeqFile, mapFile);
    
    //measure
    cout << "Measuring" << endl;
    MeasuredData data = measure(corruptedSeqFile, correctedSeqFile, mapFile, genome);
    
    //create result
    ostringstream resultStream;
    resultStream << "Genome fragment count: " << genome.getFragmentCount() << endl;
    resultStream << "Number of sequences: " << data.seqCount << endl;
    resultStream << endl;
    resultStream << getStatistics(data) << endl;
    
    //output result to file
    if (Optional<string> outputFileName = options.getOutputFile()) {
        ofstream file(*outputFileName);
        if(!file.good()) {
            cerr << "Cannot write to ouptut file" << endl << endl;
            return 1;
        }
        file << resultStream.str();
    }
    
    //output result to console
    cout << endl;
    cout << resultStream.str();
    cout << "Done" << endl;
    cout << endl;
    
    return 0;
}

Fasta loadGenome(string genomeFileName) {
    Fasta genome;
    try {
        genome.loadFromFile(genomeFileName);
        
    } catch (FastaException& e) {
        if (string(e.what()) == string("cannot-open-file")) {
            cerr << "Cannot open genome file" << endl;
            cerr << "For help, run with --help" << endl;
            cerr << endl;
            exit(1);
        } else {
            cerr << "Problem with reading genome data" << endl;
            cerr << "For help, run with --help" << endl;
            cerr << endl;
            exit(1);
        }
    }
    
    //check if genome has only allowed characters
    for (ULL fragIndex = 0; fragIndex < genome.getFragmentCount(); fragIndex++) {
        string& fragment = genome.getData(fragIndex);
        
        for (char ch: fragment) {
            if (count(allowedChar.begin(), allowedChar.end(), ch) == 0) {
                cerr << "Bad data in genome file" << endl;
                cerr << "For help, run with --help" << endl;
                cerr << endl;
                exit(1);
            }
        }
    }
    
    return genome;
}

void checkFiles(ifstream& corruptedSeqFile, ifstream& correctedSeqFile, ifstream& mapFile) {
    if (!corruptedSeqFile.is_open()) {
        cerr << "Cannot open file with corrupted sequences" << endl;
        cerr << "For help, run with --help" << endl;
        cerr << endl;
        exit(1);
    }
    
    if (!correctedSeqFile.is_open()) {
        cerr << "Cannot open file with corrected sequences" << endl;
        cerr << "For help, run with --help" << endl;
        cerr << endl;
        exit(1);
    }
    
    if (!mapFile.is_open()) {
        cerr << "Cannot open mapping file" << endl;
        cerr << "For help, run with --help" << endl;
        cerr << endl;
        exit(1);
    }
}

string getStatistics(MeasuredData data) {
    
    ostringstream os;
    os << "Statistics:" << endl;
    os << "True negatives:  " << std::setw(10) << data.counter[BS_TrueNegative] << endl;
    os << "True positives:  " << std::setw(10) << data.counter[BS_TruePositive] << endl;
    os << "False negatives: " << std::setw(10) << data.counter[BS_FalseNegative] << endl;
    os << "False positives: " << std::setw(10) << data.counter[BS_FalsePositive] << endl;
    os << endl;
    
    ULL baseCount = 0;
    for (int i=0; i < 4; i++) baseCount += data.counter[i];
    
    //print original error count
    double percentage = static_cast<double>(data.originalErrors)/static_cast<double>(baseCount);
    percentage *= 100;
    os << "Original error count: " << data.originalErrors << " - ";
    os << percentage << "%" << endl;
    
    //print errors left
    ULL errorsLeft = data.counter[BS_FalseNegative] + data.counter[BS_FalsePositive];

    percentage = static_cast<double>(errorsLeft)/static_cast<double>(baseCount);
    percentage *= 100;
    os << "Errors left: " << errorsLeft << " - ";
    os << percentage << "%" << endl;
    
    //print relative gain
    double relativeGain =
        static_cast<double>(data.counter[BS_TruePositive] - data.counter[BS_FalsePositive]) /
	static_cast<double>(data.counter[BS_TruePositive] + data.counter[BS_FalseNegative]);
    os << "Relative gain: " << relativeGain << endl;
    
    //print absolute gain
    double absoluteGain = static_cast<double>(data.originalErrors)/static_cast<double>(errorsLeft);
    os << "Absolute gain: " << absoluteGain << endl;
    
    //print unaltered sequences
    percentage = static_cast<double>(data.unalteredSeqCount)/static_cast<double>(data.seqCount);
    percentage *= 100;
    os << "Unaltered sequences: " << data.unalteredSeqCount << " - ";
    os << percentage << "%" << endl;
    
    return os.str();
}

