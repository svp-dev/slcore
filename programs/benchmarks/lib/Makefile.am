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

include $(top_srcdir)/build-aux/sl.mk

EXTRA_DIST = \
	benchmark.c \
	benchmark.h

CLEANFILES =
noinst_DATA =
noinst_LIBRARIES =


noinst_LIBRARIES += host-host-seqc/libbench.a

host_host_seqc_libbench_a_SOURCES =
host_host_seqc_libbench_a_LIBADD = \
	host-host-seqc/benchmark.o

host-host-seqc/%.o: $(srcdir)/%.c
	$(AM_V_at)$(MKDIR_P) host-host-seqc
	$(slc_verbose)$(SLC_RUN) -b seqc -c -o $@ $<

CLEANFILES += host-host-seqc/benchmark.o


if ENABLE_SLC_PTL
noinst_LIBRARIES += host-host-ptl/libbench.a

host_host_ptl_libbench_a_SOURCES =
host_host_ptl_libbench_a_LIBADD = \
	host-host-ptl/benchmark.o

host-host-ptl/%.o: $(srcdir)/%.c
	$(AM_V_at)$(MKDIR_P) host-host-ptl
	$(slc_verbose)$(SLC_RUN) -b ptl -c -o $@ $<

CLEANFILES += host-host-ptl/benchmark.o
endif

if ENABLE_SLC_MTALPHA
noinst_DATA += mtalpha-sim/libbench.a

mtalpha_sim_libbench_a_CONTENTS = \
    mtalpha-sim/benchmark.o

mtalpha-sim/%.o: $(srcdir)/%.c
	$(AM_V_at)$(MKDIR_P) mtalpha-sim
	$(slc_verbose)$(SLC_RUN) -b ppp-mtalpha -c -o $@ $<

mtalpha-sim/libbench.a: $(mtalpha_sim_libbench_a_CONTENTS)
	$(AM_V_at)rm -f $@
	$(AM_V_AR)$(AR_MTALPHA) cru $@ $^
	$(AM_V_at)$(RANLIB_MTALPHA) $@

CLEANFILES += mtalpha-sim/benchmark.o
endif

