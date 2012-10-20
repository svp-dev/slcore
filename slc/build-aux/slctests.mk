## tests.mk: This file is part of the SL toolchain.
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

include $(top_srcdir)/build-aux/sl.mk

EXTRA_TEST_IMPL =

if ENABLE_CHECK_PTL
EXTRA_TEST_IMPL += ptl_n ptl_s 
EXTRA_TEST_IMPL += ptld_n ptld_s
endif

if ENABLE_CHECK_HLSIM
EXTRA_TEST_IMPL += hls hls_n hls_s
EXTRA_TEST_IMPL += hlsd hlsd_n hlsd_s
endif

if ENABLE_CHECK_PPP
EXTRA_TEST_IMPL += mta_on
if ENABLE_CHECK_COMA_ZL
EXTRA_TEST_IMPL += mta_on::-m~lzcoma_default
endif
endif

if ENABLE_CHECK_SPR
if ENABLE_CHECK_SPR_MTA
EXTRA_TEST_IMPL += mta
EXTRA_TEST_IMPL += mta_n
EXTRA_TEST_IMPL += mta_s
if ENABLE_CHECK_COMA_ZL
EXTRA_TEST_IMPL += mta::-m~lzcoma_default
EXTRA_TEST_IMPL += mta_n::-m~lzcoma_default
EXTRA_TEST_IMPL += mta_s::-m~lzcoma_default
endif
endif

if ENABLE_CHECK_SPR_MTS
EXTRA_TEST_IMPL += mts_n
endif

if ENABLE_CHECK_SPR_MIPSEL
EXTRA_TEST_IMPL += mipsel_s
endif
# add extra implementations here when they become available
endif

if ENABLE_CHECK_UTC
EXTRA_TEST_IMPL += utc0:-O0 utc0:-O1 utcx
endif

SLT_IMPL_LIST ?= seqc $(EXTRA_TEST_IMPL)

TEST_EXTENSIONS = .sl .c
SL_LOG_COMPILER = \
	env $(SLC_VARS) SRCDIR=$(srcdir) \
	SLT_IMPL_LIST="$(SLT_IMPL_LIST)" \
	DUMP_LOGS=1 TEXT_ONLY=1 SEQUENTIAL=1 \
	$(BASH) $(SLT)

C_LOG_COMPILER = $(SL_LOG_COMPILER)

.PHONY: check-slt
check-slt: $(check_DATA) $(TESTS)
	-echo; echo "Current directory:" `pwd`
	$(AM_V_at)$(SLC_VARS) SLT_IMPL_LIST="$(SLT_IMPL_LIST)" SRCDIR=$(srcdir) \
	    $(SLT_MANY) \
	    `for t in $(TESTS); do if test -r $$t; then echo $$t; else echo $(srcdir)/$$t; fi; done`

clean-local:
	$(AM_V_at)find . -name _\* -type d | xargs rm -rf

