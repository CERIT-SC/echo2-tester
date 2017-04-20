SUBDIR = genome_generator matrix_generator quality_measurer sequence_generator samtomap

.PHONY: all
all:
	for d in $(SUBDIR); do $(MAKE) -C sources/$$d; done

.PHONY: clean
clean:
	for d in $(SUBDIR); do $(MAKE) -C sources/$$d clean; done

.PHONY: here
here:
	mkdir -p Build
	for d in $(SUBDIR); do cp sources/$$d/$$d Build/$$d; done