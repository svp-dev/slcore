## sl.mk: this file is part of the SL toolchain.
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

SUFFIXES = .sl .x

slc_verbose = $(slc_verbose_$(V))
slc_verbose_ = $(slc_verbose_$(AM_DEFAULT_VERBOSITY))
slc_verbose_0 = @echo '  SLC    $@';

if ENABLE_MTALPHA
slc_ifpppalpha =
else
slc_ifpppalpha = :
endif

SLC_BEFORE = function slc_compile() { \
       if test -n "$$SLC_OUT"; then rm -f "$$SLC_OUT" "$$SLC_OUT".{mtalpha,ptl,seqc}; fi && \
       echo "$$SLC_OUT -> seqc" && \
          $(abs_top_builddir)/bin/slc $${SLC_OUT:+-o "$$SLC_OUT".seqc} -b seqc "$$@" $(AM_CFLAGS) $(CFLAGS) && \
       echo "$$SLC_OUT -> ptl" && \
          $(abs_top_builddir)/bin/slc $${SLC_OUT:+-o "$$SLC_OUT".ptl} -b ptl "$$@" $(AM_CXXFLAGS) $(CXXFLAGS) && \
       $(slc_ifppalpha) echo "$$SLC_OUT -> pppalpha" && \
          $(slc_ifppalpha) $(abs_top_builddir)/bin/slc $${SLC_OUT:+-o "$$SLC_OUT".mtalpha} -b ppp "$$@" && \
       if test -n "$$SLC_OUT"; then \
          printf '\#\! /bin/sh\nex=$${1:?missing run tag}; shift; exec $${SLR:-$(abs_top_builddir)/bin/slr} $$0.$$ex "$$@"\n'  >"$@" && \
          chmod +x "$@" ; \
       fi; }

.sl.x:
	$(slc_verbose) $(SLC_BEFORE); $(SLC_VARS) SLC_OUT="$@" slc_compile $<


