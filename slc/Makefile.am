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

SUBDIRS = tools lib/mgos/tlsmalloc . build-aux doc
DIST_SUBDIRS = $(SUBDIRS) lib

README: $(srcdir)/doc/README.rst
	$(AM_V_at)rm -f $@
	$(AM_V_GEN)cp $(srcdir)/doc/README.rst $@
	$(AM_V_at)chmod -w $@

NEWS: $(srcdir)/doc/NEWS.rst
	$(AM_V_at)rm -f $@
	$(AM_V_GEN)cp $(srcdir)/doc/NEWS.rst $@
	$(AM_V_at)chmod -w $@

include $(srcdir)/build-aux/slcvars.mk
include $(srcdir)/build-aux/version.mk

show-vars:
	@echo
	@echo ": Set the following to work from the source directory;"
	@echo
	@for n in $(SLC_VARS); do \
	   echo "export $$n;"; \
	done
	@echo "export PATH=$(abs_top_builddir)/tools/bin:$$PATH;"


ACLOCAL_AMFLAGS = -I build-aux

noinst_DATA = 
include tools/lib.mk

