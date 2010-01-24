# -*- makefile -*-

BINUTILS_SRC = $(SRCBASE)/binutils-$(BINUTILS_VERSION)
BINUTILS_BUILD = $(BLDBASE)/binutils-$(BINUTILS_VERSION)
BINUTILS_TARGETS = alpha-linux-gnu mtalpha-linux-gnu sparc-leon-linux mtsparc-leon-linux

BINUTILS_CFG_TARGETS = $(foreach T,$(BINUTILS_TARGETS),$(BINUTILS_BUILD)-$(T)/configure_done)
BINUTILS_BUILD_TARGETS = $(foreach T,$(BINUTILS_TARGETS),$(BINUTILS_BUILD)-$(T)/build_done)
BINUTILS_INST_TARGETS = $(foreach T,$(BINUTILS_TARGETS),$(REQDIR)/.binutils-installed-$(T))

.PRECIOUS: $(BINUTILS_ARCHIVE) $(BINUTILS_CFG_TARGETS) $(BINUTILS_BUILD_TARGETS) $(BINUTILS_INST_TARGETS)

binutils-fetch: $(BINUTILS_ARCHIVE) ; $(RULE_DONE)
binutils-configure: $(BINUTILS_CFG_TARGETS) ; $(RULE_DONE)
binutils-build: $(BINUTILS_BUILD_TARGETS) ; $(RULE_DONE)
binutils-install: $(BINUTILS_INST_TARGETS) ; $(RULE_DONE)

$(BINUTILS_SRC)/configure: $(BINUTILS_ARCHIVE)
	rm -f $@
	$(UNTAR) $(SRCBASE) $(BINUTILS_ARCHIVE)
	touch $@

$(BINUTILS_BUILD)-%/configure_done: $(BINUTILS_SRC)/configure $(REQTAG)
	rm -f $@
	$(MKDIR_P) $(BINUTILS_BUILD)-$*
	SRC=$$(cd $(BINUTILS_SRC) && pwd) && \
           cd $(BINUTILS_BUILD)-$* && \
	   find . -name config.cache -exec rm '{}' \; && \
	   $$SRC/configure --target=$* \
			  CC="$(CC)" \
	                  CFLAGS="$(CPPFLAGS) $(CFLAGS)" \
	                  LDFLAGS="$(CFLAGS) $(LDFLAGS)" \
			   --disable-werror \
	                   --prefix=$(REQDIR)
	touch $@

$(BINUTILS_BUILD)-%/build_done: $(BINUTILS_BUILD)-%/configure_done
	rm -f $@
	cd $(BINUTILS_BUILD)-$* && $(MAKE) 
	touch $@

$(REQDIR)/.binutils-installed-%: $(BINUTILS_BUILD)-%/build_done
	rm -f $@
	cd $(BINUTILS_BUILD)-$* && $(MAKE) -j1 install
	touch $@
