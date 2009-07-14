## tests.mkm this file is part of the SL toolchain.
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
EXTRA_TEST_IMPL += utc0:-O0 utc0:-O1 utc0:-O2 utcx
endif
endif

TEST_EXTENSIONS = .sl
SL_LOG_COMPILER = \
	$(SLC_VARS) \
	SLT_IMPL_LIST="seqc $(EXTRA_TEST_IMPL)" \
	DUMP_LOGS=1 TEXT_ONLY=1 SEQUENTIAL=1 \
	$(BASH) $(abs_top_builddir)/tools/bin/slt

