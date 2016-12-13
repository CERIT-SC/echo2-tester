include ../Makefile.inc

OBJ=main.o MatrGenOptions.o N44Matrix.o

matrix_generator: ${OBJ}
	${CXX} -o $@ ${OBJ} ${BOOST}

clean:
	rm -f ${OBJ} matrix_generator
