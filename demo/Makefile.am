# 
# slc, a SL compiler.
# 
# Copyright (C) 2008,2009 The SL project..
# All rights reserved.
# 

DEMOS = 10threads.sl answer.sl fibonacci.sl helloworld.sl roman.sl # test.sl
EXTRA_DIST = $(DEMOS)

TESTS = $(DEMOS)
include $(top_srcdir)/tests.mk

noinst_DATA = $(DEMOS:.sl=.mtalpha-x) $(DEMOS:.sl=.ptl-x) $(DEMOS:.sl=.seqc-x)
CLEANFILES = $(noinst_DATA)

SUFFIXES = .sl .mtalpha-x

.sl.mtalpha-x:
	$(SLC) -b ppp-mtalpha -o $@ $<

.sl.ptl-x:
	$(SLC) -b ptl -o $@ $<

.sl.seqc-x:
	$(SLC) -b seqc -o $@ $<

