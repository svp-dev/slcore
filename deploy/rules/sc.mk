# -*- makefile -*-

SC_SRC = \
   $(SOURCES)/systemc-$(SC_VERSION)

SC_CFG_TARGETS = \
	$(BUILD)/systemc-$(SC_VERSION)/configure_done

SC_BUILD_TARGETS = \
	$(BUILD)/systemc-$(SC_VERSION)/src/libsystemc.a

SC_INST_TARGETS = \
   $(REQDIR)/include/systemc

sc-configure: $(SC_CFG_TARGETS)
sc-build: $(SC_BUILD_TARGETS)
sc-install: $(SC_INST_TARGETS)

$(SC_SRC)/configure: $(SC_ARCHIVE)
	rm -f $@
	mkdir -p $(SOURCES)
	tar -C $(SOURCES) -xjvf $(SC_ARCHIVE)
	touch $@

$(BUILD)/systemc-$(SC_VERSION)/configure_done: $(SC_SRC)/configure
	mkdir -p $(REQDIR) # sysc configure needs $(REQDIR) to already exist
	mkdir -p $(BUILD)/systemc-$(SC_VERSION)
	rm -f $@
	SRC=$$(cd $(SC_SRC) && pwd) && \
	  cd $(BUILD)/systemc-$(SC_VERSION) && \
	  $$SRC/configure --prefix=$(REQDIR) \
	                  CFLAGS="$$CFLAGS $(EXTRA_CFLAGS)" \
	                  LDFLAGS="$$LDFLAGS $(EXTRA_LDFLAGS)"
	touch $@

$(BUILD)/systemc-$(SC_VERSION)/src/libsystemc.a: $(BUILD)/systemc-$(SC_VERSION)/configure_done
	cd $(BUILD)/systemc-$(SC_VERSION) && \
	   if ! $(MAKE) $(MAKE_FLAGS) pthreads; then \
                $(MAKE) clean; \
	        $(MAKE) $(MAKE_FLAGS); \
           fi

$(REQDIR)/include/systemc: $(BUILD)/systemc-$(SC_VERSION)/src/libsystemc.a
	cd $(BUILD)/systemc-$(SC_VERSION) && $(MAKE) install


