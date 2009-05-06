# 
# slc, a SL compiler.
# 
# Copyright (C) 2008,2009 The SL project..
# All rights reserved.
# 

SLC_VARS = \
	SLC_INCDIR=$(abs_top_srcdir)/include:$(abs_top_builddir)/include \
	SLC_LIBDIR=$(abs_top_srcdir)/lib:$(abs_top_builddir)/lib \
	SLC_DATADIR=$(abs_top_srcdir)/lib:$(abs_top_builddir)/lib \
	SPP=$(abs_top_srcdir)/bin/spp \
	SCU=$(abs_top_srcdir)/bin/scu \
	SAG=$(abs_top_srcdir)/bin/sag \
	CCE=$(abs_top_builddir)/bin/cce \
	SLR=$(abs_top_builddir)/bin/slr \
	SLC=$(abs_top_builddir)/bin/slc

SLC = $(SLC_VARS) $(abs_top_builddir)/bin/slc

EXTRA_TEST_IMPL =

if ENABLE_CHECK_PPP
if ENABLE_MTALPHA
EXTRA_TEST_IMPL += ppp::-Ws,-o~-Ws,NumProcessors=1 ppp
endif
endif

if ENABLE_CHECK_UTC
EXTRA_TEST_IMPL += utc0 utcx
endif


TESTS_ENVIRONMENT = \
	$(SLC_VARS) \
	TEST_HERE=1 \
	SLT_IMPL_LIST="seqc ptl $(EXTRA_TEST_IMPL)" \
	$(BASH) $(abs_top_builddir)/bin/slt

