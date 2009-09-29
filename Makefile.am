## Makefile.am: this file is part of the SL toolchain.
## 
## Copyright (C) 2009 The SL project
##
## This program is free software; you can redistribute it and/or
## modify it under the terms of the GNU General Public License
## as published by the Free Software Foundation; either version 2
## of the License, or (at your option) any later version.
##
## The complete GNU General Public Licence Notice can be found as the
## `COPYING' file in the root directory.
##

# 

EXTRA_DIST = deploy/Makefile \
	deploy/layout.mk.defaults \
	deploy/versions.mk.defaults \
	deploy/tools.mk.defaults \
	deploy/README \
	deploy/patches/alpha.h.patch \
	deploy/patches/alpha.c.patch \
	deploy/patches/alpha.md.patch \
	deploy/patches/constraints.md.patch \
	deploy/patches/m4.patch \
	deploy/rules/binutils.mk \
	deploy/rules/mggcc.mk \
	deploy/rules/gcc.mk \
	deploy/rules/mgsim.mk \
	deploy/rules/m4.mk \
	deploy/rules/sl.mk \
	deploy/slenv

SUBDIRS = . slc tests programs

# run `slt' on the console (eye candy, for more intensive CPU
# usage and faster results, use `make check')

check-slt:
	$(AM_V_at)cd slc && $(MAKE) $(AM_MAKEFLAGS) all
	$(AM_V_at)set -e; for d in `find . -type d \
	    -and -exec test -r '{}'/Makefile \; \
	    -and -exec test -r $(srcdir)/'{}'/Makefile.am \; \
	    -and -exec grep -q 'tests\.mk' '{}'/Makefile \; \
	    -and -print`; do \
	   (cd $$d && $(MAKE) $(AM_MAKEFLAGS) check-slt) \
	done

find_sources = \
	find $(srcdir)/slc $(srcdir)/lib $(srcdir)/programs \
	  -name '*.h' \
	-or -name '*.c' \
	-or -name '*.cpp' \
	-or -name '*.cc' \
	-or -name '*.hh' | \
	grep -v 'lib/src/malloc.c' 

# Update copyright notice.
rehead:
	$(find_sources) | xargs perl $(srcdir)/reheader.pl

include $(srcdir)/slc/build-aux/slcvars.mk

show-vars:
	@echo
	@echo ": Set the following to work from the source directory;"
	@echo
	@for n in $(SLC_VARS); do \
	   echo "export $$n;"; \
	done
	@echo "export PATH=$(abs_top_builddir)/slc/tools/bin:$$PATH;"

ACLOCAL_AMFLAGS = -I slc/build-aux
DISTCHECK_CONFIGURE_FLAGS = --enable-testsuite --disable-build-programs

