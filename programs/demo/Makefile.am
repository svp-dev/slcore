## Makefile.am: this file is part of the SL toolchain.
## 
## Copyright (C) 2009,2010 The SL project
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
DEMOS = \
	10threads.c \
	answer.c \
	bitblt.c keyblit.c \
	checkers.c \
	env.c \
	fibonacci.c \
	fibre.c \
	helloworld.c \
	perf.c \
	pi.c \
	roman.c \
	sep.c \
	tmalloc.c \
	ca1d_demo.c \
	ca1d_demo2.c

TESTS = $(DEMOS)

include $(top_srcdir)/build-aux/slctests.mk

EXTRA_DIST = $(DEMOS)
EXTRA_DIST += fibre.c.d1 fibre.c.d2 fibre.c.d3 fibre.c.d4 bitblt.c.d keyblit.c.d
EXTRA_DIST += ca1d.h ca1d.c ca1d_main.c

if ENABLE_DEMOS
noinst_DATA = $(DEMOS:.c=.x)
CLEANFILES = $(DEMOS:.c=.x) $(DEMOS:.c=.bin.*)
endif

