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
	(cd $(SOURCES)/m4-dev && ./bootstrap && touch bootstrap_done)

$(REQDIR)/bin/m4: $(SOURCES)/m4-dev/bootstrap_done
	mkdir -p $(BUILD)/m4-dev
	(SRC=$$PWD/$(SOURCES)/m4-dev; \
	  cd $(BUILD)/m4-dev && \
	  $$SRC/configure --prefix=$(REQDIR) && \
	  $(MAKE) && \
	  $(MAKE) install)
