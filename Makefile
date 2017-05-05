SUBDIR = genome_generator matrix_generator quality_measurer sequence_generator
prefix ?= /usr/local

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

.PHONY: install
install:
	mkdir -p $(prefix)/bin
	for d in $(SUBDIR); do cp sources/$$d/$$d $(prefix)/bin/; done

.PHONY: uninstall
uninstall:
	for d in $(SUBDIR); do rm $(prefix)/bin/$$d; done