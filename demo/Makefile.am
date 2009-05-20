# 
# slc, a SL compiler.
# 
# Copyright (C) 2008,2009 The SL project..
# All rights reserved.
# 
DEMOS = 10threads.sl answer.sl fibonacci.sl helloworld.sl roman.sl fft.sl
TESTS = $(DEMOS)
include $(top_srcdir)/tests.mk

BUILT_SOURCES = 
EXTRA_DIST = $(DEMOS) $(BUILT_SOURCES)
MAINTAINERCLEANFILES = $(BUILT_SOURCES)

# Also build demos during "make all":
noinst_DATA = $(DEMOS:.sl=.ptl-x) $(DEMOS:.sl=.seqc-x)
if ENABLE_MTALPHA
noinst_DATA += $(DEMOS:.sl=.mtalpha-x)
endif
CLEANFILES = $(noinst_DATA)

# Definitions for "make check":

#####  Special targets for the FFT demo #####

dist_noinst_SCRIPTS = testfft.sh
BUILT_SOURCES += fft_table.slh

noinst_PROGRAMS = fft_gentable
fft_gentable_SOURCES = fft_gentable.c
fft_gentable_CFLAGS = -O0 -ffloat-store
fft_gentable_LDFLAGS = -lm

fft.ptl-x fft.seqc-x fft.mtalpha-x: fft.sl fft_table.slh

if MAINTAINER_MODE

fft_table.slh: fft_gentable$(EXEEXT)
	$(AM_V_at) rm -f $@
	$(AM_V_GEN) ./fft_gentable$(EXEEXT) >$@
	$(AM_V_at) chmod -w $@

endif
