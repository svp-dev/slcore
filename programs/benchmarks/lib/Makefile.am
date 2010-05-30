## Makefile.am: this file is part of the SL toolchain.
## 
## Copyright (C) 2009,2010 The SL project
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

##
## Targets for `seqc'
##
noinst_LIBRARIES += seqc/libbench.a

seqc_libbench_a_SOURCES =
seqc_libbench_a_LIBADD = \
	seqc/benchmark.o

seqc/%.o: $(srcdir)/%.c
	$(AM_V_at)$(MKDIR_P) seqc
	$(slc_verbose)$(SLC_RUN) -g -b seqc -c -o $@ $<

CLEANFILES += seqc/benchmark.o

###
### Targets for `ptl'
###
if ENABLE_SLC_PTL
noinst_LIBRARIES += ptl/libbench.a

ptl_libbench_a_SOURCES =
ptl_libbench_a_LIBADD = \
	ptl/benchmark.o

ptl/%.o: $(srcdir)/%.c
	$(AM_V_at)$(MKDIR_P) ptl
	$(slc_verbose)$(SLC_RUN) -g -b ptl -c -o $@ $<

CLEANFILES += ptl/benchmark.o
endif

###
### Targets for `mtalpha'
###
if ENABLE_SLC_MTALPHA

noinst_DATA += mta/libbench.a
CLEANFILES += mta/libbench.a

mta_libbench_a_CONTENTS = \
    mta/benchmark.o

mta/%.o: $(srcdir)/%.c
	$(AM_V_at)$(MKDIR_P) mta
	$(slc_verbose)$(SLC_RUN) -b mta -c -o $@ $<

mta/libbench.a: $(mta_libbench_a_CONTENTS)
	$(AM_V_at)rm -f $@
	$(AM_V_AR)$(AR_MTALPHA) cru $@ $^
	$(AM_V_at)$(RANLIB_MTALPHA) $@

CLEANFILES += mta/benchmark.o

##

noinst_DATA += mta_n/libbench.a
CLEANFILES += mta_n/libbench.a

mta_n_libbench_a_CONTENTS = \
    mta_n/benchmark.o

mta_n/%.o: $(srcdir)/%.c
	$(AM_V_at)$(MKDIR_P) mta_n
	$(slc_verbose)$(SLC_RUN) -b mta_n -c -o $@ $<

mta_n/libbench.a: $(mta_n_libbench_a_CONTENTS)
	$(AM_V_at)rm -f $@
	$(AM_V_AR)$(AR_MTALPHA) cru $@ $^
	$(AM_V_at)$(RANLIB_MTALPHA) $@

CLEANFILES += mta_n/benchmark.o

##

noinst_DATA += mta_on/libbench.a
CLEANFILES += mta_on/libbench.a

mta_on_libbench_a_CONTENTS = \
    mta_on/benchmark.o

mta_on/%.o: $(srcdir)/%.c
	$(AM_V_at)$(MKDIR_P) mta_on
	$(slc_verbose)$(SLC_RUN) -b mta_on -c -o $@ $<

mta_on/libbench.a: $(mta_on_libbench_a_CONTENTS)
	$(AM_V_at)rm -f $@
	$(AM_V_AR)$(AR_MTALPHA) cru $@ $^
	$(AM_V_at)$(RANLIB_MTALPHA) $@

CLEANFILES += mta_on/benchmark.o

##

noinst_DATA += mta_s/libbench.a
CLEANFILES += mta_s/libbench.a

mta_s_libbench_a_CONTENTS = \
    mta_s/benchmark.o

mta_s/%.o: $(srcdir)/%.c
	$(AM_V_at)$(MKDIR_P) mta_s
	$(slc_verbose)$(SLC_RUN) -b mta_s -c -o $@ $<

mta_s/libbench.a: $(mta_s_libbench_a_CONTENTS)
	$(AM_V_at)rm -f $@
	$(AM_V_AR)$(AR_MTALPHA) cru $@ $^
	$(AM_V_at)$(RANLIB_MTALPHA) $@

CLEANFILES += mta_s/benchmark.o

endif

