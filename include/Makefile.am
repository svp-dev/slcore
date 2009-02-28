nobase_dist_pkgdata_DATA = \
	slc.m4 \
	slc_m4sugar.m4 \
	svp/assert.slh \
	svp/div.slh \
	svp/io.slh \
	svp/iomacros.slh \
	svp/lib.slh

MAINTAINERCLEANFILES = slc_m4sugar.m4

if MAINTAINER_MODE
slc_m4sugar.m4: $(srcdir)/m4sugar.m4 $(srcdir)/m4sugar_transform.sed
	$(SED) -f $(srcdir)/m4sugar_transform.sed $< >$@.tmp
	mv $@.tmp $@
endif
