##
## Binutils dist
## 

include common.mk
include dist-common.mk

BINUTILS_DIST_SVNBRANCH = trunk
BINUTILS_DIST_REV = latest
BINUTILS_DIST_BASEVER = 1.18

BINUTILS_BASEBRANCH := $(strip $(shell basename $(BINUTILS_DIST_SVNBRANCH)))
BU_SVN_REV := $(strip $(if $(filter latest, $(BINUTILS_DIST_REV)), \
	          $(shell $(SVN) info $(BINUTILS_REPO)/$(BINUTILS_DIST_SVNBRANCH) | \
	                  grep 'Last Changed Rev' | \
	                  cut -d: -f2), $(BINUTILS_DIST_REV)))

BINUTILS_DIST_VERSION = $(BINUTILS_DIST_BASEVER)-$(BU_SVN_REV)-$(BINUTILS_BASEBRANCH)
BINUTILS_DISTBASE = binutils-$(BINUTILS_DIST_VERSION)

$(META_SOURCES)/$(BINUTILS_DISTBASE)/download_done:
	rm -f $@
	$(MKDIR_P) $(META_SOURCES)
	cd $(META_SOURCES) && \
	   $(SVN) export -r$(BU_SVN_REV) $(BINUTILS_REPO)/$(BINUTILS_DIST_SVNBRANCH) $(BINUTILS_DISTBASE)
	touch $@

$(ARCHIVEDIR)/$(BINUTILS_DISTBASE).tar.bz2: $(META_SOURCES)/$(BINUTILS_DISTBASE)/download_done
	rm -f $@.tmp $@
	$(MKDIR_P) $(ARCHIVEDIR)
	echo $(BINUTILS_DIST_VERSION) > $(META_SOURCES)/$(BINUTILS_DISTBASE)/bfd/.tarball-version
	tar -cjvf $@.tmp -C $(META_SOURCES) $(BINUTILS_DISTBASE)
	mv -f $@.tmp $@

binutils-dist: $(ARCHIVEDIR)/$(BINUTILS_DISTBASE).tar.bz2

