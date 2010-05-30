## Makefile.am: this file is part of the SL toolchain.
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

# 

RUNSCRIPTS = \
	r-host.sh \
	r-mtalpha-sim.sh \
	r-mtsparc-sim.sh
pkglib_SCRIPTS = $(RUNSCRIPTS)
CLEANFILES = $(RUNSCRIPTS)

r-host.sh: r-host.sh.in Makefile
	$(AM_V_at)rm -f $@ $@.out
	$(AM_V_GEN)$(SED) <$< >$@.out \
	   -e 's|@BASH[@]|$(BASH)|g'
	$(AM_V_at)chmod +x,a-w $@.out
	$(AM_V_at)mv -f $@.out $@

r-mtalpha-sim.sh: r-mt-sim.sh.in Makefile
	$(AM_V_at)rm -f $@ $@.out
	$(AM_V_GEN)$(SED) <$< >$@.out \
	 -e 's|@slc_datadir[@]|$(pkgdatadir)|g' \
	 -e 's|@MGSIM_ARCH[@]|$(MGSIM_ALPHA)|g' \
	 -e 's|@SIMX_ARCH[@]|$(SIMX_ALPHA)|g' \
	 -e 's|@NM_ARCH[@]|$(NM_MTALPHA)|g' \
	 -e 's|@ARCH[@]|ALPHA|g' \
	 -e 's|@arch[@]|alpha|g' \
	 -e 's|@BASH[@]|$(BASH)|g' \
	 -e 's|@python[@]|$(PYTHON)|g'
	$(AM_V_at)chmod +x,a-w $@.out
	$(AM_V_at)mv -f $@.out $@

r-mtsparc-sim.sh: r-mt-sim.sh.in Makefile
	$(AM_V_at)rm -f $@ $@.out
	$(AM_V_GEN)$(SED) <$< >$@.out \
	 -e 's|@slc_datadir[@]|$(pkgdatadir)|g' \
	 -e 's|@MGSIM_ARCH[@]|$(MGSIM_SPARC)|g' \
	 -e 's|@SIMX_ARCH[@]|$(SIMX_SPARC)|g' \
	 -e 's|@NM_ARCH[@]|$(NM_MTSPARC)|g' \
	 -e 's|@ARCH[@]|SPARC|g' \
	 -e 's|@arch[@]|sparc|g' \
	 -e 's|@BASH[@]|$(BASH)|g' \
	 -e 's|@python[@]|$(PYTHON)|g'
	$(AM_V_at)chmod +x,a-w $@.out
	$(AM_V_at)mv -f $@.out $@

nobase_python_PYTHON = \
	slc_config.py \
	slc/__init__.py \
	slc/msg.py \
	slc/loc.py \
	slc/ast.py \
	slc/visitors.py \
	slc/mtalpha/__init__.py \
	slc/mtalpha/regmagic.py \
	slc/mtalpha/compile.py \
	slc/mtalpha/visitors.py \
	slc/mtalpha/asmproc/__init__.py \
	slc/mtalpha/asmproc/base.py \
	slc/mtalpha/asmproc/common.py \
	slc/mtalpha/asmproc/opt.py \
	slc/lower/__init__.py \
	slc/lower/seq.py \
	slc/lower/split.py \
	slc/lower/lowercreate.py \
	slc/lower/lowgeta.py \
	slc/lower/flavorseta.py \
	slc/lower/lowccast.py \
	slc/lower/lowclabels.py \
	slc/lower/lowcvars.py \
	slc/lower/remflavors.py \
	slc/lower/rename.py \
	slc/input/__init__.py \
	slc/input/read.py \
	slc/input/parse.py \
	slc/input/cm4.py \
	slc/input/check.py \
	slc/front/__init__.py,
	slc/front/main.py \
	slc/front/chains.py \
	slc/front/dump.py \
	slc/front/opts.py

nobase_dist_pkgdata_DATA = \
	arggen.c \
	t-utc/slimpl.m4 \
	t-utc0/slimpl.m4 \
	t-utc0/include/sl_support.h \
        mt-profiles/coma128 \
        mt-profiles/coma256 \
        mt-profiles/default \
        mt-profiles/lzcoma128_1chan \
        mt-profiles/lzcoma128_2chan \
        mt-profiles/lzcoma256_1chan \
        mt-profiles/lzcoma256_4chan \
	mt-profiles/lzcoma_default \
        mt-profiles/ml-npcrypt-p1 \
        mt-profiles/ml-npcrypt-p2 \
        mt-profiles/ml-npcrypt-p4 \
        mt-profiles/ml-npcrypt-p8 \
        mt-profiles/mlcoma128_1chan \
        mt-profiles/mlcoma256_1chan \
	mt-profiles/mlcoma_default \
        mt-profiles/rbm1 \
        mt-profiles/rbm128 \
        mt-profiles/rbm256 \
        mt-profiles/rbm4 \
        mt-profiles/rbm8 \
        mt-profiles/simple

include $(top_srcdir)/build-aux/sl.mk
nobase_pkglib_DATA =
nobase_pkglib_LIBRARIES = 
EXTRA_DIST = r-mt-sim.sh.in r-host.sh.in
MAINTAINERCLEANFILES = 
BUILT_SOURCES = 

include $(srcdir)/seqc.mk
include $(srcdir)/ptl.mk
include $(srcdir)/mtalpha.mk
