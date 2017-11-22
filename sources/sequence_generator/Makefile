include ../Makefile.inc

OBJ= main.o ProbMatrixLoader.o SeqGenOptions.o N44Matrix.o Fasta.o

sequence_generator: ${OBJ}
	${CXX} -o $@ ${OBJ} ${BOOST}

clean:
	rm -f ${OBJ} sequence_generator
