##
## Binutils dist
## 

include common.mk
include dist-common.mk

BINUTILS_DIST_REV = latest
BINUTILS_DIST_BASEVER = 1.18

BU_SVN_REV := $(strip $(if $(filter latest, $(BINUTILS_DIST_REV)), \
	          $(shell $(SVN) info $(BINUTILS_REPO) | \
	                  grep 'Last Changed Rev' | \
	                  cut -d: -f2), $(BINUTILS_DIST_REV)))

BINUTILS_DIST_VERSION = $(BINUTILS_DIST_BASEVER)-$(BU_SVN_REV)
BINUTILS_DISTBASE = binutils-$(BINUTILS_DIST_VERSION)

$(META_SOURCES)/$(BINUTILS_DISTBASE)/download_done: $(META_SOURCES)
	rm -f $@
	(cd $(META_SOURCES) && \
	  $(SVN) export -r$(BU_SVN_REV) $(BINUTILS_REPO) $(BINUTILS_DISTBASE) && \
	  touch $(BINUTILS_DISTBASE)/download_done)

$(DISTDIR)/$(BINUTILS_DISTBASE).tar.bz2: $(META_SOURCES)/$(BINUTILS_DISTBASE)/download_done $(DISTDIR)
	(cd $(META_SOURCES) && \
	  echo $(BINUTILS_DIST_VERSION) > $(BINUTILS_DISTBASE)/bfd/.tarball-version && \
	  tar -cjvf $$OLDPWD/$@.tmp $(BINUTILS_DISTBASE) && \
	  mv -f $$OLDPWD/$@.tmp $$OLDPWD/$@)

binutils-dist: $(DISTDIR)/$(BINUTILS_DISTBASE).tar.bz2

