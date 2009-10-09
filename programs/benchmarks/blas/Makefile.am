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
	bench-daxpy.c \
	bench-saxpy.c

BENCHMARK_INPUTS = \
	bench-daxpy.inputs \
	bench-saxpy.inputs \
	axy.d1 \
	axy.d2 \
	axy.d3 

EXTRA_DIST = \
	templates/xaxpy.c \
	templates/xaxpyc.c \
	templates/xdot.c \
	templates/xdotc.c \
	blas_complex.h \
	caxpy.c \
	cdotc.c \
	cdotu.c \
	daxpy.c \
	ddot.c \
	saxpy.c \
	sdot.c \
	zaxpy.c \
	zdotc.c \
	zdotu.c \
	$(BENCHMARKS) \
	$(BENCHMARKS_INPUTS)


include $(top_srcdir)/build-aux/benchmarks.mk
