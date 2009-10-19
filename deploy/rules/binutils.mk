# -*- makefile -*-

BINUTILS_SRC = \
   $(SOURCES)/binutils-$(BINUTILS_REV)/download_done

BINUTILS_TARGETS = \
     $(REQDIR)/bin/alpha-linux-gnu-as \
     $(REQDIR)/bin/mtalpha-linux-gnu-as \
     $(REQDIR)/bin/sparc-leon-linux-as \
     $(REQDIR)/bin/mtsparc-leon-linux-as \

$(SOURCES)/binutils-$(BINUTILS_REV)/download_done:
	rm -f $@
	mkdir -p $(SOURCES)
	(cd $(SOURCES) && \
	  $(SVN) export -r$(BINUTILS_REV) $(BINUTILS_REPO) binutils-$(BINUTILS_REV) && \
	  touch binutils-$(BINUTILS_REV)/download_done)

$(REQDIR)/bin/alpha-linux-gnu-as: $(SOURCES)/binutils-$(BINUTILS_REV)/download_done
	mkdir -p $(BUILD)/binutils-alpha-$(BINUTILS_REV)
	(SRC=$$(cd $(SOURCES)/binutils-$(BINUTILS_REV); pwd); \
           cd $(BUILD)/binutils-alpha-$(BINUTILS_REV) && \
	   $$SRC/configure --target=alpha-linux-gnu \
			  CFLAGS="$$CFLAGS $(EXTRA_CFLAGS)" \
	                  LDFLAGS="$$LDFLAGS $(EXTRA_LDFLAGS)" \
			   --disable-werror \
	                   --prefix=$(REQDIR) && \
	   $(MAKE) $(MAKE_FLAGS) && \
	   $(MAKE) install)

$(REQDIR)/bin/sparc-leon-linux-as: $(SOURCES)/binutils-$(BINUTILS_REV)/download_done
	mkdir -p $(BUILD)/binutils-sparc-$(BINUTILS_REV)
	(SRC=$$(cd $(SOURCES)/binutils-$(BINUTILS_REV); pwd); \
           cd $(BUILD)/binutils-sparc-$(BINUTILS_REV) && \
	   $$SRC/configure --target=sparc-leon-linux \
			  CFLAGS="$$CFLAGS $(EXTRA_CFLAGS)" \
	                  LDFLAGS="$$LDFLAGS $(EXTRA_LDFLAGS)" \
			   --disable-werror \
	                   --prefix=$(REQDIR) && \
	   $(MAKE) $(MAKE_FLAGS) && \
	   $(MAKE) install)

$(REQDIR)/bin/mtalpha-linux-gnu-as: $(SOURCES)/binutils-$(BINUTILS_REV)/download_done
	mkdir -p $(BUILD)/binutils-mtalpha-$(BINUTILS_REV)
	(SRC=$$(cd $(SOURCES)/binutils-$(BINUTILS_REV); pwd); \
           cd $(BUILD)/binutils-mtalpha-$(BINUTILS_REV) && \
	   $$SRC/configure --target=mtalpha-linux-gnu \
			  CFLAGS="$$CFLAGS $(EXTRA_CFLAGS)" \
	                  LDFLAGS="$$LDFLAGS $(EXTRA_LDFLAGS)" \
			   --disable-werror \
	                   --prefix=$(REQDIR) && \
	   $(MAKE) $(MAKE_FLAGS) && \
           $(MAKE) install)

$(REQDIR)/bin/mtsparc-leon-linux-as: $(SOURCES)/binutils-$(BINUTILS_REV)/download_done
	mkdir -p $(BUILD)/binutils-mtsparc-$(BINUTILS_REV)
	(SRC=$$(cd $(SOURCES)/binutils-$(BINUTILS_REV); pwd); \
           cd $(BUILD)/binutils-mtsparc-$(BINUTILS_REV) && \
	   $$SRC/configure --target=mtsparc-leon-linux \
			  CFLAGS="$$CFLAGS $(EXTRA_CFLAGS)" \
	                  LDFLAGS="$$LDFLAGS $(EXTRA_LDFLAGS)" \
			   --disable-werror \
	                   --prefix=$(REQDIR) && \
	   $(MAKE) $(MAKE_FLAGS) && \
           $(MAKE) install)
