# -*- makefile -*-

SC_SRC = \
   $(SOURCES)/systemc-$(SC_VERSION)/bootstrap_done

SC_TARGETS = \
   $(REQDIR)/include/systemc

$(SOURCES)/systemc-$(SC_VERSION)/extract_done:
	rm -f $@
	mkdir -p $(SOURCES)
	(cd $(SOURCES) && $(TAR) -xzf $$OLDPWD/sysc/$(SC_ARCHIVE) \
	&& touch systemc-$(SC_VERSION)/extract_done)

$(SOURCES)/systemc-$(SC_VERSION)/patch_done: $(SOURCES)/systemc-$(SC_VERSION)/extract_done
	rm -f $@
	(cd $(SOURCES)/systemc-$(SC_VERSION) && \
	  patch -p1 <$$OLDPWD/patches/sysc.patch && \
	  touch patch_done)

$(SOURCES)/systemc-$(SC_VERSION)/bootstrap_done: $(SOURCES)/systemc-$(SC_VERSION)/patch_done
	rm -f $@
	(cd $(SOURCES)/systemc-$(SC_VERSION) && $$OLDPWD/sysc/bootstrap && touch bootstrap_done)

$(REQDIR)/include/systemc: $(SOURCES)/systemc-$(SC_VERSION)/bootstrap_done
	mkdir -p $(BUILD)/systemc-$(SC_VERSION)
	(SRC=$$(cd $(SOURCES)/systemc-$(SC_VERSION); pwd); \
	  cd $(BUILD)/systemc-$(SC_VERSION) && \
	  $$SRC/configure --prefix=$(REQDIR) \
	                  CFLAGS="$$CFLAGS $(EXTRA_CFLAGS)" \
	                  LDFLAGS="$$LDFLAGS $(EXTRA_LDFLAGS)" \
	    && if ! $(MAKE) $(MAKE_FLAGS); then \
                 $(MAKE) clean; $(MAKE) $(MAKE_FLAGS) pthreads; \
               fi && $(MAKE) install)

