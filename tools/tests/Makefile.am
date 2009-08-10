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

include $(srcdir)/alltests.mk
EXTRA_DIST = $(TESTSUITE) $(XTESTSUITE)

if ENABLE_TESTSUITE
TESTS = $(TESTSUITE)
if ENABLE_XFAIL_TESTS
TESTS += $(XTESTSUITE)
endif
endif

include $(top_srcdir)/tests.mk

EXTRA_DIST += 0_slr/fibre.c.d
