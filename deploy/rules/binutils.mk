# -*- makefile -*-

BINUTILS_SRC = $(SOURCES)/binutils-$(BINUTILS_VERSION)
BINUTILS_BUILD = $(BUILD)/binutils-$(BINUTILS_VERSION)
BINUTILS_TARGETS = alpha-linux-gnu mtalpha-linux-gnu sparc-leon-linux mtsparc-leon-linux

BINUTILS_CFG_TARGETS = $(foreach T,$(BINUTILS_TARGETS),$(BINUTILS_BUILD)-$(T)/configure_done)
BINUTILS_BUILD_TARGETS = $(foreach T,$(BINUTILS_TARGETS),$(BINUTILS_BUILD)-$(T)/build_done)
BINUTILS_INST_TARGETS = $(foreach T,$(BINUTILS_TARGETS),$(REQDIR)/.binutils-installed-$(T))

.PRECIOUS: $(BINUTILS_CFG_TARGETS) $(BINUTILS_BUILD_TARGETS) $(BINUTILS_INST_TARGETS)

binutils-configure: $(BINUTILS_CFG_TARGETS)
binutils-build: $(BINUTILS_BUILD_TARGETS)
binutils-install: $(BINUTILS_INST_TARGETS)

$(BINUTILS_SRC)/configure: $(BINUTILS_ARCHIVE)
	rm -f $@
	mkdir -p $(SOURCES)
	tar -C $(SOURCES) -xjvf $(BINUTILS_ARCHIVE)
	touch $@

$(BINUTILS_BUILD)-%/configure_done: $(BINUTILS_SRC)/configure $(REQTAG)
	rm -f $@
	mkdir -p $(BINUTILS_BUILD)-$*
	SRC=$$(cd $(BINUTILS_SRC) && pwd) && \
           cd $(BINUTILS_BUILD)-$* && \
	   find . -name config.cache -exec rm '{}' \; && \
	   $$SRC/configure --target=$* \
			  CFLAGS="$$CFLAGS $(EXTRA_CFLAGS)" \
	                  LDFLAGS="$$LDFLAGS $(EXTRA_LDFLAGS)" \
			   --disable-werror \
	                   --prefix=$(REQDIR)
	touch $@

$(BINUTILS_BUILD)-%/build_done: $(BINUTILS_BUILD)-%/configure_done
	rm -f $@
	cd $(BINUTILS_BUILD)-$* && $(MAKE) $(MAKE_FLAGS)
	touch $@

$(REQDIR)/.binutils-installed-%: $(BINUTILS_BUILD)-%/build_done
	rm -f $@
	cd $(BINUTILS_BUILD)-$* && $(MAKE) -j1 install
	touch $@
