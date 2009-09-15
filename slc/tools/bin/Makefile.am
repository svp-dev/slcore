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
bin_SCRIPTS = cce slc slr slt timeout slt-many cm4
dist_bin_SCRIPTS = scu spp sag

EXTRA_DIST = cce.in slc.in slr.in slt.in timeout.in slt-many.in cm4.in

CLEANFILES = $(bin_SCRIPTS)

edit_scripts = $(SED) \
	       -e 's|@BASH[@]|$(BASH)|g' \
	       -e 's|@CC_HOST[@]|$(CC)|g' \
	       -e 's|@CXX_HOST[@]|$(CXX)|g' \
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
	       -e 's|@slt_helper[@]|$(bindir)/slt|g' \
	       -e 's|@sag_helper[@]|$(bindir)/sag|g' \
	       -e 's|@spp_helper[@]|$(bindir)/spp|g' \
	       -e 's|@cm4_helper[@]|$(bindir)/cm4|g' \
	       -e 's|@slc_version[@]|$(PACKAGE_VERSION)|g' \
	       -e 's|@slc_revision[@]|$(SVN_REVISION)|g' \
	       -e 's|@bugreports[@]|$(PACKAGE_BUGREPORT)|g'

cce slr slc slt timeout slt-many cm4: Makefile
	$(AM_V_GEN)(D=$$$$.tmp; mkdir $$D && \
	    $(edit_scripts) $(srcdir)/$@.in >$$D/sed_out && \
            chmod +x,a-w $$D/sed_out && \
            mv -f $$D/sed_out $@ && \
            rm -rf $$D)

cce: cce.in
slr: slr.in
slc: slc.in
slt: slt.in
timeout: timeout.in
slt-many: slt-many.in
cm4: cm4.in

#
# Documentation for the scripts
#
dist_man1_MANS = cce.1 slc.1 slr.1 slt.1 slt-many.1

cce.1 slc.1 slr.1 slt.1 slt-many.1: $(top_srcdir)/configure.ac
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) $(@:.1=)
	$(AM_V_GEN)$(HELP2MAN) -N --output=$@ \
	    `test -r $(srcdir)/$(@:.1=) && echo $(srcdir) || echo .`/$(@:.1=)

cce.1: cce.in
slc.1: slc.in
slr.1: slr.in
slt.1: slt.in
slt-many.1: slt-many.in

MAINTAINERCLEANFILES = $(dist_man1_MANS)
