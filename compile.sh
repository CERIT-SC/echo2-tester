#!/bin/bash

mkdir Build 2> /dev/null

# genome_generator
echo ""
echo "Compiling genome_generator"
cd sources/genome_generator

g++ -std=c++11 -O3 -m64 *.h *.cpp ../global/*.h ../global/*.cpp -I/usr/local/include -L/usr/local/lib -lboost_program_options

rm *.gch ../global/*.gch

cd ../..

mv sources/genome_generator/a.out Build/genome_generator



# matrix_generator
echo ""
echo "Compiling matrix_generator"
cd sources/matrix_generator

g++ -std=c++11 -O3 -m64 *.h *.cpp ../global/*.h ../global/*.cpp -I/usr/local/include -L/usr/local/lib -lboost_program_options

rm *.gch ../global/*.gch

cd ../..

mv sources/matrix_generator/a.out Build/matrix_generator


# quality_measurer
echo ""
echo "Compiling quality_measurer"
cd sources/quality_measurer

g++ -std=c++11 -O3 -m64 *.h *.cpp ../global/*.h ../global/*.cpp -I/usr/local/include -L/usr/local/lib -lboost_program_options

rm *.gch ../global/*.gch

cd ../..

mv sources/quality_measurer/a.out Build/quality_measurer


# sequence_generator
echo ""
echo "Compiling sequence_generator"
cd sources/sequence_generator

g++ -std=c++11 -O3 -m64 *.h *.cpp ../global/*.h ../global/*.cpp -I/usr/local/include -L/usr/local/lib -lboost_program_options

rm *.gch ../global/*.gch

cd ../..

mv sources/sequence_generator/a.out Build/sequence_generator

echo ""
echo "Done"
echo ""

