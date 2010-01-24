##
## Binutils dist
## 

include common.mk
include dist-common.mk

M4_GIT_BRANCH = branch-1.6

M4_TIMESTAMP := $(strip $(shell date +%Y%m%d))
M4_DISTBASE = m4-$(M4_TIMESTAMP)
M4_METASRC = $(META_SOURCES)/$(M4_DISTBASE)

$(M4_METASRC)/download_done:
	rm -f $@
	mkdir -p $(META_SOURCES)
	cd $(META_SOURCES) && git clone $(M4_REPO) $(M4_DISTBASE)
	cd $(M4_METASRC) && git checkout -b $(M4_GIT_BRANCH) origin/$(M4_GIT_BRANCH)
	touch $@

$(M4_METASRC)/patch_done: $(M4_METASRC)/download_done
	rm -f $@
	cd $(M4_METASRC) && patch -p1 <$$OLDPWD/patches/m4.patch
	touch $@

$(M4_METASRC)/bootstrap_done: $(M4_METASRC)/patch_done
	rm -f $@
	cd $(M4_METASRC) && bash -e ./bootstrap
	touch $@

$(M4_METASRC)/configure_done: $(M4_METASRC)/bootstrap_done
	rm -f $@
	cd $(M4_METASRC) && ./configure
	touch $@

$(M4_METASRC)/build1_done: $(M4_METASRC)/configure_done
	rm -f $@
	cd $(M4_METASRC) && $(MAKE)
	touch $@

.PHONY: m4-dist
m4-dist: $(M4_METASRC)/build1_done
	mkdir -p $(DISTDIR)
	rm -f $(M4_METASRC)/*.tar.*
	cd $(M4_METASRC) && make dist
	for arch in $(M4_METASRC)/*.tar.bz2; do \
	   bn=`basename $$arch`; \
	   if ! test -f $(DISTDIR)/$$bn; then \
	      mv -f $$arch $(DISTDIR)/; \
	   fi; \
	done

