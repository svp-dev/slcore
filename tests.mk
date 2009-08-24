## tests.mk: This file is part of the SL toolchain.
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

include $(top_srcdir)/sl.mk

EXTRA_TEST_IMPL =

if ENABLE_CHECK_PTL
EXTRA_TEST_IMPL += ptl
endif

if ENABLE_CHECK_PPP
if ENABLE_MTALPHA
EXTRA_TEST_IMPL += ppp::-n~1 ppp
endif
endif

if ENABLE_CHECK_UTC
if ENABLE_MTALPHA
EXTRA_TEST_IMPL += utc0:-O0 utc0:-O1 utcx
endif
endif

SLT_IMPL_LIST ?= seqc $(EXTRA_TEST_IMPL)

TEST_EXTENSIONS = .sl .c
SL_LOG_COMPILER = \
	$(SLC_VARS) SRCDIR=$(srcdir) \
	SLT_IMPL_LIST="$(SLT_IMPL_LIST)" \
	DUMP_LOGS=1 TEXT_ONLY=1 SEQUENTIAL=1 \
	$(BASH) $(abs_top_builddir)/tools/bin/slt

C_LOG_COMPILER = $(SL_LOG_COMPILER)

.PHONY: check-slt
check-slt: $(check_DATA) $(TESTS)
	-echo; echo "Current directory:" `pwd`
	$(AM_V_at)$(SLC_VARS) SLT_IMPL_LIST="$(SLT_IMPL_LIST)" SRCDIR=$(srcdir) \
	    $(top_builddir)/tools/bin/slt-many \
	    `for t in $(TESTS); do if test -r $$t; then echo $$t; else echo $(srcdir)/$$t; fi; done`

clean-local:
	$(AM_V_at)find . -name _\* -type d | xargs rm -rf

