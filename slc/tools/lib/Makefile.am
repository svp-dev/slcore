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
dist_pkglib_SCRIPTS = \
	r-host.sh

pkglib_SCRIPTS = \
	r-mtalpha-sim.sh \
	r-mtsparc-sim.sh

r-mtalpha-sim.sh: r-mt-sim.sh.in Makefile
	$(AM_V_at)rm -f $@ $@.out
	$(AM_V_GEN)$(SED) <$< >$@.out \
	 -e 's|@MGSIM_ARCH[@]|$(MGSIM_ALPHA)|g' \
	 -e 's|@SIMX_ARCH[@]|$(SIMX_ALPHA)|g' \
	 -e 's|@ARCH[@]|ALPHA|g' \
	 -e 's|@arch[@]|alpha|g'
	$(AM_V_at)chmod +x,a-w $@.out
	$(AM_V_at)mv -f $@.out $@

r-mtsparc-sim.sh: r-mt-sim.sh.in Makefile
	$(AM_V_at)rm -f $@ $@.out
	$(AM_V_GEN)$(SED) <$< >$@.out \
	 -e 's|@MGSIM_ARCH[@]|$(MGSIM_SPARC)|g' \
	 -e 's|@SIMX_ARCH[@]|$(SIMX_SPARC)|g' \
	 -e 's|@ARCH[@]|SPARC|g' \
	 -e 's|@arch[@]|sparc|g'
	$(AM_V_at)chmod +x,a-w $@.out
	$(AM_V_at)mv -f $@.out $@

nobase_dist_pkgdata_DATA = \
	arggen.c \
	t-utc/slimpl.m4 \
	t-utc0/slimpl.m4 \
	t-utc0/include/sl_support.h 

include $(top_srcdir)/build-aux/sl.mk
nobase_pkglib_DATA =
nobase_pkglib_LIBRARIES = 
EXTRA_DIST = r-mt-sim.sh.in
CLEANFILES =
MAINTAINERCLEANFILES = 
BUILT_SOURCES = 

include $(srcdir)/seqc.mk
include $(srcdir)/ptl.mk
include $(srcdir)/mtalpha.mk
