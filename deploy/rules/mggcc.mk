# -*- makefile -*-

MGGCC_SRC = \
  $(SOURCES)/mggcc-$(MGGCC_VERSION)

MGGCC_TARGETS = mtalpha-linux-gnu

MGGCC_CFG_TARGETS = $(foreach T,$(MGGCC_TARGETS),$(BUILD)/mggcc-$(MGGCC_VERSION)-$(T)/configure_done)
MGGCC_BUILD_TARGETS = $(foreach T,$(MGGCC_TARGETS),$(BUILD)/mggcc-$(MGGCC_VERSION)-$(T)/gcc/gcc-cross)
MGGCC_INST_TARGETS = $(foreach T,$(MGGCC_TARGETS),$(REQDIR)/bin/$(T)-gcc)

GCC_CONFIG_FLAGS = \
   --disable-bootstrap --disable-libmudflap --disable-libssp \
   --disable-coverage --enable-gdb --disable-threads --disable-nls \
   --disable-multilib --enable-languages=c 

mggcc-configure: $(MGGCC_CFG_TARGETS)
mggcc-build: $(MGGCC_BUILD_TARGETS)
mggcc-install: $(MGGCC_INST_TARGETS)

$(MGGCC_SRC)/configure: $(MGGCC_ARCHIVE)
	rm -f $@
	mkdir -p $(SOURCES)
	tar -C $(SOURCES) -xjvf $(MGGCC_ARCHIVE)
	touch $@

$(BUILD)/mggcc-$(MGGCC_VERSION)-%/configure_done: $(MGGCC_SRC)/configure $(REQDIR)/bin/%-as
	rm -f $@
	mkdir -p $(BUILD)/mggcc-$(MGGCC_VERSION)-$*
	SRC=$$(cd $(SOURCES)/mggcc-$(MGGCC_VERSION) && pwd) && \
           cd $(BUILD)/mggcc-$(MGGCC_VERSION)-$* && \
			  CFLAGS="$$CFLAGS $(EXTRA_CFLAGS)" \
	                  LDFLAGS="$$LDFLAGS $(EXTRA_LDFLAGS)" \
	   $$SRC/configure --target=$* \
			       --prefix=$(REQDIR) \
	                       $(GCC_CONFIG_FLAGS)
	touch $@

$(BUILD)/mggcc-$(MGGCC_VERSION)-%/gcc/gcc-cross: $(BUILD)/mggcc-$(MGGCC_VERSION)-%/configure_done
	cd $(BUILD)/mggcc-$(MGGCC_VERSION)-$* && $(MAKE) $(MAKE_FLAGS)

$(REQDIR)/bin/%-gcc: $(BUILD)/mggcc-$(MGGCC_VERSION)-%/gcc/gcc-cross
	cd $(BUILD)/mggcc-$(MGGCC_VERSION)-$* && $(MAKE) -j1 install

