LIBDIRS = lib/slc/t-ptl \
	  lib/slc/host-host \
	  lib/slc/div \
	  lib/slc/mtalpha-sim-proto

.PHONY: all clean doc test

all:
	@echo "Building libraries..."
	@set -e; \
	for d in $(LIBDIRS); do \
	   echo "=> $$d"; \
	   (cd $$d && make);  \
	done
	@echo "Done."
	@echo
	@echo "NOTE:"
	@echo "  - Ensure that the env. var MGSIM is set."
	@echo "    (to the source directory of the simulator)"
	@echo
	@echo "  - Ensure that CC_ALPHA is set or alpha-linux-gnu-gcc is in the PATH."
	@echo
	@echo "  - Ensure that UTCC_MTALPHA is set or mtalpha-linux-gnu-gcc is in the PATH."
	@echo
	@echo "  - Run 'make doc' to generate the man pages (if needed)."
	@echo
	@echo "Ready to use: slc"
	@echo "Use --help for more details on each script."

clean:
	@echo "Cleaning libraries..."
	@set -e; \
	for d in $(LIBDIRS); do \
	   echo "=> $$d"; \
	   (cd $$d && make clean);  \
	done
	@echo "Done."


doc:
	cd doc && make
	help2man -N bin/slc >doc/slc.1

test:
	cd tests && ../test-all

