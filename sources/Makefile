SUBDIR= genome_generator matrix_generator quality_measurer sequence_generator

all: 
	for d in ${SUBDIR}; do (cd $$d && ${MAKE}); done

clean: 
	for d in ${SUBDIR}; do (cd $$d && ${MAKE} clean); done
