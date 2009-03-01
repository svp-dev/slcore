EXTRA_DIST = README
SUBDIRS = . bin lib doc demo

README: $(srcdir)/doc/README.txt
	rm -f $@
	cp $(srcdir)/doc/README.txt $@
	chmod -w $@

nobase_dist_pkgdata_DATA = \
	include/slc.m4 \
	include/slc_m4sugar.m4 \
	include/svp/assert.slh \
	include/svp/div.slh \
	include/svp/io.slh \
	include/svp/iomacros.slh \
	include/svp/lib.slh

MAINTAINERCLEANFILES = include/slc_m4sugar.m4

if MAINTAINER_MODE
include/slc_m4sugar.m4: $(srcdir)/include/m4sugar.m4 $(srcdir)/include/m4sugar_transform.sed
	$(SED) -f $(srcdir)/include/m4sugar_transform.sed $< >$@.tmp
	mv $@.tmp $@
endif
