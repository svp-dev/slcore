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

if ENABLE_CHECK_MTA
EXTRA_TEST_IMPL += mta
EXTRA_TEST_IMPL += mta_n
EXTRA_TEST_IMPL += mta_s
endif

if ENABLE_CHECK_MTS
EXTRA_TEST_IMPL += mtsn
EXTRA_TEST_IMPL += mtsn_n
EXTRA_TEST_IMPL += mtsn_s
endif

if ENABLE_CHECK_MIPSEL
EXTRA_TEST_IMPL += mipsel_s
endif


SLT_IMPL_LIST ?= seqc $(EXTRA_TEST_IMPL)

TEST_EXTENSIONS = .c
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

