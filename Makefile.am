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

EXTRA_DIST = deploy/Makefile-reqs.orig deploy/reqs.mk deploy/README deploy/slenv
SUBDIRS = . tools lib programs doc

README: $(srcdir)/doc/README.txt
	$(AM_V_at)rm -f $@
	$(AM_V_GEN)cp $(srcdir)/doc/README.txt $@
	$(AM_V_at)chmod -w $@

NEWS: $(srcdir)/doc/NEWS.txt
	$(AM_V_at)rm -f $@
	$(AM_V_GEN)cp $(srcdir)/doc/NEWS.txt $@
	$(AM_V_at)chmod -w $@




if MAINTAINER_MODE

find_sources = \
	find $(srcdir)/tools $(srcdir)/lib \
	  -name '*.h' \
	-or -name '*.c' \
	-or -name '*.sl' \
	-or -name '*.slh' \
	-or -name '*.cpp' \
	-or -name '*.cc' \
	-or -name '*.hh'

# Update copyright notice.
rehead:
	$(find_sources) | xargs perl $(srcdir)/reheader.pl

endif

include $(top_srcdir)/sl.mk
show-vars:
	@echo
	@echo ": Set the following to work from the source directory;"
	@echo
	@for n in $(SLC_VARS); do \
	   echo "export $$n;"; \
	done
	@echo "export PATH=$(abs_top_builddir)/tools/bin:$$PATH;"

ACLOCAL_AMFLAGS = -I m4
DISTCHECK_CONFIGURE_FLAGS = --enable-testsuite --disable-demos
