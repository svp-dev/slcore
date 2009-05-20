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

SUFFIXES = .sl .mtalpha-x .seqc-x .ptl-x

slc_verbose = $(slc_verbose_$(V))
slc_verbose_ = $(slc_verbose_$(AM_DEFAULT_VERBOSITY))
slc_verbose_0 = @echo '  SLC    $@';

.sl.mtalpha-x:
	$(slc_verbose)$(SLC) -b ppp -o $@ $< 

.sl.ptl-x:
	$(slc_verbose)$(SLC) -b ptl -o $@ $< $(AM_CXXFLAGS) $(CXXFLAGS)

.sl.seqc-x:
	$(slc_verbose)$(SLC) -b seqc -o $@ $< $(AM_CFLAGS) $(CFLAGS)

