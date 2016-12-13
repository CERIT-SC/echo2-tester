include ../Makefile.inc

genome_generator: main.o
	${CXX} -o $@ main.o ${BOOST}

clean:
	rm -f main.o genome_generator
