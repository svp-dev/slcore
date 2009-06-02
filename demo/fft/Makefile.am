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
DEMOS = fft_bench1.sl fft_bench2.sl fft_test1.sl fft_test2.sl
TESTS = $(DEMOS)

include $(top_srcdir)/tests.mk

EXTRA_DIST = $(DEMOS) fft_bench.sl fft_test.sl fft_impl1.sl fft_impl2.sl fft.slh fft_extra.sl

# Also build demos during "make all":
noinst_DATA = $(DEMOS:.sl=.x)
CLEANFILES = $(DEMOS:.sl=.x*)

#####  Special targets for the FFT demo #####

dist_noinst_SCRIPTS = testfft.sh gen_table_2.sh

noinst_PROGRAMS = gen_table_1
gen_table_1_SOURCES = gen_table_1.c
gen_table_1_CFLAGS = -g -O0 -ffloat-store
gen_table_1_LDFLAGS = -lm

BUILT_SOURCES = fft_table1.h fft_table2.h
dist_noinst_DATA = $(BUILT_SOURCES)

fft_bench1.x: fft_bench.sl fft_impl1.sl fft.slh fft_table1.h
fft_bench2.x: fft_bench.sl fft_impl2.sl fft.slh fft_table2.h
fft_test1.x: fft_test.sl fft_impl1.sl fft_extra.sl fft.slh fft_table1.h
fft_test2.x: fft_test.sl fft_impl2.sl fft_extra.sl fft.slh fft_table2.h

if MAINTAINER_MODE

fft_table2.h: $(srcdir)/gen_table_2.sh
	$(AM_V_at) rm -f "$@"
	$(AM_V_GEN) $(SHELL) $(srcdir)/gen_table_2.sh >"$@"
	$(AM_V_at) chmod -w "$@"

fft_table1.h: gen_table_1$(EXEEXT)
	$(AM_V_at) rm -f "$@"
	$(AM_V_GEN) ./gen_table_1$(EXEEXT) >"$@"
	$(AM_V_at) chmod -w "$@"

endif
