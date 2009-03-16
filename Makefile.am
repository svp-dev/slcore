EXTRA_DIST = README
SUBDIRS = . bin lib doc tests demo

README: $(srcdir)/doc/README.txt
	rm -f $@
	cp $(srcdir)/doc/README.txt $@
	chmod -w $@

NEWS: $(srcdir)/doc/NEWS.txt
	rm -f $@
	cp $(srcdir)/doc/NEWS.txt $@
	chmod -w $@

nobase_dist_pkgdata_DATA = \
	include/slc.m4 \
	include/slc_m4sugar.m4 \
	include/slc_m4sversion.m4 \
	include/slc_m4sforeach.m4 \
	include/cstdint.slh \
	include/cstddef.slh \
	include/svp/argslot.slh \
	include/svp/assert.slh \
	include/svp/div.slh \
	include/svp/io.slh \
	include/svp/iomacros.slh \
	include/svp/lib.slh

MAINTAINERCLEANFILES = include/slc_m4sugar.m4 include/slc_m4sversion.m4 include/slc_m4sforeach.m4

if MAINTAINER_MODE
include/slc_m4sugar.m4: $(srcdir)/include/m4sugar.m4 $(srcdir)/include/m4sugar_transform.sed
	$(MKDIR_P) include
	$(SED) -f $(srcdir)/include/m4sugar_transform.sed $< >$@.tmp
	mv $@.tmp $@

include/slc_m4sversion.m4: $(srcdir)/include/m4sugar_version.m4 $(srcdir)/include/m4sugar_transform.sed
	$(MKDIR_P) include
	$(SED) -f $(srcdir)/include/m4sugar_transform.sed $< >$@.tmp
	mv $@.tmp $@

include/slc_m4sforeach.m4: $(srcdir)/include/m4sugar_foreach.m4 $(srcdir)/include/m4sugar_transform.sed
	$(MKDIR_P) include
	$(SED) -f $(srcdir)/include/m4sugar_transform.sed $< >$@.tmp
	mv $@.tmp $@
endif
