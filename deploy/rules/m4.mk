# -*- makefile -*-

M4_SRC = \
  $(SOURCES)/m4-dev/bootstrap_done

M4_TARGETS = \
     $(REQDIR)/bin/m4

$(SOURCES)/m4-dev/download_done:
	rm -f $@
	mkdir -p $(SOURCES)
	(cd $(SOURCES) && \
	  git clone $(M4_REPO) m4-dev && \
	  cd m4-dev && git checkout -b branch-$(M4_VERSION) origin/branch-$(M4_VERSION) && \
	  touch download_done)

$(SOURCES)/m4-dev/patch_done: $(SOURCES)/m4-dev/download_done
	rm -f $@
	(cd $(SOURCES)/m4-dev && \
	  patch -p1 <$$OLDPWD/patches/m4.patch && \
	  touch patch_done)

$(SOURCES)/m4-dev/bootstrap_done: $(SOURCES)/m4-dev/patch_done
	rm -f $@
	(cd $(SOURCES)/m4-dev && bash -e ./bootstrap && touch bootstrap_done)

$(REQDIR)/bin/m4: $(SOURCES)/m4-dev/bootstrap_done
	mkdir -p $(BUILD)/m4-dev
	(SRC=$$(cd $(SOURCES)/m4-dev; pwd); \
	  cd $(BUILD)/m4-dev && \
	  $$SRC/configure --prefix=$(REQDIR) \
			  CFLAGS="$$CFLAGS $(EXTRA_CFLAGS)" \
	                  LDFLAGS="$$LDFLAGS $(EXTRA_LDFLAGS)" \
	     && \
	  if ! $(MAKE) $(MAKE_FLAGS); then \
	     rm -rf * && POSIXLY_CORRECT=1 $$SRC/configure --prefix=$(REQDIR) \
			  CFLAGS="$$CFLAGS $(EXTRA_CFLAGS)" \
	                  LDFLAGS="$$LDFLAGS $(EXTRA_LDFLAGS)" \
	      && \
	     if ! $(MAKE) $(MAKE_FLAGS); then \
	        $(MAKE) clean all $(MAKE_FLAGS) CPPFLAGS="-U__GNUC__ -D__inline__=__attribute__((__weak__)) -D__inline=__inline__ -Dinline=__inline__"; \
	     fi; \
	  fi && \
	  $(MAKE) install)
