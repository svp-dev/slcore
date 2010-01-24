# -*- makefile -*-

M4_SRC = \
  $(SOURCES)/m4-$(M4_VERSION)

M4_BUILD_TARGETS = \
     $(BUILD)/m4-$(M4_VERSION)/src/m4

M4_INST_TARGETS = \
     $(REQDIR)/bin/m4

m4-configure: 
m4-build: $(M4_BUILD_TARGETS)
m4-install: $(M4_INST_TARGETS)

$(M4_SRC)/configure: $(M4_ARCHIVE)
	rm -f $@
	mkdir -p $(SOURCES)
	tar -C $(SOURCES) -xjvf $(M4_ARCHIVE)
	touch $@

$(BUILD)/m4-$(M4_VERSION)/src/m4: $(M4_SRC)/configure
	mkdir -p $(BUILD)/m4-$(M4_VERSION)
	SRC=$$(cd $(M4_SRC) && pwd) && \
	  cd $(BUILD)/m4-$(M4_VERSION) && \
	  $$SRC/configure --prefix=$(REQDIR) \
			  CFLAGS="$$CFLAGS $(EXTRA_CFLAGS)" \
	                  LDFLAGS="$$LDFLAGS $(EXTRA_LDFLAGS)" \
	     && \
	  if ! $(MAKE) $(MAKE_FLAGS); then \
	     rm -rf * && POSIXLY_CORRECT=1 $$SRC/configure --prefix=$(REQDIR) \
			  CFLAGS="$$CFLAGS $(EXTRA_CFLAGS)" \
	                  LDFLAGS="$$LDFLAGS $(EXTRA_LDFLAGS)" \
	      && \
	     $(MAKE) $(MAKE_FLAGS); \
	  fi

$(REQDIR)/bin/m4: $(BUILD)/m4-$(M4_VERSION)/src/m4
	cd $(BUILD)/m4-$(M4_VERSION) && $(MAKE) install

