#!/bin/bash

mkdir Build 2> /dev/null

# genome_generator
echo ""
echo "Compiling genome_generator"
cd sources/genome_generator

g++ -std=c++11 -O3 -m64 *.cpp ../global/*.cpp -I/usr/local/include -L/usr/local/lib -lboost_program_options

cd ../..

mv sources/genome_generator/a.out Build/genome_generator



# matrix_generator
echo ""
echo "Compiling matrix_generator"
cd sources/matrix_generator

g++ -std=c++11 -O3 -m64 *.cpp ../global/*.cpp -I/usr/local/include -L/usr/local/lib -lboost_program_options

cd ../..

mv sources/matrix_generator/a.out Build/matrix_generator



# sequence_generator
echo ""
echo "Compiling sequence_generator"
cd sources/sequence_generator

g++ -std=c++11 -O3 -m64 *.cpp ../global/*.cpp -I/usr/local/include -L/usr/local/lib -lboost_program_options

cd ../..

mv sources/sequence_generator/a.out Build/sequence_generator



# quality_measurer
echo ""
echo "Compiling quality_measurer"
cd sources/quality_measurer

g++ -std=c++11 -O3 -m64 *.cpp ../global/*.cpp -I/usr/local/include -L/usr/local/lib -lboost_program_options

cd ../..

mv sources/quality_measurer/a.out Build/quality_measurer



# samtomap
echo ""
echo "Compiling samtomap"
cd sources/samtomap

g++ -std=c++11 -O3 -m64 *.cpp ../global/*.cpp -I/usr/local/include -L/usr/local/lib -lboost_program_options

cd ../..

mv sources/samtomap/a.out Build/samtomap



echo ""
echo "Done"
echo ""

