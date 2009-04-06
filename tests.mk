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

EXTRA_IGNORE =

if !ENABLE_CHECK_UTC
EXTRA_IGNORE += utc0*:[CLRD]
endif

if !ENABLE_CHECK_PPP
EXTRA_IGNORE += ppp*:[CLRD]
endif

TESTS_ENVIRONMENT = \
	$(SLC_VARS) \
	TEST_HERE=1 XIGNORE="*x:* $(EXTRA_IGNORE)" \
	SLT_IMPL_LIST="seqc ptl ppp::-Ws,-o~-Ws,NumProcessors=1 ppp utc0 utcx" \
	$(SHELL) $(abs_top_builddir)/bin/slt

