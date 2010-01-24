# -*- makefile -*-

BINUTILS_SRC = \
   $(SOURCES)/binutils-$(BINUTILS_VERSION)

BINUTILS_TARGETS = alpha-linux-gnu mtalpha-linux-gnu sparc-leon-linux mtsparc-leon-linux

BINUTILS_CFG_TARGETS = $(foreach T,$(BINUTILS_TARGETS),$(BUILD)/binutils-$(BINUTILS_VERSION)-$(T)/configure_done)
BINUTILS_BUILD_TARGETS = $(foreach T,$(BINUTILS_TARGETS),$(BUILD)/binutils-$(BINUTILS_VERSION)-$(T)/gas/as-new)
BINUTILS_INST_TARGETS = $(foreach T,$(BINUTILS_TARGETS),$(REQDIR)/bin/$(T)-as)

binutils-configure: $(BINUTILS_CFG_TARGETS)
binutils-build: $(BINUTILS_BUILD_TARGETS)
binutils-install: $(BINUTILS_INST_TARGETS)

$(BINUTILS_SRC)/configure: $(BINUTILS_ARCHIVE)
	rm -f $@
	mkdir -p $(SOURCES)
	tar -C $(SOURCES) -xjvf $(BINUTILS_ARCHIVE)
	touch $@

$(BUILD)/binutils-$(BINUTILS_VERSION)-%/configure_done: $(BINUTILS_SRC)/configure
	rm -f $@
	mkdir -p $(BUILD)/binutils-$(BINUTILS_VERSION)-$*
	SRC=$$(cd $(BINUTILS_SRC) && pwd) && \
           cd $(BUILD)/binutils-$(BINUTILS_VERSION)-$* && \
	   $$SRC/configure --target=$* \
			  CFLAGS="$$CFLAGS $(EXTRA_CFLAGS)" \
	                  LDFLAGS="$$LDFLAGS $(EXTRA_LDFLAGS)" \
			   --disable-werror \
	                   --prefix=$(REQDIR)
	touch $@

$(BUILD)/binutils-$(BINUTILS_VERSION)-%/gas/as-new: $(BUILD)/binutils-$(BINUTILS_VERSION)-%/configure_done
	cd $(BUILD)/binutils-$(BINUTILS_VERSION)-$* && $(MAKE) $(MAKE_FLAGS)

$(REQDIR)/bin/%-as: $(BUILD)/binutils-$(BINUTILS_VERSION)-%/gas/as-new
	cd $(BUILD)/binutils-$(BINUTILS_VERSION)-$* && $(MAKE) -j1 install

