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
	mandel.c \
	mandel_colors.c

BENCHMARK_INPUTS = \
	mandel.d1 \
	mandel.d2

EXTRA_DIST = \
	$(BENCHMARKS) \
	$(BENCHMARK_INPUTS)

include $(top_srcdir)/build-aux/benchmarks.mk
