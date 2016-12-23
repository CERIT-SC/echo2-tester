include ../Makefile.inc

SRC= main.cpp FileLoaders.cpp Measuring.cpp QMeasOptions.cpp Fasta.cpp
OBJ=${SRC:.cpp=.o}

CPPFLAGS=-I../global

LIB=-lboost_program_options

quality_measurer: ${OBJ}
	${CXX} -o $@ ${OBJ} ${LIB}

clean:
	rm -f ${OBJ} quality_measurer
