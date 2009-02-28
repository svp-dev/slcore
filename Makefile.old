LIBDIRS = lib/slc/host-host-ptl/include \
	  lib/slc/host-host-ptl \
	  lib/slc/host-host-seqc \
	  lib/slc/div \
	  lib/slc/mtalpha-sim \
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
	find . -name '*~' -o -name '#*#' | xargs rm -f
	@echo "Cleaning libraries..."
	@set -e; \
	for d in $(LIBDIRS); do \
	   echo "=> $$d"; \
	   (cd $$d && make clean);  \
	done
	@echo "Done."


doc:
	help2man -N bin/cce >doc/cce.1
	help2man -N bin/sgr >doc/sgr.1
	help2man -N bin/slc >doc/slc.1
	cd doc && make

test:
	cd tests && ../test-all

