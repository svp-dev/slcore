# -*- makefile -*-

SLC_SRC = $(SRCBASE)/sl-core-$(SLC_VERSION)
SLC_BUILD = $(BLDBASE)/sl-core-$(SLC_VERSION)

SLC_CFG_TARGETS = $(SLC_BUILD)/configure_done
SLC_BUILD_TARGETS = $(SLC_BUILD)/build_done
SLC_INST_TARGETS = $(SLDIR)/.slc-installed

.PRECIOUS: $(SLC_ARCHIVE) $(SLC_CFG_TARGETS) $(SLC_BUILD_TARGETS) $(SLC_INST_TARGETS)

slc-fetch: $(SLC_ARCHIVE) ; $(RULE_DONE)
slc-configure: $(SLC_CFG_TARGETS) ; $(RULE_DONE)
slc-build: $(SLC_BUILD_TARGETS) ; $(RULE_DONE)
slc-install: $(SLC_INST_TARGETS) ; $(RULE_DONE)

$(SLC_SRC)/configure: $(SLC_ARCHIVE)
	rm -f $@
	$(UNTAR) $(SRCBASE) $(SLC_ARCHIVE)
	touch $@

$(SLC_BUILD)/configure_done: $(SLC_SRC)/configure \
		$(BINUTILS_INST_TARGETS) \
		$(GCC_INST_TARGETS) \
		$(MGGCC_INST_TARGETS) \
		$(M4_INST_TARGETS) \
		$(MGSIM_INST_TARGETS)
	rm -f $@
	$(MKDIR_P) $(SLC_BUILD)
	SRC=$$(cd $(SLC_SRC) && pwd) && \
	   cd $(SLC_BUILD) && \
	   PATH=$(SLCURRENT)/bin:$(REQCURRENT)/bin:$$PATH \
	     $$SRC/configure --prefix=$(SLDIR) \
			  CC="$(CC)" CXX="$(CXX)" \
	                  CPPFLAGS="-I$(REQCURRENT)/include $(CPPFLAGS)" \
			  CFLAGS="$(CFLAGS)" CXXFLAGS="$(CXXFLAGS)" \
	                  LDFLAGS="-L$(REQCURRENT)/lib $(LDFLAGS)"
	touch $@

$(SLC_BUILD)/build_done: $(SLC_BUILD)/configure_done
	rm -f $@
	cd $(SLC_BUILD) && $(MAKE) all
	touch $@

$(SLDIR)/.slc-installed: $(SLC_BUILD)/build_done
	rm -f $@
	cd $(SLC_BUILD) && $(MAKE) install
	touch $@
