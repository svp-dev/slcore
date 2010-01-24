# -*- makefile -*-

M4_SRC = $(SOURCES)/m4-$(M4_VERSION)
M4_BUILD = $(BUILD)/m4-$(M4_VERSION)
M4_BUILD_TARGETS = $(M4_BUILD)/build_done
M4_INST_TARGETS = $(REQDIR)/.m4-installed

.PRECIOUS: $(M4_BUILD_TARGETS) $(M4_INST_TARGETS)

m4-configure: 
m4-build: $(M4_BUILD_TARGETS)
m4-install: $(M4_INST_TARGETS)

$(M4_SRC)/configure: $(M4_ARCHIVE)
	rm -f $@
	mkdir -p $(SOURCES)
	tar -C $(SOURCES) -xjvf $(M4_ARCHIVE)
	touch $@

$(M4_BUILD)/build_done: $(M4_SRC)/configure $(REQTAG)
	rm -f $@
	mkdir -p $(M4_BUILD)
	SRC=$$(cd $(M4_SRC) && pwd) && \
	  cd $(M4_BUILD) && \
	  find . -name config.cache -exec rm '{}' \; && \
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
	touch $@

$(REQDIR)/.m4-installed: $(M4_BUILD)/build_done
	rm -f $@
	cd $(M4_BUILD) && $(MAKE) install
	touch $@
