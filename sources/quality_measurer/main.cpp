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
#include "QMeasOptions.h"
#include "Measuring.h"
#include "Optional.h"
using namespace std;

string loadGenome(string genomeFileName);
string getStatistics(MeasuredData data);

int main(int argc, const char * argv[]) {
    
    //load options
    QMeasOptions options(argc, argv);
    
    if(options.optionsState() == OPS_HELP) return 0;
    if(options.optionsState() == OPS_ERR)  return 1;
    
    //opening/loading input files
    string genome = loadGenome(options.getGenomeFName());
    ifstream corruptedSeqFile(options.getCorruptedSeqFName());
    ifstream correctedSeqFile(options.getCorrectedSeqFName());
    ifstream mapFile(options.getSeqMapFName());
    
    if (!corruptedSeqFile.is_open()) {
        cerr << "Cannot open file with corrupted sequences" << endl << endl;
        exit(1);
    }
    
    if (!correctedSeqFile.is_open()) {
        cerr << "Cannot open file with corrected sequences" << endl << endl;
        exit(1);
    }
    
    if (!mapFile.is_open()) {
        cerr << "Cannot open mapping file" << endl << endl;
        exit(1);
    }
    
    ostringstream resultStream;
    resultStream << "Genome length: " << genome.size() << endl;
    
    MeasuredData data = measure(genome, corruptedSeqFile, correctedSeqFile, mapFile);
    
    resultStream << "Number of sequences: " << data.seqCount << endl << endl;

    resultStream << getStatistics(data) << endl;
    
    //output result
    if (Optional<string> outputFileName = options.getOutputFile()) {
        ofstream file(*outputFileName);
        if(!file.good()) {
            cerr << "Cannot write to ouptut file" << endl << endl;
            return 1;
        }
        file << resultStream.str();
    } else {
        cout << resultStream.str();
    }
    
    return 0;
}

string loadGenome(string genomeFileName) {
    ifstream genomeFile(genomeFileName);
    string genome;
    
    if (!genomeFile.is_open()) {
        cerr << "Cannot open genome file" << endl << endl;
        exit(1);
    }
    
    genomeFile >> genome;
    
    if (genomeFile.fail()) {
        cerr << "Cannot load genome from file" << endl << endl;
        exit(1);
    }
    
    transform(genome.begin(), genome.end(), genome.begin(), ::toupper);
    
    //check if genome has only A C G T characters
    for_each(genome.begin(), genome.end(), [&](char& c){
        if (c != 'A' && c != 'C' && c != 'G' && c != 'T') {
            cerr << "Bad data in genome file" << endl << endl;
            exit(1);
        }
    });
    
    return genome;
}

string getStatistics(MeasuredData data) {
    
    ostringstream os;
    os << "Statistics:" << endl;
    os << "True negatives:  " << std::setw(10) << data.counter[BS_TN] << endl;
    os << "True positives:  " << std::setw(10) << data.counter[BS_TP] << endl;
    os << "False negatives: " << std::setw(10) << data.counter[BS_FN] << endl;
    os << "False positives: " << std::setw(10) << data.counter[BS_FP] << endl;
    os << endl;
    
    ULL baseCount = 0;
    for (int i=0; i<BS_MAX; i++) baseCount += data.counter[i];
    
    //print original error count
    double percentage = static_cast<double>(data.originalErrors)/static_cast<double>(baseCount);
    percentage *= 100;
    os << "Original error count: " << data.originalErrors << " - ";
    os << percentage << "%" << endl;
    
    //print errors left
    ULL errorsLeft = data.counter[BS_FN] + data.counter[BS_FP];

    percentage = static_cast<double>(errorsLeft)/static_cast<double>(baseCount);
    percentage *= 100;
    os << "Errors left: " << errorsLeft << " - ";
    os << percentage << "%" << endl;
    
    //print gain
    double gain = 
        static_cast<double>(data.counter[BS_TP] - data.counter[BS_FP]) /
	static_cast<double>(data.counter[BS_TP] + data.counter[BS_FN]);
    os << "Gain: " << gain << endl;
    
    //print unaltered sequences
    percentage = static_cast<double>(data.unalteredSeqCount)/static_cast<double>(data.seqCount);
    percentage *= 100;
    os << "Unaltered sequences: " << data.unalteredSeqCount << " - ";
    os << percentage << "%" << endl;
    
    return os.str();
}

