# 
# slc, a SL compiler.
# 
# Copyright (C) 2008,2009 The SL project..
# All rights reserved.
# 
bin_SCRIPTS = cce slc sgr
dist_bin_SCRIPTS = scu spp slt timeout
EXTRA_DIST = cce.in slc.in sgr.in

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
	       -e 's|@slc_incdir[@]|$(includedir)|g' \
	       -e 's|@slc_datadir[@]|$(pkgdatadir)|g' \
	       -e 's|@cce_helper[@]|$(bindir)/cce|g' \
	       -e 's|@scu_helper[@]|$(bindir)/scu|g' \
	       -e 's|@spp_helper[@]|$(bindir)/spp|g'

cce sgr slc: Makefile
	rm -f $@ $@.tmp
	$(edit_scripts) $(srcdir)/$@.in >$@.tmp
	chmod +x $@.tmp
	chmod a-w $@.tmp
	mv $@.tmp $@

cce: cce.in
sgr: sgr.in
slc: slc.in

