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
	roman.c \
	sep.c

TESTS = $(DEMOS)

include $(top_srcdir)/build-aux/slctests.mk

EXTRA_DIST = $(DEMOS)
EXTRA_DIST += fibre.c.d1 fibre.c.d2 fibre.c.d3 bitblt.c.d keyblit.c.d

if ENABLE_DEMOS
noinst_DATA = $(DEMOS:.c=.x)
CLEANFILES = $(DEMOS:.c=.x) $(DEMOS:.c=.bin.*)
endif

