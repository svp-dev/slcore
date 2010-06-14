SUBDIRS = bin . lib

nobase_dist_pkgdata_DATA = \
	include/slc.m4 \
	include/slc_m4sugar.m4 \
	include/slc_m4sversion.m4 \
	include/slc_m4sforeach.m4 \
	include/svp/slr.h \
	include/svp/delegate.h \
	include/svp/fibre.h \
	include/svp/compiler.h \
	include/svp/gomt.h \
	include/svp/abort.h \
	include/svp/testoutput.h \
	include/svp/mgsim.h

EXTRA_DIST = \
	include/m4sugar.m4 \
	include/m4sugar_version.m4 \
	include/m4sugar_foreach.m4 \
	include/m4sugar_regexp.patch \
	include/m4sugar_transform.sed

MAINTAINERCLEANFILES = \
	include/slc_m4sugar.m4 \
	include/slc_m4sversion.m4 \
	include/slc_m4sforeach.m4

include/slc_m4sugar.m4: $(srcdir)/include/m4sugar.m4 $(srcdir)/include/m4sugar_transform.sed $(srcdir)/include/m4sugar_regexp.patch
	$(AM_V_at)$(MKDIR_P) include
	$(AM_V_GEN)$(SED) -f $(srcdir)/include/m4sugar_transform.sed $< >$@.tmp && patch $@.tmp $(srcdir)/include/m4sugar_regexp.patch
	$(AM_V_at)mv $@.tmp $@

include/slc_m4sversion.m4: $(srcdir)/include/m4sugar_version.m4 $(srcdir)/include/m4sugar_transform.sed
	$(AM_V_at)$(MKDIR_P) include
	$(AM_V_GEN)$(SED) -f $(srcdir)/include/m4sugar_transform.sed $< >$@.tmp
	$(AM_V_at)mv $@.tmp $@

include/slc_m4sforeach.m4: $(srcdir)/include/m4sugar_foreach.m4 $(srcdir)/include/m4sugar_transform.sed
	$(AM_V_at)$(MKDIR_P) include
	$(AM_V_gen)$(SED) -f $(srcdir)/include/m4sugar_transform.sed $< >$@.tmp
	$(AM_V_at)mv $@.tmp $@


