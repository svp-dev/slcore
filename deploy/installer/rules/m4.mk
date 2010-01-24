# -*- makefile -*-

M4_SRC = $(SRCBASE)/m4-$(M4_VERSION)
M4_BUILD = $(BLDBASE)/m4-$(M4_VERSION)
M4_BUILD_TARGETS = $(M4_BUILD)/build_done
M4_INST_TARGETS = $(REQDIR)/.m4-installed

.PRECIOUS: $(M4_ARCHIVE) $(M4_BUILD_TARGETS) $(M4_INST_TARGETS)

m4-fetch: $(M4_ARCHIVE) ; $(RULE_DONE)
m4-configure:  ; $(RULE_DONE)
m4-build: $(M4_BUILD_TARGETS) ; $(RULE_DONE)
m4-install: $(M4_INST_TARGETS) ; $(RULE_DONE)

$(M4_SRC)/configure: $(M4_ARCHIVE)
	rm -f $@
	$(UNTAR) $(SRCBASE) $(M4_ARCHIVE)
	touch $@

$(M4_BUILD)/build_done: $(M4_SRC)/configure $(REQTAG)
	rm -f $@
	$(MKDIR_P) $(M4_BUILD)
	SRC=$$(cd $(M4_SRC) && pwd) && \
	  cd $(M4_BUILD) && \
	  find . -name config.cache -exec rm '{}' \; && \
	  $$SRC/configure --prefix=$(REQDIR) \
			  CC="$(CC)" \
			  CFLAGS="$(CPPFLAGS) $(CFLAGS)" \
	                  LDFLAGS="$(CFLAGS) $(LDFLAGS)" \
	     && \
	  if ! $(MAKE) ; then \
	     rm -rf * && POSIXLY_CORRECT=1 $$SRC/configure --prefix=$(REQDIR) \
			  CC="$(CC)" \
			  CFLAGS="$(CPPFLAGS) $(CFLAGS)" \
	                  LDFLAGS="$(CFLAGS) $(LDFLAGS)" \
	      && \
	     $(MAKE) ; \
	  fi
	touch $@

$(REQDIR)/.m4-installed: $(M4_BUILD)/build_done
	rm -f $@
	cd $(M4_BUILD) && $(MAKE) install
	touch $@
