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
	bench-drot.c 	bench-srot.c \
	bench-ddot.c 	bench-sdot.c \
	bench-dasum.c	bench-sasum.c \
	bench-dnrm2.c	bench-snrm2.c \
	bench-dscal.c	bench-sscal.c \
	bench-dswap.c	bench-sswap.c \
	bench-dcopy.c	bench-scopy.c \
	bench-idamax.c	bench-isamax.c \
	bench-daxpy.c 	bench-saxpy.c

BENCHMARK_INPUTS = \
	bench-daxpy.inputs bench-saxpy.inputs \
	bench-dasum.inputs bench-sasum.inputs \
	bench-dscal.inputs bench-sscal.inputs \
	bench-ddot.inputs bench-sdot.inputs \
	bench-dswap.inputs bench-sswap.inputs \
	bench-dcopy.inputs bench-scopy.inputs \
	bench-drot.inputs bench-srot.inputs \
	bench-idamax.inputs bench-isamax.inputs \
	bench-dnrm2.inputs bench-snrm2.inputs 


EXTRA_DIST = \
	templates/xaxpy.c \
	templates/xaxpyc.c \
	templates/xdot.c \
	templates/xdotc.c \
	templates/xrot.c \
	templates/xasum.c \
	templates/xscal.c \
	templates/xnrm2.c \
	templates/xswap.c \
	templates/xcopy.c \
	templates/xiamax.c \
	templates/bench-xaxpy.c \
	templates/bench-xdot.c \
	templates/bench-xrot.c \
	templates/bench-xasum.c \
	templates/bench-xscal.c \
	templates/bench-xnrm2.c \
	templates/bench-xswap.c \
	templates/bench-xcopy.c \
	templates/bench-xiamax.c \
	blasbench.h \
	blas_complex.h \
	caxpy.c cdotc.c cdotu.c \
	zaxpy.c zdotc.c zdotu.c \
	dasum.c daxpy.c ddot.c drot.c dnrm2.c dscal.c idamax.c sswap.c scopy.c \
	sasum.c saxpy.c sdot.c srot.c snrm2.c sscal.c isamax.c dswap.c dcopy.c \
	$(BENCHMARKS) \
	$(BENCHMARK_INPUTS)

include extradist.mk
include $(top_srcdir)/build-aux/benchmarks.mk
