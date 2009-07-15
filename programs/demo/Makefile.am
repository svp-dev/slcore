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
DEMOS = 10threads.sl answer.sl fibonacci.sl helloworld.sl roman.sl fibre.sl
TESTS = $(DEMOS)

include $(top_srcdir)/tests.mk

EXTRA_DIST = $(DEMOS)

if ENABLE_DEMOS
noinst_DATA = $(DEMOS:.sl=.x)
CLEANFILES = $(DEMOS:.sl=.x) $(DEMOS:.sl=.bin.*)
endif
