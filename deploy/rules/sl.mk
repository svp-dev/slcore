# -*- makefile -*-

SL_TARGETS = \
     $(SLDIR)/bin/slc

$(SOURCES)/slc-$(SLC_REV)/configure:
	mkdir -p $(SOURCES)
	(cd $(SOURCES) && \
	    $(SVN) export -r$(SLC_REV) $(SLC_REPO) slc-$(SLC_REV) && \
	    cd slc-$(SLC_REV) && \
	    ./bootstrap)

$(SLDIR)/bin/slc: $(SOURCES)/slc-$(SLC_REV)/configure $(SLDIR)/bin/mgsim-alpha
	mkdir -p $(BUILD)/slc-$(SLC_REV)
	(SRC=$$PWD/$(SOURCES)/slc-$(SLC_REV); cd $(BUILD)/slc-$(SLC_REV) && \
	 PATH=$(PREFIX)/slreqs-current/bin:$$PATH $$SRC/configure --prefix=$(SLDIR) && \
	 ($(MAKE) $(MAKE_FLAGS) || true) && \
	 ($(MAKE) install || $(MAKE) -C tools/bin install-man1 || true))
