## sl.mk: this file is part of the SL toolchain.
##
## Copyright (C) 2009-2015 The SL project
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

SUFFIXES = .x .bin

slc_verbose = $(slc_verbose_$(V))
slc_verbose_ = $(slc_verbose_$(AM_DEFAULT_VERBOSITY))
slc_verbose_0 = @echo '  SLC    $@';

slc_shverbose = $(slc_shverbose_$(V))
slc_shverbose_ = $(slc_shverbose_$(AM_DEFAULT_VERBOSITY))
slc_shverbose_0 = :

SL_TARGET_LIST =

if ENABLE_SLC_MTALPHA
SL_TARGET_LIST += mta mta_s mta_n mta_on
endif

if ENABLE_SLC_MTSPARC
SL_TARGET_LIST += mtsn mtsn_s mtsn_n
endif

if ENABLE_SLC_LEON2MT
SL_TARGET_LIST += l2mt l2mt_s l2mt_n
endif

if ENABLE_SLC_MIPSEL
SL_TARGET_LIST += mipsel_s
endif

if ENABLE_SLC_PTL
SL_TARGET_LIST += ptl_n ptl_s ptld_n ptld_s
endif

if ENABLE_SLC_HLSIM
SL_TARGET_LIST += hls hls_n hls_s hlsd hlsd_n hlsd_s
endif

.PHONY: %.bin
%.bin:
	$(slc_verbose)t=$@; t=$${t%.bin}; t=$${t##*.}; \
	 cmd="$(SLC_LOCAL) -b $$t -o $@ $(SOURCE) -I$(srcdir) -I$(builddir) $(SLFLAGS) `eval echo \\$$SLFLAGS_$$t`"; \
	 $(slc_shverbose) echo "$$cmd"; \
	 $$cmd

%.x: %.c
	$(AM_V_at)rm -f $@ $*.*.bin
	$(AM_V_at)$(foreach P,$(SL_TARGET_LIST), export SLFLAGS_$(P)="$(SLFLAGS_$(P))" &&) \
	$(MAKE) `n=$@; for t in $(SL_TARGET_LIST); do echo $${n%.x}.$$t.bin; done` SOURCE=$<
	$(AM_V_at)echo echo "This script is only a stub, run .bin with slr." >$@
