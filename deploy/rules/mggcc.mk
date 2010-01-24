# -*- makefile -*-

MGGCC_SRC = $(SOURCES)/mggcc-$(MGGCC_VERSION)
MGGCC_BUILD = $(BUILD)/mggcc-$(MGGCC_VERSION)

MGGCC_TARGETS = mtalpha-linux-gnu

MGGCC_CFG_TARGETS = $(foreach T,$(MGGCC_TARGETS),$(MGGCC_BUILD)-$(T)/configure_done)
MGGCC_BUILD_TARGETS = $(foreach T,$(MGGCC_TARGETS),$(MGGCC_BUILD)-$(T)/build_done)
MGGCC_INST_TARGETS = $(foreach T,$(MGGCC_TARGETS),$(REQDIR)/bin/$(T)-gcc)

GCC_CONFIG_FLAGS = \
   --disable-bootstrap --disable-libmudflap --disable-libssp \
   --disable-coverage --enable-gdb --disable-threads --disable-nls \
   --disable-multilib --enable-languages=c 

.PRECIOUS: $(MGGCC_CFG_TARGETS) $(MGGCC_BUILD_TARGETS) $(MGGCC_INST_TARGETS)

mggcc-configure: $(MGGCC_CFG_TARGETS)
mggcc-build: $(MGGCC_BUILD_TARGETS)
mggcc-install: $(MGGCC_INST_TARGETS)

$(MGGCC_SRC)/configure: $(MGGCC_ARCHIVE)
	rm -f $@
	mkdir -p $(SOURCES)
	tar -C $(SOURCES) -xjvf $(MGGCC_ARCHIVE)
	touch $@

$(MGGCC_BUILD)-%/configure_done: $(MGGCC_SRC)/configure $(REQDIR)/.binutils-installed-%
	rm -f $@
	mkdir -p $(MGGCC_BUILD)-$*
	SRC=$$(cd $(MGGCC_SRC) && pwd) && \
           cd $(MGGCC_BUILD)-$* && \
	   find . -name config.cache -exec rm '{}' \; && \
			  CFLAGS="$$CFLAGS $(EXTRA_CFLAGS)" \
	                  LDFLAGS="$$LDFLAGS $(EXTRA_LDFLAGS)" \
	   $$SRC/configure --target=$* \
			       --prefix=$(REQDIR) \
	                       $(GCC_CONFIG_FLAGS)
	touch $@

$(MGGCC_BUILD)-%/build_done: $(MGGCC_BUILD)-%/configure_done
	rm -f $@
	cd $(MGGCC_BUILD)-$* && $(MAKE) $(MAKE_FLAGS)
	touch $@

$(REQDIR)/bin/%-gcc: $(MGGCC_BUILD)-%/build_done
	cd $(MGGCC_BUILD)-$* && $(MAKE) -j1 install
	touch $@
