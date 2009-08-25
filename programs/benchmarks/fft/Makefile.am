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
DEMOS = fft_bench1.c fft_bench2.c fft_test1.c fft_test2.c
TESTS = fft_test1.c fft_test2.c

include $(top_srcdir)/tests.mk

EXTRA_DIST = $(DEMOS) fft_bench.c fft_test.c fft_impl1.c fft_impl2.c fft.h fft_extra.c

if ENABLE_DEMOS
noinst_DATA = $(DEMOS:.c=.x)
CLEANFILES = $(DEMOS:.c=.x) $(DEMOS:.c=.bin.*)
endif

#####  Special targets for the FFT demo #####

dist_noinst_SCRIPTS = testfft.sh gen_table_2.sh

BUILT_SOURCES = fft_table1_16_data.h fft_table1_8_data.h fft_table2_16_data.h fft_table2_8_data.h
DISTCLEANFILES = $(BUILT_SOURCES)

fft_bench1.x: fft_bench.c fft_impl1.c fft.h fft_table1_16_data.h
fft_bench2.x: fft_bench.c fft_impl2.c fft.h fft_table2_16_data.h
fft_test1.x: fft_test.c fft_impl1.c fft_extra.c fft.h fft_table1_8_data.h
fft_test2.x: fft_test.c fft_impl2.c fft_extra.c fft.h fft_table2_8_data.h

noinst_PROGRAMS = gen_table_1_16 gen_table_1_8
gen_table_1_16_SOURCES = gen_table_1.c
gen_table_1_16_CPPFLAGS = -DTABLE_SIZE=16
gen_table_1_16_CFLAGS = -O1 -fno-fast-math -ffloat-store
gen_table_1_16_LDFLAGS = -lm

gen_table_1_8_SOURCES = gen_table_1.c
gen_table_1_8_CPPFLAGS = -DTABLE_SIZE=8
gen_table_1_8_CFLAGS = -O1 -fno-fast-math -ffloat-store
gen_table_1_8_LDFLAGS = -lm

fft_table2_16_data.h: $(srcdir)/gen_table_2.sh
	$(AM_V_at) rm -f "$@"
	$(AM_V_GEN) TABLE_SIZE=16 $(SHELL) $(srcdir)/gen_table_2.sh >"$@"
	$(AM_V_at) chmod -w "$@"

fft_table2_8_data.h: $(srcdir)/gen_table_2.sh
	$(AM_V_at) rm -f "$@"
	$(AM_V_GEN) TABLE_SIZE=8 $(SHELL) $(srcdir)/gen_table_2.sh >"$@"
	$(AM_V_at) chmod -w "$@"

fft_table1_8_data.h: gen_table_1_8$(EXEEXT)
	$(AM_V_at) rm -f "$@"
	$(AM_V_GEN) ./gen_table_1_8$(EXEEXT) >"$@"
	$(AM_V_at) chmod -w "$@"

fft_table1_16_data.h: gen_table_1_16$(EXEEXT)
	$(AM_V_at) rm -f "$@"
	$(AM_V_GEN) ./gen_table_1_16$(EXEEXT) >"$@"
	$(AM_V_at) chmod -w "$@"


