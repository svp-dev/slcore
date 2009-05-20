# 
# slc, a SL compiler.
# 
# Copyright (C) 2008,2009 The SL project..
# All rights reserved.
# 

include $(top_srcdir)/sl.mk

EXTRA_TEST_IMPL =

if ENABLE_CHECK_PTL
EXTRA_TEST_IMPL += ptl
endif

if ENABLE_CHECK_PPP
if ENABLE_MTALPHA
EXTRA_TEST_IMPL += ppp::-Ws,-o~-Ws,NumProcessors=1 ppp
endif
endif

if ENABLE_CHECK_UTC
if ENABLE_MTALPHA
EXTRA_TEST_IMPL += utc0:-O0 utc0:-O1 utc0:-O2 utcx
endif
endif


TESTS_ENVIRONMENT = \
	$(SLC_VARS) \
	SLT_IMPL_LIST="seqc $(EXTRA_TEST_IMPL)" \
	$(BASH) $(abs_top_builddir)/bin/slt

