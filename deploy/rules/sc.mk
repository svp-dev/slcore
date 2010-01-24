# -*- makefile -*-

SC_SRC = $(SOURCES)/systemc-$(SC_VERSION)
SC_BUILD = $(BUILD)/systemc-$(SC_VERSION)
SC_CFG_TARGETS = $(SC_BUILD)/configure_done
SC_BUILD_TARGETS = $(SC_BUILD)/build_done
SC_INST_TARGETS = $(REQDIR)/.systemc-installed

.PRECIOUS: $(SC_CFG_TARGETS) $(SC_BUILD_TARGETS) $(SC_INST_TARGETS)

sc-configure: $(SC_CFG_TARGETS)
sc-build: $(SC_BUILD_TARGETS)
sc-install: $(SC_INST_TARGETS)

# used by mggcc
sc-flags:
	@SRC=`cd $(SC_SRC) && pwd` && echo CPPFLAGS=\"-I$$SRC/src $(EXTRA_CFLAGS)\" CFLAGS=\"-I$$SRC/src $(EXTRA_CFLAGS)\"
	@BLD=`cd $(BUILD)/systemc-$(SC_VERSION) && pwd` && echo LDFLAGS=\"-L$$BLD/src $(EXTRA_LDFLAGS)\"

$(SC_SRC)/configure: $(SC_ARCHIVE)
	rm -f $@
	mkdir -p $(SOURCES)
	tar -C $(SOURCES) -xjvf $(SC_ARCHIVE)
	touch $@

# sysc configure needs $(REQDIR) to already exist
$(SC_BUILD)/configure_done: $(SC_SRC)/configure $(REQTAG)
	rm -f $@
	mkdir -p $(SC_BUILD)
	SRC=$$(cd $(SC_SRC) && pwd) && \
	  cd $(SC_BUILD) && \
	  find . -name config.cache -exec rm '{}' \; && \
	  $$SRC/configure --prefix=$(REQDIR) \
	                  CFLAGS="$$CFLAGS $(EXTRA_CFLAGS)" \
	                  LDFLAGS="$$LDFLAGS $(EXTRA_LDFLAGS)"
	touch $@

$(SC_BUILD)/build_done: $(SC_BUILD)/configure_done
	rm -f $@
	cd $(SC_BUILD) && \
	   if ! $(MAKE) $(MAKE_FLAGS) pthreads; then \
                $(MAKE) clean; \
	        $(MAKE) $(MAKE_FLAGS); \
           fi
	touch $@

$(REQDIR)/.systemc-installed: $(SC_BUILD)/build_done
	rm -f $@
	cd $(SC_BUILD) && $(MAKE) install
	touch $@

