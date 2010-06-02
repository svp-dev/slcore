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

MGSIM_PROGRAMS = mgsim-alpha simx-alpha mgsim-sparc

$(MGSIM_METASRC)/download_done:
	rm -f $@
	$(MKDIR_P) $(META_SOURCES)
	cd $(META_SOURCES) && $(SVN) checkout -r$(MGSIM_SVN_REV) $(MGSIM_REPO)/$(MGSIM_DIST_SVNBRANCH) $(MGSIM_DISTBASE)
	touch $@

$(MGSIM_METASRC)/bootstrap_done: $(MGSIM_METASRC)/download_done
	rm -f $@
	cd $(MGSIM_METASRC) && bash -e ./bootstrap
	touch $@

# rather hackish: in order to make an archive for mgsim,
# we need to generate the man pages, which in turn require
# to build the programs, which in turn require a properly 
# built systemc. This needs to be present on the system already!
$(MGSIM_METASRC)/configure_done: $(MGSIM_METASRC)/bootstrap_done
	rm -f $@
	cd $(MGSIM_METASRC) && ./configure CPPFLAGS="$$CPPFLAGS" LDFLAGS="$$LDFLAGS"
	if grep -q 'WARNING:.*COMA simulation not available' $(MGSIM_METASRC)/config.log; then \
	  echo; echo "*** COMA not enabled: SystemC missing? ***"; echo; \
	  exit 1; \
	fi
	touch $@

$(MGSIM_METASRC)/build1_done: $(MGSIM_METASRC)/configure_done
	rm -f $@
	cd $(MGSIM_METASRC) && $(MAKE) .version && $(MAKE) -C src $(MGSIM_PROGRAMS)
	touch $@

.PHONY: mgsim-dist
mgsim-dist: $(MGSIM_METASRC)/build1_done
	$(MKDIR_P) $(ARCHIVEDIR)
	rm -f $(MGSIM_METASRC)/*.tar.*
	cd $(MGSIM_METASRC) && make dist
	for arch in $(MGSIM_METASRC)/*.tar.bz2; do \
	   bn=`basename $$arch`; \
	   if ! test -f $(ARCHIVEDIR)/$$bn; then \
	      mv -f $$arch $(ARCHIVEDIR)/; \
	   fi; \
	done

