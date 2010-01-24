##
## MGSim dist
## 

include common.mk
include dist-common.mk

MGSIM_DIST_SVNBRANCH = trunk
MGSIM_DIST_REV = latest

MGSIM_BASEBRANCH := $(strip $(shell basename $(MGSIM_DIST_SVNBRANCH)))
MGSIM_SVN_REV := $(strip $(if $(filter latest, $(MGSIM_DIST_REV)), \
	          $(shell $(SVN) info $(MGSIM_REPO)/$(MGSIM_DIST_SVNBRANCH) | \
	                  grep 'Last Changed Rev' | \
	                  cut -d: -f2), $(MGSIM_DIST_REV)))

MGSIM_DIST_VERSION = $(MGSIM_SVN_REV)-$(MGSIM_BASEBRANCH)
MGSIM_DISTBASE = mgsim-$(MGSIM_DIST_VERSION)
MGSIM_METASRC = $(META_SOURCES)/$(MGSIM_DISTBASE)

$(MGSIM_METASRC)/download_done:
	rm -f $@
	mkdir -p $(META_SOURCES)
	cd $(META_SOURCES) && $(SVN) checkout -r$(MGSIM_SVN_REV) $(MGSIM_REPO)/$(MGSIM_DIST_SVNBRANCH) $(MGSIM_DISTBASE)
	touch $@

$(MGSIM_METASRC)/bootstrap_done: $(MGSIM_METASRC)/download_done
	rm -f $@
	cd $(MGSIM_METASRC) && bash -e ./bootstrap
	touch $@

# rather hackish: in order to make an archive for mgsim,
# we need to generate the man pages, which in turn require
# to build the programs, which in turn require a properly 
# built systemc. So we build just that first.
$(MGSIM_METASRC)/configure_done: $(MGSIM_METASRC)/bootstrap_done
	rm -f $@
	$(MAKE) sc-build
	eval `$(MAKE) sc-flags` && \
	   cd $(MGSIM_METASRC) && ./configure CPPFLAGS="$$CPPFLAGS" LDFLAGS="$$LDFLAGS"
	touch $@

$(MGSIM_METASRC)/build1_done: $(MGSIM_METASRC)/configure_done
	rm -f $@
	cd $(MGSIM_METASRC) && $(MAKE) all
	touch $@

.PHONY: mgsim-dist
mgsim-dist: $(MGSIM_METASRC)/build1_done
	mkdir -p $(DISTDIR)
	rm -f $(MGSIM_METASRC)/*.tar.*
	cd $(MGSIM_METASRC) && make dist
	for arch in $(MGSIM_METASRC)/*.tar.bz2; do \
	   bn=`basename $$arch`; \
	   if ! test -f $(DISTDIR)/$$bn; then \
	      mv -f $$arch $(DISTDIR)/; \
	   fi; \
	done

