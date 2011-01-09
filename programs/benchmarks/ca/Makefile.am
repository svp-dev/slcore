## Makefile.am: this file is part of the SL toolchain.
## 
## Copyright (C) 2011 The SL project
##
## This program is free software; you can redistribute it and/or
## modify it under the terms of the GNU General Public License
## as published by the Free Software Foundation; either version 2
## of the License, or (at your option) any later version.
##
## The complete GNU General Public Licence Notice can be found as the
## `COPYING' file in the root directory.
##

BENCHMARKS = \
	benchca.c \
	benchca_gfx.c

BENCHMARK_INPUTS = \
	benchca.inputs \
	one.d9000 \
	lidka.d9000 \
	lidka.d37200 \
	lidka.d768800 \
	herschel.d9000 \
	rabbits.d9000 \
	rpent.d9000 \
	glider.d9000

EXTRA_DIST = \
	ca2d.c \
	ca2d.h \
	makerule2d.c \
	$(BENCHMARKS) \
	$(BENCHMARK_INPUTS)

include $(top_srcdir)/build-aux/benchmarks.mk
