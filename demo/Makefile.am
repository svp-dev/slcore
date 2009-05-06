# 
# slc, a SL compiler.
# 
# Copyright (C) 2008,2009 The SL project..
# All rights reserved.
# 

DEMOS = 10threads.sl answer.sl fibonacci.sl helloworld.sl roman.sl fft.sl
dist_noinst_SCRIPTS = testfft.sh
EXTRA_DEMOS = 
BUILT_SOURCES = fft_table.slh

ALL_DEMOS = $(DEMOS) $(EXTRA_DEMOS)
EXTRA_DIST = $(ALL_DEMOS) $(BUILT_SOURCES)

# Definitions for "make check":

TESTS = $(DEMOS)
include $(top_srcdir)/tests.mk

# Also build demos during "make all":

noinst_DATA = $(ALL_DEMOS:.sl=.ptl-x) $(ALL_DEMOS:.sl=.seqc-x)
if ENABLE_MTALPHA
noinst_DATA += $(ALL_DEMOS:.sl=.mtalpha-x)
endif

SUFFIXES = .sl .mtalpha-x .ptl-x .seqc-x

.sl.mtalpha-x:
	$(SLC) -b ppp-mtalpha -o $@ $< -mcpu=ev6 -mbwx -mmax -mfix -mcix

.sl.ptl-x:
	$(SLC) -b ptl -o $@ $<

.sl.seqc-x:
	$(SLC) -b seqc -o $@ $<

fft.ptl-x fft.seqc-x fft.mtalpha-x: fft.sl fft_table.slh

CLEANFILES = $(noinst_DATA) 

# Special targets for the FFT demo:
if MAINTAINER_MODE

noinst_PROGRAMS = fft_gentable
fft_gentable_SOURCES = fft_gentable.c
fft_gentable_CFLAGS = -O0 -ffloat-store
fft_gentable_LDFLAGS = -lm

fft_table.slh: fft_gentable$(EXEEXT)
	./fft_gentable$(EXEEXT) >$@

MAINTAINERCLEANFILES = $(BUILT_SOURCES)

endif
