bin_SCRIPTS = cce slc sgr
noinst_SCRIPTS = cce.local slc.local sgr.local
dist_bin_SCRIPTS = scu

EXTRA_DIST = cce.in slc.in sgr.in
CLEANFILES = $(bin_SCRIPTS) $(noinst_SCRIPTS)

edit_base = $(SED) \
	       -e 's|@SBASH[@]|$(SBASH)|g'

edit_src = $(edit_base) \
	       -e 's|@incdir[@]|$(abs_top_srcdir)/include|g' \
	       -e 's|@srcdatadir[@]|$(abs_top_srcdir)/lib|g' \
	       -e 's|@pkgdatadir[@]|$(abs_top_builddir)/lib|g' \
	       -e 's|@srclibdir[@]|$(abs_top_srcdir)/lib|g' \
	       -e 's|@pkglibdir[@]|$(abs_top_builddir)/lib|g' \
	       -e 's|@cce_helper[@]|$(abs_builddir)/cce.local|g' 

edit_inst = $(edit_base) \
	       -e 's|@incdir[@]|$(pkgdatadir)/include|g' \
	       -e 's|@srcdatadir[@]|$(pkgdatadir)|g' \
	       -e 's|@pkgdatadir[@]|$(pkgdatadir)|g' \
	       -e 's|@srclibdir[@]|$(pkglibdir)|g' \
	       -e 's|@pkglibdir[@]|$(pkglibdir)|g' \
	       -e 's|@cce_helper[@]|$(bindir)/cce|g'

cce sgr slc: Makefile
	$(MKDIR_P) bin
	rm -f $@ $@.tmp
	$(edit_inst) $(srcdir)/$@.in >$@.tmp
	chmod +x $@.tmp
	chmod a-w $@.tmp
	mv $@.tmp $@

cce.local sgr.local slc.local: Makefile
	rm -f $@ $@.tmp
	$(edit_src) $(srcdir)/$(@:.local=.in) >$@.tmp
	chmod +x $@.tmp
	chmod a-w $@.tmp
	mv $@.tmp $@

cce cce.local: cce.in
sgr sgr.local: sgr.in
slc slc.local: slc.in

