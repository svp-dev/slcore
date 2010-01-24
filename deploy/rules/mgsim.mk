# -*- makefile -*-

MGSIM_SRC = $(SOURCES)/mgsim-$(MGSIM_VERSION)
MGSIM_BUILD = $(BUILD)/mgsim-$(MGSIM_VERSION)

MGSIM_CFG_TARGETS = $(MGSIM_BUILD)/configure_done
MGSIM_BUILD_TARGETS = $(MGSIM_BUILD)/build_done
MGSIM_INST_TARGETS = $(SLDIR)/.mgsim-installed

.PRECIOUS: $(MGSIM_CFG_TARGETS) $(MGSIM_BUILD_TARGETS) $(MGSIM_INST_TARGETS)

mgsim-configure: $(MGSIM_CFG_TARGETS)
mgsim-build: $(MGSIM_BUILD_TARGETS)
mgsim-install: $(MGSIM_INST_TARGETS)

$(MGSIM_SRC)/configure: $(MGSIM_ARCHIVE)
	rm -f $@
	mkdir -p $(SOURCES)
	tar -C $(SOURCES) -xjvf $(MGSIM_ARCHIVE)
	touch $@

$(MGSIM_BUILD)/configure_done: $(MGSIM_SRC)/configure $(SLTAG) $(SC_INST_TARGETS) $(BINUTILS_INST_TARGETS)
	rm -f $@
	mkdir -p $(MGSIM_BUILD)
	rm -f $(PREFIX)/slreqs-current
	ln -s $(REQDIR) $(PREFIX)/slreqs-current
	SRC=$$(cd $(MGSIM_SRC) && pwd) && \
	   cd $(MGSIM_BUILD) && \
	   PATH=$(PREFIX)/slreqs-current/bin:$$PATH $$SRC/configure --prefix=$(SLDIR) \
	                  CPPFLAGS="$$CPPFLAGS -I$(PREFIX)/slreqs-current/include $(EXTRA_CFLAGS)" \
			  CFLAGS="$$CFLAGS $(EXTRA_CFLAGS)" \
	                  LDFLAGS="$$LDFLAGS -L$(PREFIX)/slreqs-current/lib $(EXTRA_LDFLAGS)"
	rm -f $(MGSIM_BUILD)/src/*main.o
	touch $@

$(MGSIM_BUILD)/build_done: $(MGSIM_BUILD)/configure_done
	rm -f $@
	cd $(MGSIM_BUILD) && $(MAKE) all
	touch $@

$(SLDIR)/.mgsim-installed: $(MGSIM_BUILD)/build_done
	rm -f $@
	cd $(MGSIM_BUILD) && $(MAKE) install
	rm -f $(PREFIX)/sl-current
	ln -s $(SLDIR) $(PREFIX)/sl-current
	touch $@

