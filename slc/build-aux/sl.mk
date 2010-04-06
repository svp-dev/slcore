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

include $(top_srcdir)/build-aux/slcvars.mk

SUFFIXES = .x

slc_verbose = $(slc_verbose_$(V))
slc_verbose_ = $(slc_verbose_$(AM_DEFAULT_VERBOSITY))
slc_verbose_0 = @echo '  SLC    $@';

slc_shverbose = $(slc_shverbose_$(V))
slc_shverbose_ = $(slc_shverbose_$(AM_DEFAULT_VERBOSITY))
slc_shverbose_0 = :

if ENABLE_SLC_MTALPHA
slc_ifmtalpha =
else
slc_ifmtalpha = :
endif

if ENABLE_SLC_PTL
slc_ifptl =
else
slc_ifptl = :
endif

SLC_BEFORE = function slc_compile() { \
	$(slc_shverbose) set -x ; \
	if test -n "$$SLC_OUT"; then rm -f "$$SLC_OUT" "$$SLC_OUT".{mtalpha,ptl,seqc}; fi && \
	echo "  SLC    $$SLC_OUT".seqc && \
	$(SLC_LOCAL) $${SLC_OUT:+-o "$$SLC_OUT".seqc} -b seqc "$$@" -I$(srcdir) -I$(builddir) \
	      $(AM_CFLAGS) $(CFLAGS) $(SLFLAGS) $(SLFLAGS_SEQC) $(LIBS) && \
	$(slc_ifptl) echo "  SLC    $$SLC_OUT".ptl && \
        $(slc_ifptl) $(SLC_LOCAL) $${SLC_OUT:+-o "$$SLC_OUT".ptl} -b ptl "$$@" -I$(srcdir) -I$(builddir) \
	      $(AM_CXXFLAGS) $(CXXFLAGS) $(SLFLAGS) $(SLFLAGS_PTL) $(LIBS) && \
	$(slc_ifmtalpha) echo "  SLC    $$SLC_OUT".mtalpha && \
        $(slc_ifmtalpha) $(SLC_LOCAL) $${SLC_OUT:+-o "$$SLC_OUT".mtalpha} -b mtalpha "$$@" \
	      -I$(srcdir) -I$(builddir) -lm $(SLFLAGS) $(SLFLAGS_MTALPHA) && \
	if test -n "$$SLC_OUT"; then \
	  printf '\#! /bin/sh\n' >"$@" && \
	  printf 'echo "This script is only a stub. Run the actual program with:"\n' >>"$@" && \
	  printf 'echo; echo "\tslr "$$(basename "$$0" .x)".bin.XXXX"; echo; exit 1\n' >>"$@"; \
	fi; }

.c.x:
	@$(SLC_BEFORE); $(SLC_VARS) SLC_OUT="$(@:.x=.bin)" slc_compile $<


