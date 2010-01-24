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

$(META_SOURCES)/$(BINUTILS_DISTBASE)/download_done:
	rm -f $@
	mkdir -p $(META_SOURCES)
	cd $(META_SOURCES) && $(SVN) export -r$(BU_SVN_REV) $(BINUTILS_REPO) $(BINUTILS_DISTBASE)
	touch $@

$(DISTDIR)/$(BINUTILS_DISTBASE).tar.bz2: $(META_SOURCES)/$(BINUTILS_DISTBASE)/download_done
	rm -f $@.tmp $@
	mkdir -p $(DISTDIR)
	echo $(BINUTILS_DIST_VERSION) > $(META_SOURCES)/$(BINUTILS_DISTBASE)/bfd/.tarball-version
	tar -cjvf $@.tmp -C $(META_SOURCES) $(BINUTILS_DISTBASE)
	mv -f $@.tmp $@

binutils-dist: $(DISTDIR)/$(BINUTILS_DISTBASE).tar.bz2

