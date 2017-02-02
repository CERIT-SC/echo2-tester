include ../Makefile.inc

SRC= main.cpp SamLoader.cpp SamToMapOptions.cpp Fasta.cpp
OBJ=${SRC:.cpp=.o}

CPPFLAGS=-I../global

LIB=-lboost_program_options

samtomap: ${OBJ}
	${CXX} -o $@ ${OBJ} ${LIB}

clean:
	rm -f ${OBJ} samtomap
