##
## utc-gcc dist
## 

include common.mk
include dist-common.mk

MGGCC_DIST_SVNBRANCH = branches/mt-ra-shared-split
MGGCC_DIST_REV = latest
MGGCC_DIST_BASEVER = 4.1.0

MGGCC_BASEBRANCH := $(strip $(shell basename $(MGGCC_DIST_SVNBRANCH)))

MGGCC_SVN_REV := $(strip $(if $(filter latest, $(MGGCC_DIST_REV)), \
	          $(shell $(SVN) info $(MGGCC_REPO)/$(MGGCC_DIST_SVNBRANCH) | \
	                  grep 'Last Changed Rev' | \
	                  cut -d: -f2), $(MGGCC_DIST_REV)))

MGGCC_DIST_VERSION = $(MGGCC_DIST_BASEVER)-$(MGGCC_SVN_REV)-$(MGGCC_BASEBRANCH)
MGGCC_DISTBASE = mggcc-$(MGGCC_DIST_VERSION)

$(META_SOURCES)/$(MGGCC_DISTBASE)/download_done:
	rm -f $@
	mkdir -p $(META_SOURCES)
	cd $(META_SOURCES) && \
	   $(SVN) export -r$(MGGCC_SVN_REV) $(MGGCC_REPO)/$(MGGCC_DIST_SVNBRANCH) $(MGGCC_DISTBASE)
	touch $@

$(DISTDIR)/$(MGGCC_DISTBASE).tar.bz2: $(META_SOURCES)/$(MGGCC_DISTBASE)/download_done
	rm -f $@.tmp $@
	mkdir -p $(DISTDIR)
	echo $(MGGCC_DIST_VERSION) > $(META_SOURCES)/$(MGGCC_DISTBASE)/gcc/.tarball-version
	tar -cjvf $@.tmp -C $(META_SOURCES) $(MGGCC_DISTBASE)
	mv -f $@.tmp $@

mggcc-dist: $(DISTDIR)/$(MGGCC_DISTBASE).tar.bz2

