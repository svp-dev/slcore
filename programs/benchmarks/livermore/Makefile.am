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

BENCHMARKS = bench1.c bench2.c bench3.c bench4.c bench5.c bench6.c	\
  bench7.c bench8.c bench9.c bench10.c bench11.c bench12.c bench15.c	\
  bench19.c bench21.c bench22.c bench23.c
BENCHMARK_INPUTS = $(BENCHMARKS:.c=.inputs)

KERNELS = kernel1.c kernel2.c kernel3.c kernel4.c kernel5.c kernel6.c	\
  kernel7.c kernel8.c kernel9.c kernel10.c kernel11.c kernel12.c	\
  kernel15.c kernel19.c kernel21.c kernel22.c kernel23.c


EXTRA_DIST = $(KERNELS) $(BENCHMARKS) $(BENCHMARK_INPUTS)

include extradist.mk
include $(top_srcdir)/build-aux/benchmarks.mk

