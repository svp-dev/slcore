##
## GCC dist
## 

include common.mk
include dist-common.mk

GCC_DIST_REV = latest
GCC_DIST_BASEVER = 4.4.3

GCC_VENDOR_ARCHIVE = gcc-core-$(GCC_DIST_BASEVER).tar.bz2
GCC_PATCH = patches/gcc.patch

GCC_SVN_REV := $(strip $(if $(filter latest, $(GCC_DIST_REV)), \
	          $(shell $(SVN) info $(DEPLOY_REPO)/$(GCC_PATCH) | \
	                  grep 'Last Changed Rev' | \
	                  cut -d: -f2), $(GCC_DIST_REV)))


GCC_DIST_VERSION = $(GCC_DIST_BASEVER)-$(GCC_SVN_REV)
GCC_DISTBASE = gcc-$(GCC_DIST_VERSION)
GCC_METASRC = $(META_SOURCES)/$(GCC_DISTBASE)

$(META_SOURCES)/$(GCC_VENDOR_ARCHIVE):
	rm -f $@
	$(MKDIR_P) $(META_SOURCES)
	cd $(META_SOURCES) && $(FETCH) $(GCC_MIRROR)/gcc-$(GCC_DIST_BASEVER)/$(GCC_VENDOR_ARCHIVE)

$(GCC_METASRC)/extract_done: $(META_SOURCES)/$(GCC_VENDOR_ARCHIVE)
	rm -f $@
	cd $(META_SOURCES) && tar -xjf $(GCC_VENDOR_ARCHIVE)
	rm -rf $(GCC_METASRC)
	mv -f $(META_SOURCES)/gcc-$(GCC_DIST_BASEVER) $(GCC_METASRC)
	touch $@

$(GCC_METASRC)/patch_done: $(GCC_METASRC)/extract_done $(GCC_PATCH)
	rm -f $@
	cd $(GCC_METASRC) && patch -p1 <$$OLDPWD/$(GCC_PATCH)
	touch $@

$(ARCHIVEDIR)/$(GCC_DISTBASE).tar.bz2: $(GCC_METASRC)/patch_done
	rm -f $@.tmp $@
	$(MKDIR_P) $(ARCHIVEDIR)
	tar -cjvf $@.tmp -C $(META_SOURCES) $(GCC_DISTBASE)
	mv -f $@.tmp $@

gcc-dist: $(ARCHIVEDIR)/$(GCC_DISTBASE).tar.bz2

