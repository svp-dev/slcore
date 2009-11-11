# -*- makefile -*-

TX_SRC = \
   $(SOURCES)/tx-$(TX_VERSION)/configure

TX_TARGETS = \
   $(REQDIR)/lib/libtinyxml.a

$(SOURCES)/tx-$(TX_VERSION)/$(TX_ARCHIVE):
	rm -f $@
	mkdir -p $(SOURCES)/tx-$(TX_VERSION)
	(cd $(SOURCES)/tx-$(TX_VERSION) && $(FETCH) $(TX_MIRROR)/$(TX_ARCHIVE))

$(SOURCES)/tx-$(TX_VERSION)/extract_done: $(SOURCES)/tx-$(TX_VERSION)/$(TX_ARCHIVE)
	rm -f $@
	(cd $(SOURCES)/tx-$(TX_VERSION) && $(UNZIP) $(TX_ARCHIVE) && touch extract_done)

$(SOURCES)/tx-$(TX_VERSION)/configure.ac: tinyxml/configure.ac
	cp -f $< $@

$(SOURCES)/tx-$(TX_VERSION)/Makefile.am: tinyxml/Makefile.am
	cp -f $< $@

$(SOURCES)/tx-$(TX_VERSION)/bootstrap: tinyxml/bootstrap
	cp -f $< $@

$(SOURCES)/tx-$(TX_VERSION)/configure: $(SOURCES)/tx-$(TX_VERSION)/extract_done \
	$(SOURCES)/tx-$(TX_VERSION)/Makefile.am \
	$(SOURCES)/tx-$(TX_VERSION)/bootstrap \
	$(SOURCES)/tx-$(TX_VERSION)/configure.ac
	(cd $(SOURCES)/tx-$(TX_VERSION) && ./bootstrap)

$(REQDIR)/lib/libtinyxml.a: $(SOURCES)/tx-$(TX_VERSION)/configure
	mkdir -p $(BUILD)/tx-$(TX_VERSION)
	(SRC=$$(cd $(SOURCES)/tx-$(TX_VERSION); pwd); \
	  cd $(BUILD)/tx-$(TX_VERSION) && \
	  $$SRC/configure --prefix=$(REQDIR) \
	                  CFLAGS="$$CFLAGS $(EXTRA_CFLAGS)" \
	                  LDFLAGS="$$LDFLAGS $(EXTRA_LDFLAGS)" \
	    && $(MAKE) $(MAKE_FLAGS) && $(MAKE) install)

