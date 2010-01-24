##
## MGSim dist
## 

include common.mk
include dist-common.mk

SLC_DIST_SVNBRANCH = trunk
SLC_DIST_REV = latest

SLC_BASEBRANCH := $(strip $(shell basename $(SLC_DIST_SVNBRANCH)))
SLC_SVN_REV := $(strip $(if $(filter latest, $(SLC_DIST_REV)), \
	          $(shell $(SVN) info $(SLC_REPO)/$(SLC_DIST_SVNBRANCH)/slc | \
	                  grep 'Last Changed Rev' | \
	                  cut -d: -f2), $(SLC_DIST_REV)))

SLC_DIST_VERSION = $(SLC_SVN_REV)-$(SLC_BASEBRANCH)
SLC_DISTBASE = slc-$(SLC_DIST_VERSION)
SLC_METASRC = $(META_SOURCES)/$(SLC_DISTBASE)

$(SLC_METASRC)/download_done:
	rm -f $@
	$(MKDIR_P) $(META_SOURCES)
	cd $(META_SOURCES) && $(SVN) checkout -r$(SLC_SVN_REV) $(SLC_REPO)/$(SLC_DIST_SVNBRANCH)/slc $(SLC_DISTBASE)
	touch $@

$(SLC_METASRC)/bootstrap_done: $(SLC_METASRC)/download_done
	rm -f $@
	cd $(SLC_METASRC) && bash -e ./bootstrap
	touch $@

$(SLC_METASRC)/configure_done: $(SLC_METASRC)/bootstrap_done
	rm -f $@
	cd $(SLC_METASRC) && ./configure
	touch $@

$(SLC_METASRC)/build1_done: $(SLC_METASRC)/configure_done
	rm -f $@
	cd $(SLC_METASRC) && $(MAKE) all
	touch $@

.PHONY: slc-dist
slc-dist: $(SLC_METASRC)/build1_done
	$(MKDIR_P) $(ARCHIVEDIR)
	rm -f $(SLC_METASRC)/*.tar.*
	cd $(SLC_METASRC) && make dist
	for arch in $(SLC_METASRC)/*.tar.bz2; do \
	   bn=`basename $$arch`; \
	   if ! test -f $(ARCHIVEDIR)/$$bn; then \
	      mv -f $$arch $(ARCHIVEDIR)/; \
	   fi; \
	done

