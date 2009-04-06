# 
# slc, a SL compiler.
# 
# Copyright (C) 2008,2009 The SL project..
# All rights reserved.
# 
bin_SCRIPTS = cce slc slr slt timeout
dist_bin_SCRIPTS = scu spp sag
EXTRA_DIST = cce.in slc.in slr.in slt.in timeout.in

CLEANFILES = $(bin_SCRIPTS)

edit_scripts = $(SED) \
	       -e 's|@BASH[@]|$(BASH)|g' \
	       -e 's|@CC[@]|$(CC)|g' \
	       -e 's|@CXX[@]|$(CXX)|g' \
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
	       -e 's|@spp_helper[@]|$(bindir)/spp|g'

cce slr slc slt timeout: Makefile
	rm -f $@ $@.tmp
	$(edit_scripts) $(srcdir)/$@.in >$@.tmp
	chmod +x $@.tmp
	chmod a-w $@.tmp
	mv $@.tmp $@

cce: cce.in
slr: slr.in
slc: slc.in
slt: slt.in
timeout: timeout.in

