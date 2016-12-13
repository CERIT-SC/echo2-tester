include ../Makefile.inc

SRC=FileLoaders.cpp main.cpp Measuring.cpp QMeasOptions.cpp
OBJ=${SRC:.cpp=.o}

CPPFLAGS=-I../global

LIB=-lboost_program_options

quality_measurer: ${OBJ}
	${CXX} -o $@ ${OBJ} ${LIB}

clean:
	rm -f ${OBJ} quality_measurer
