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
BENCHMARKS = \
	fft-bench1-small.c \
	fft-bench2-small.c \
	fft-bench1.c \
	fft-bench2.c

BENCHMARK_INPUTS = \
	fft-bench1-small.inputs \
	fft-bench2-small.inputs \
	fft-bench1.inputs \
	fft-bench2.inputs \
	fft.d1 fft.d2 fft.d8 fft.d256 # fft.d1024 fft.d4096 fft.d16384 fft.d65536

EXTRA_DIST = \
	$(BENCHMARKS) \
	$(BENCHMARK_INPUTS) \
	fft-bench.c fft_impl1.c fft_impl2.c fft.h fft_extra.c

include $(top_srcdir)/build-aux/benchmarks.mk

#####  Special targets for the FFT demo #####

dist_noinst_SCRIPTS = gen_table_2.sh

fft-bench1.x: fft-bench.c fft_impl1.c fft_extra.c fft.h fft_table1_8_data.h
fft-bench2.x: fft-bench.c fft_impl2.c fft_extra.c fft.h fft_table2_8_data.h
fft-bench1-small.x: fft-bench.c fft_impl1.c fft.h fft_table1_8_data.h
fft-bench2-small.x: fft-bench.c fft_impl2.c fft.h fft_table2_8_data.h

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

DISTCLEANFILES += fft_table*_data.h

