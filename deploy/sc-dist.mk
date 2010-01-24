##
## Binutils dist
## 

include common.mk
include dist-common.mk

SC_DIST_REV = latest
SC_DIST_BASEVER = 2.1.v1

SC_VENDOR_ARCHIVE = sysc/systemc-$(SC_DIST_BASEVER).tgz
SC_PATCH = patches/sysc.patch
SC_BOOTSTRAP = sysc/bootstrap

SC_SVN_REV := $(strip $(if $(filter latest, $(SC_DIST_REV)), \
	          $(shell $(SVN) info $(DEPLOY_REPO)/$(SC_PATCH) | \
	                  grep 'Last Changed Rev' | \
	                  cut -d: -f2), $(SC_DIST_REV)))


SC_DIST_VERSION = $(SC_DIST_BASEVER)-$(SC_SVN_REV)
SC_DISTBASE = systemc-$(SC_DIST_VERSION)
SC_METASRC = $(META_SOURCES)/$(SC_DISTBASE)

$(SC_METASRC)/extract_done:
	rm -f $@
	mkdir -p $(META_SOURCES)
	tar -C $(META_SOURCES) -xzf $(SC_VENDOR_ARCHIVE)
	rm -rf $(SC_METASRC)
	mv -f $(META_SOURCES)/systemc-$(SC_DIST_BASEVER) $(SC_METASRC)
	touch $@

$(SC_METASRC)/patch_done: $(SC_METASRC)/extract_done $(SC_PATCH)
	rm -f $@
	cd $(SC_METASRC) && patch -p1 <$$OLDPWD/$(SC_PATCH)
	touch $@

$(SC_METASRC)/bootstrap_done: $(SC_METASRC)/patch_done $(SC_BOOTSTRAP)
	rm -f $@
	cd $(SC_METASRC) && bash -e $$OLDPWD/$(SC_BOOTSTRAP)
	touch $@

$(DISTDIR)/$(SC_DISTBASE).tar.bz2: $(SC_METASRC)/bootstrap_done
	rm -f $@.tmp $@
	mkdir -p $(DISTDIR)
	tar -cjvf $@.tmp -C $(META_SOURCES) $(SC_DISTBASE)
	mv -f $@.tmp $@

sc-dist: $(DISTDIR)/$(SC_DISTBASE).tar.bz2

