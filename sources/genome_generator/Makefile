include ../Makefile.inc

OBJ=main.o DnaGenOptions.o

genome_generator: ${OBJ}
	${CXX} -o $@ ${OBJ} ${BOOST}

clean:
	rm -f ${OBJ} genome_generator
