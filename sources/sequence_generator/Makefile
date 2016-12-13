include ../Makefile.inc

OBJ=CorruptedSeqGenerator.o ProbMatrixGetter.o SeqGenOptions.o main.o N44Matrix.o

sequence_generator: ${OBJ}
	${CXX} -o $@ ${OBJ} ${BOOST}

clean:
	rm -f ${OBJ} sequence_generator
