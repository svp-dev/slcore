# -*- makefile -*-

BINUTILS_SRC = \
   $(SOURCES)/binutils-$(BINUTILS_VERSION)

BINUTILS_TARGETS = alpha-linux-gnu mtalpha-linux-gnu sparc-leon-linux mtsparc-leon-linux

BINUTILS_BUILD_TARGETS = $(foreach T,$(BINUTILS_TARGETS),$(BUILD)/binutils-$(BINUTILS_VERSION)-$(T)/gas/as-new)
BINUTILS_INST_TARGETS = $(foreach T,$(BINUTILS_TARGETS),$(REQDIR)/bin/$(T)-as)

binutils-build: $(BINUTILS_BUILD_TARGETS)
binutils: $(BINUTILS_INST_TARGETS)

$(BINUTILS_SRC)/configure: $(BINUTILS_ARCHIVE) $(SOURCES)
	rm -f $@
	tar -C $(SOURCES) -xjvf $(BINUTILS_ARCHIVE)
	touch $@

$(BUILD)/binutils-$(BINUTILS_VERSION)-%/gas/as-new: $(BINUTILS_SRC)/configure
	mkdir -p $(BUILD)/binutils-$(BINUTILS_VERSION)-$*
	(SRC=$$(cd $(BINUTILS_SRC) && pwd); \
           cd $(BUILD)/binutils-$(BINUTILS_VERSION)-$* && \
	   $$SRC/configure --target=$* \
			  CFLAGS="$$CFLAGS $(EXTRA_CFLAGS)" \
	                  LDFLAGS="$$LDFLAGS $(EXTRA_LDFLAGS)" \
			   --disable-werror \
	                   --prefix=$(REQDIR) && \
	   $(MAKE) $(MAKE_FLAGS))

$(REQDIR)/bin/%-as: $(BUILD)/binutils-$(BINUTILS_VERSION)-%/gas/as-new
	cd $(BUILD)/binutils-$(BINUTILS_VERSION)-$* && $(MAKE) install

