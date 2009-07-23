## Makefile.am: this file is part of the SL toolchain.
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

STATIC_DEMOS = k2-alt.sl k4-alt.sl
DEMOS = $(STATIC_DEMOS)
EXTRA_DIST = \
	benchmarker.sh bench.exp \
	main_bench.sl main_simple.sl main_test.sl \
	arrays.h ll.slh \
	$(STATIC_DEMOS)
DISTCLEANFILES = 
check_DATA = 

define gen-ll-program
	$(AM_V_at)rm -f $@ $@.tmp
	$(AM_V_GEN)(echo "/* This file was auto-generated. Do not edit! */" && \
	echo "// XIGNORE: *:D" && \
	echo "m4_define([[KERNEL]], $*)" && \
	echo "m4_define([[MAIN]],[[main_$(subst $*,,$@)]])" && \
	echo "m4_include(ll.slh)" && \
	for u in `$(GREP) "LL_USE:" <"$<"|$(SED) -e 's/^.*:\([^:]*\)$$/\1/g;s/,/ /g'`; do \
	  $(SED) -e 's,//.*$$,,g' <"$(srcdir)/arrays.h" | $(GREP) " $$u[^a-zA-Z0-9_]" && \
	  echo "const long donttouch$$RANDOM[4096] = {0};"; \
	done && \
	echo "m4_include([[k]]KERNEL.sl)" && \
	echo "m4_include(MAIN)") >$@.tmp
	$(AM_V_at)chmod a-w $@.tmp
	$(AM_V_at)mv $@.tmp $@
endef

simple%.sl: k%.sl arrays.h Makefile.am
	$(gen-ll-program)

bench%.sl: k%.sl arrays.h Makefile.am
	$(gen-ll-program)
	$(AM_V_at)chmod +w $@
	$(AM_V_at)echo "// SLT_RUN: -f TEST.in" >>$@
	$(AM_V_at)chmod -w $@

test%.sl: k%.sl arrays.h Makefile.am
	$(gen-ll-program)

bench%.sl.in: bench%.bin.mtalpha.psize
	$(AM_V_at)rm -f $@ $@.tmp
	$(AM_V_GEN)(echo 1; cat $<) >$@.tmp
	$(AM_V_at)chmod a-w $@.tmp
	$(AM_V_at)mv -f $@.tmp $@

include $(srcdir)/ll.mk

TESTS = $(DEMOS)

include $(top_srcdir)/tests.mk

if ENABLE_DEMOS
noinst_DATA = $(DEMOS:.sl=.x)
CLEANFILES = $(DEMOS:.sl=.x) $(DEMOS:.sl=.bin.*)
endif


