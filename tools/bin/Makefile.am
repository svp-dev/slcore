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

# 
bin_SCRIPTS = cce slc slr slt timeout
dist_bin_SCRIPTS = scu spp sag

EXTRA_DIST = cce.in slc.in slr.in slt.in timeout.in

CLEANFILES = $(bin_SCRIPTS)

edit_scripts = $(SED) \
	       -e 's|@BASH[@]|$(BASH)|g' \
	       -e 's|@CC[@]|$(CC)|g' \
	       -e 's|@CXX[@]|$(CXX)|g' \
	       -e 's|@M4[@]|$(M4)|g' \
	       -e 's|@CC_ALPHA[@]|$(CC_ALPHA)|g' \
	       -e 's|@UTCC_MTALPHA[@]|$(UTCC_MTALPHA)|g' \
	       -e 's|@CC_SPARC[@]|$(CC_SPARC)|g' \
	       -e 's|@UTCC_MTSPARC[@]|$(UTCC_MTSPARC)|g' \
	       -e 's|@python[@]|$(PYTHON)|g' \
	       -e 's|@slc_libdir[@]|$(pkglibdir)|g' \
	       -e 's|@slc_incdir[@]|$(pkgdatadir)/include|g' \
	       -e 's|@slc_datadir[@]|$(pkgdatadir)|g' \
	       -e 's|@cce_helper[@]|$(bindir)/cce|g' \
	       -e 's|@scu_helper[@]|$(bindir)/scu|g' \
	       -e 's|@slc_helper[@]|$(bindir)/slc|g' \
	       -e 's|@slr_helper[@]|$(bindir)/slr|g' \
	       -e 's|@sag_helper[@]|$(bindir)/sag|g' \
	       -e 's|@spp_helper[@]|$(bindir)/spp|g' \
	       -e 's|@slc_version[@]|$(PACKAGE_VERSION)|g' \
	       -e 's|@slc_revision[@]|$(SVN_REVISION)|g' \
	       -e 's|@bugreports[@]|$(PACKAGE_BUGREPORT)|g'

cce slr slc slt timeout: $(top_srcdir)/ChangeLog
	$(AM_V_at)rm -f $@ $@.tmp
	$(AM_V_GEN)$(edit_scripts) $(srcdir)/$@.in >$@.tmp
	$(AM_V_at)chmod +x $@.tmp
	$(AM_V_at)chmod a-w $@.tmp
	$(AM_V_at)mv $@.tmp $@

cce: cce.in
slr: slr.in
slc: slc.in
slt: slt.in
timeout: timeout.in

#
# Documentation for the scripts
#
dist_man1_MANS = cce.1 slc.1 slr.1 slt.1

if MAINTAINER_MODE

cce.1 slc.1 slr.1 slt.1:
	rm -f $@
	$(HELP2MAN) -N --output=$@.tmp ./$(@:.1=)
	mv $@.tmp $@

cce.1: cce
slc.1: slc
slr.1: slr
slt.1: slt

MAINTAINERCLEANFILES = $(dist_man1_MANS)

endif
