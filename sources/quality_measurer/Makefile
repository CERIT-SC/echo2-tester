SRC=FileLoaders.cpp main.cpp Measuring.cpp QMeasOptions.cpp
OBJ=${SRC:.cpp=.o}

DEBUG=-O2

SRCDIR=.:../Global

CPPFLAGS=-I../Global
CXXFLAGS=-std=c++11 ${DEBUG}

LIB=-lboost_program_options

quality_measurer: ${OBJ}
	${CXX} -o $@ ${OBJ} ${LIB}

clean:
	rm -f ${OBJ} quality_measurer
