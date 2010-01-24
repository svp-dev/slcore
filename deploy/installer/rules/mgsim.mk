# -*- makefile -*-

MGSIM_SRC = $(SRCBASE)/mgsim-$(MGSIM_VERSION)
MGSIM_BUILD = $(BLDBASE)/mgsim-$(MGSIM_VERSION)

MGSIM_CFG_TARGETS = $(MGSIM_BUILD)/configure_done
MGSIM_BUILD_TARGETS = $(MGSIM_BUILD)/build_done
MGSIM_INST_TARGETS = $(SLDIR)/.mgsim-installed

.PRECIOUS: $(MGSIM_ARCHIVE) $(MGSIM_CFG_TARGETS) $(MGSIM_BUILD_TARGETS) $(MGSIM_INST_TARGETS)

mgsim-fetch: $(MGSIM_ARCHIVE) ; $(RULE_DONE)
mgsim-configure: $(MGSIM_CFG_TARGETS) ; $(RULE_DONE)
mgsim-build: $(MGSIM_BUILD_TARGETS) ; $(RULE_DONE)
mgsim-install: $(MGSIM_INST_TARGETS) ; $(RULE_DONE)

$(MGSIM_SRC)/configure: $(MGSIM_ARCHIVE)
	rm -f $@
	$(UNTAR) $(SRCBASE) $(MGSIM_ARCHIVE)
	touch $@

$(MGSIM_BUILD)/configure_done: $(MGSIM_SRC)/configure $(SLTAG) $(SC_INST_TARGETS) $(BINUTILS_INST_TARGETS)
	rm -f $@
	$(MKDIR_P) $(MGSIM_BUILD)
	cd $(DSTBASE) && rm -f slreqs-current && $(LN_S) $(REQNAME) slreqs-current
	SRC=$$(cd $(MGSIM_SRC) && pwd) && \
	   cd $(MGSIM_BUILD) && \
	   PATH=$(REQCURRENT)/bin:$$PATH $$SRC/configure --prefix=$(SLDIR) \
			  CC="$(CC)" CXX="$(CXX)" \
	                  CPPFLAGS="-I$(REQCURRENT)/include $(CPPFLAGS)" \
			  CFLAGS="$(CFLAGS)" CXXFLAGS="$(CXXFLAGS)" \
	                  LDFLAGS="-L$(REQCURRENT)/lib $(LDFLAGS)"
	rm -f $(MGSIM_BUILD)/src/*main.o
	touch $@

$(MGSIM_BUILD)/build_done: $(MGSIM_BUILD)/configure_done
	rm -f $@
	cd $(MGSIM_BUILD) && $(MAKE) all
	touch $@

$(SLDIR)/.mgsim-installed: $(MGSIM_BUILD)/build_done
	rm -f $@
	cd $(MGSIM_BUILD) && $(MAKE) install
	cd $(DSTBASE) && rm -f sl-current && $(LN_S) $(SLNAME) sl-current
	touch $@

