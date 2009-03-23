# 
# slc, a SL compiler.
# 
# Copyright (C) 2008,2009 The SL project..
# All rights reserved.
# 

DEMOS = 10threads.sl answer.sl fibonacci.sl helloworld.sl roman.sl 
EXTRA_DEMOS = fft.sl
ALL_DEMOS = $(DEMOS) $(EXTRA_DEMOS)
EXTRA_DIST = $(ALL_DEMOS)

# Definitions for "make check":

TESTS = $(DEMOS)
include $(top_srcdir)/tests.mk

# Also build demos during "make all":

noinst_DATA = $(ALL_DEMOS:.sl=.mtalpha-x) $(ALL_DEMOS:.sl=.ptl-x) $(ALL_DEMOS:.sl=.seqc-x)

SUFFIXES = .sl .mtalpha-x

.sl.mtalpha-x:
	$(SLC) -b ppp-mtalpha -o $@ $<

.sl.ptl-x:
	$(SLC) -b ptl -o $@ $<

.sl.seqc-x:
	$(SLC) -b seqc -o $@ $<

# Special targets for the FFT demo:

noinst_PROGRAMS = fft_gentable
fft_gentable_SOURCES = fft_gentable.c
BUILT_SOURCES = fft_table.slh

fft_table.slh: fft_gentable$(EXEEXT)
	./fft_gentable$(EXEEXT) >$@

CLEANFILES = $(noinst_DATA) $(BUILT_SOURCES)

