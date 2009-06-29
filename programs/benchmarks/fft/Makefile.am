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
TESTS = fft_test1.sl fft_test2.sl

include $(top_srcdir)/tests.mk

EXTRA_DIST = $(DEMOS) fft_bench.sl fft_test.sl fft_impl1.sl fft_impl2.sl fft.slh fft_extra.sl

if ENABLE_DEMOS
noinst_DATA = $(DEMOS:.sl=.x)
CLEANFILES = $(DEMOS:.sl=.x) $(DEMOS:.sl=.bin.*)
endif

#####  Special targets for the FFT demo #####

dist_noinst_SCRIPTS = testfft.sh gen_table_2.sh

noinst_PROGRAMS = 

BUILT_SOURCES = fft_table1_16.h fft_table1_8.h fft_table2_16.h fft_table2_8.h
dist_noinst_DATA = $(BUILT_SOURCES)

fft_bench1.x: fft_bench.sl fft_impl1.sl fft.slh fft_table1_16.h
fft_bench2.x: fft_bench.sl fft_impl2.sl fft.slh fft_table2_16.h
fft_test1.x: fft_test.sl fft_impl1.sl fft_extra.sl fft.slh fft_table1_8.h
fft_test2.x: fft_test.sl fft_impl2.sl fft_extra.sl fft.slh fft_table2_8.h

if MAINTAINER_MODE

noinst_PROGRAMS += gen_table_1_16 gen_table_1_8
gen_table_1_16_SOURCES = gen_table_1.c
gen_table_1_16_CPPFLAGS = -DTABLE_SIZE=16
gen_table_1_16_CFLAGS = -g -O0 -ffloat-store
gen_table_1_16_LDFLAGS = -lm

gen_table_1_8_SOURCES = gen_table_1.c
gen_table_1_8_CPPFLAGS = -DTABLE_SIZE=8
gen_table_1_8_CFLAGS = -g -O0 -ffloat-store
gen_table_1_8_LDFLAGS = -lm

fft_table2_16.h: $(srcdir)/gen_table_2.sh
	$(AM_V_at) rm -f "$@"
	$(AM_V_GEN) TABLE_SIZE=16 $(SHELL) $(srcdir)/gen_table_2.sh >"$@"
	$(AM_V_at) chmod -w "$@"

fft_table2_8.h: $(srcdir)/gen_table_2.sh
	$(AM_V_at) rm -f "$@"
	$(AM_V_GEN) TABLE_SIZE=8 $(SHELL) $(srcdir)/gen_table_2.sh >"$@"
	$(AM_V_at) chmod -w "$@"

fft_table1_8.h: gen_table_1_8$(EXEEXT)
	$(AM_V_at) rm -f "$@"
	$(AM_V_GEN) ./gen_table_1_8$(EXEEXT) >"$@"
	$(AM_V_at) chmod -w "$@"

fft_table1_16.h: gen_table_1_16$(EXEEXT)
	$(AM_V_at) rm -f "$@"
	$(AM_V_GEN) ./gen_table_1_16$(EXEEXT) >"$@"
	$(AM_V_at) chmod -w "$@"

endif
