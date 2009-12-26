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

SUBDIRS = . build-aux tools lib doc

README: $(srcdir)/doc/README.txt
	$(AM_V_at)rm -f $@
	$(AM_V_GEN)cp $(srcdir)/doc/README.txt $@
	$(AM_V_at)chmod -w $@

NEWS: $(srcdir)/doc/NEWS.txt
	$(AM_V_at)rm -f $@
	$(AM_V_GEN)cp $(srcdir)/doc/NEWS.txt $@
	$(AM_V_at)chmod -w $@

ACLOCAL_AMFLAGS = -I build-aux -I m4

##
## Version number management
##

EXTRA_DIST = .version build-aux/version-gen build-aux/package-version
BUILT_SOURCES = $(top_srcdir)/.version

$(top_srcdir)/.version:
	echo $(VERSION) >$@-t && mv $@-t $@

dist-hook: check-version
	echo $(VERSION) >$(distdir)/build-aux/tarball-version

install-recursive install-exec-recursive install-data-recursive installcheck-recursive: check-version

VERSION_GEN = (cd $(top_srcdir); pre=; if test -d ../.git -a ! -d .git; then cd ..; pre=slc/; fi; \
	$${pre}build-aux/version-gen $${pre}build-aux/tarball-version $${pre}build-aux/package-version)

.PHONY: check-version _version
check-version:
	set -e; \
	if ! test "x$(VERSION)" = "x`$(VERSION_GEN)`"; then \
	   echo "Version string not up to date: run 'make _version' first." >&2; \
	   exit 1; \
	fi

_version:
	cd $(srcdir) && rm -rf autom4te.cache .version && $${AUTORECONF:-autoreconf}
