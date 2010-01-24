# -*- makefile -*-

SLC_SRC = $(SOURCES)/sl-core-$(SLC_VERSION)
SLC_BUILD = $(BUILD)/sl-core-$(SLC_VERSION)

SLC_CFG_TARGETS = $(SLC_BUILD)/configure_done
SLC_BUILD_TARGETS = $(SLC_BUILD)/build_done
SLC_INST_TARGETS = $(SLDIR)/.slc-installed

.PRECIOUS: $(SLC_CFG_TARGETS) $(SLC_BUILD_TARGETS) $(SLC_INST_TARGETS)

slc-configure: $(SLC_CFG_TARGETS)
slc-build: $(SLC_BUILD_TARGETS)
slc-install: $(SLC_INST_TARGETS)

$(SLC_SRC)/configure: $(SLC_ARCHIVE)
	rm -f $@
	mkdir -p $(SOURCES)
	tar -C $(SOURCES) -xjvf $(SLC_ARCHIVE)
	touch $@

$(SLC_BUILD)/configure_done: $(SLC_SRC)/configure \
		$(BINUTILS_INST_TARGETS) \
		$(GCC_INST_TARGETS) \
		$(MGGCC_INST_TARGETS) \
		$(M4_INST_TARGETS) \
		$(MGSIM_INST_TARGETS)
	rm -f $@
	mkdir -p $(SLC_BUILD)
	SRC=$$(cd $(SLC_SRC) && pwd) && \
	   cd $(SLC_BUILD) && \
	   PATH=$(PREFIX)/sl-current/bin:$(PREFIX)/slreqs-current/bin:$$PATH \
	     $$SRC/configure --prefix=$(SLDIR) \
	                  CPPFLAGS="$$CPPFLAGS -I$(PREFIX)/slreqs-current/include $(EXTRA_CFLAGS)" \
			  CFLAGS="$$CFLAGS $(EXTRA_CFLAGS)" \
	                  LDFLAGS="$$LDFLAGS -L$(PREFIX)/slreqs-current/lib $(EXTRA_LDFLAGS)"
	touch $@

$(SLC_BUILD)/build_done: $(SLC_BUILD)/configure_done
	rm -f $@
	cd $(SLC_BUILD) && $(MAKE) all
	touch $@

$(SLDIR)/.slc-installed: $(SLC_BUILD)/build_done
	rm -f $@
	cd $(SLC_BUILD) && $(MAKE) install
	touch $@
