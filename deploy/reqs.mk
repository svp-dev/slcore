# -*- makefile -*-
#
# This is reqs.mk, a companion Makefile
# for Makefile.reqs. See the comments at the start
# of Makefile-reqs.orig for details.

######## YOU SHOULD NOT NEED TO EDIT THE FOLLOWING ########

REQDIR = $(PREFIX)/slreqs-$(BINUTILS_REV)-$(UTC_GCC_REV)-$(GCC_VERSION)
SLDIR = $(PREFIX)/sl-$(MGSIM_REV)-$(SLC_REV)

BUILD = sl-reqs/build
SOURCES = sl-reqs/src

GCC_CONFIG_FLAGS = \
   --disable-bootstrap --disable-libmudflap --disable-libssp \
   --disable-coverage --enable-gdb --disable-threads --disable-nls \
   --disable-multilib --enable-languages=c 

all: \
     $(PREFIX)/slenv \
     $(REQDIR)/bin/alpha-linux-gnu-as \
     $(REQDIR)/bin/alpha-linux-gnu-gcc \
     $(REQDIR)/bin/mtalpha-linux-gnu-as \
     $(REQDIR)/bin/sparc-leon-linux-as \
     $(REQDIR)/bin/mtsparc-leon-linux-as \
     $(SLDIR)/bin/mgsim-alpha \
     $(SLDIR)/bin/mgsim-sparc \
     $(REQDIR)/bin/mtalpha-linux-gnu-gcc \
     $(SLDIR)/bin/slc
	@echo
	@echo "*** All dependencies are now installed. ***"
	@echo


$(PREFIX)/slenv: slenv
	mkdir -p $(PREFIX)
	cp $^ $@
	chmod +x $@

##### Binutils #####

$(SOURCES)/binutils-$(BINUTILS_REV)/download_done:
	rm -f $@
	mkdir -p $(SOURCES)
	(cd $(SOURCES) && \
	  $(SVN) export -r$(BINUTILS_REV) $(BINUTILS_REPO) binutils-$(BINUTILS_REV) && \
	  touch binutils-$(BINUTILS_REV)/download_done)

$(REQDIR)/bin/alpha-linux-gnu-as: $(SOURCES)/binutils-$(BINUTILS_REV)/download_done
	mkdir -p $(BUILD)/binutils-alpha-$(BINUTILS_REV)
	(SRC=$$PWD/$(SOURCES)/binutils-$(BINUTILS_REV); \
           cd $(BUILD)/binutils-alpha-$(BINUTILS_REV) && \
	   $$SRC/configure --target=alpha-linux-gnu \
	                   --prefix=$(REQDIR) && \
	   $(MAKE) $(MAKE_FLAGS) && \
	   $(MAKE) install)

$(REQDIR)/bin/sparc-leon-linux-as: $(SOURCES)/binutils-$(BINUTILS_REV)/download_done
	mkdir -p $(BUILD)/binutils-sparc-$(BINUTILS_REV)
	(SRC=$$PWD/$(SOURCES)/binutils-$(BINUTILS_REV); \
           cd $(BUILD)/binutils-sparc-$(BINUTILS_REV) && \
	   $$SRC/configure --target=sparc-leon-linux \
	                   --prefix=$(REQDIR) && \
	   $(MAKE) $(MAKE_FLAGS) && \
	   $(MAKE) install)

$(REQDIR)/bin/mtalpha-linux-gnu-as: $(SOURCES)/binutils-$(BINUTILS_REV)/download_done
	mkdir -p $(BUILD)/binutils-mtalpha-$(BINUTILS_REV)
	(SRC=$$PWD/$(SOURCES)/binutils-$(BINUTILS_REV); \
           cd $(BUILD)/binutils-mtalpha-$(BINUTILS_REV) && \
	   $$SRC/configure --target=mtalpha-linux-gnu \
	                   --prefix=$(REQDIR) && \
	   $(MAKE) $(MAKE_FLAGS) && \
           $(MAKE) install)

$(REQDIR)/bin/mtsparc-leon-linux-as: $(SOURCES)/binutils-$(BINUTILS_REV)/download_done
	mkdir -p $(BUILD)/binutils-mtsparc-$(BINUTILS_REV)
	(SRC=$$PWD/$(SOURCES)/binutils-$(BINUTILS_REV); \
           cd $(BUILD)/binutils-mtsparc-$(BINUTILS_REV) && \
	   $$SRC/configure --target=mtsparc-leon-linux \
	                   --prefix=$(REQDIR) && \
	   $(MAKE) $(MAKE_FLAGS) && \
           $(MAKE) install)


##### muTC core compiler #####

$(SOURCES)/utc-gcc-$(UTC_GCC_REV)/download_done:
	rm -f $@
	mkdir -p $(SOURCES)
	(cd $(SOURCES) && \
         $(SVN) export -r$(UTC_GCC_REV) $(UTC_GCC_REPO) utc-gcc-$(UTC_GCC_REV) && \
	 touch utc-gcc-$(UTC_GCC_REV)/download_done)

$(REQDIR)/bin/mtalpha-linux-gnu-gcc: \
	$(REQDIR)/bin/mtalpha-linux-gnu-as \
	$(SOURCES)/utc-gcc-$(UTC_GCC_REV)/download_done
	mkdir -p $(BUILD)/gcc-mtalpha-$(UTC_GCC_REV)
	(SRC=$$PWD/$(SOURCES)/utc-gcc-$(UTC_GCC_REV); \
           cd $(BUILD)/gcc-mtalpha-$(UTC_GCC_REV) && \
	   $$SRC/configure --target=mtalpha-linux-gnu \
			       --prefix=$(REQDIR) \
	                       $(GCC_CONFIG_FLAGS) && \
	 $(MAKE) $(MAKE_FLAGS) && \
	 $(MAKE) install)

##### GCC cross-compiler #####

$(SOURCES)/$(GCC_ARCHIVE):
	mkdir -p $(SOURCES)
	(cd $(SOURCES) && $(FETCH) $(GCC_MIRROR)/$(GCC_ARCHIVE))

$(SOURCES)/gcc-$(GCC_VERSION)/extract_done: $(SOURCES)/$(GCC_ARCHIVE)
	rm -f $@
	(cd $(SOURCES) && \
	  $(TAR) -xjf $(GCC_ARCHIVE) && \
	  touch gcc-$(GCC_VERSION)/extract_done)

$(REQDIR)/bin/alpha-linux-gnu-gcc: \
	$(REQDIR)/bin/alpha-linux-gnu-as \
	$(SOURCES)/gcc-$(GCC_VERSION)/extract_done
	mkdir -p $(BUILD)/gcc-alpha-$(GCC_VERSION)
	(SRC=$$PWD/$(SOURCES)/gcc-$(GCC_VERSION); \
          cd $(BUILD)/gcc-alpha-$(GCC_VERSION) && \
	  $$SRC/configure --target=alpha-linux-gnu \
	                  --prefix=$(REQDIR) \
			  $(GMP_CONF) $(MPFR_CONF) \
	                  $(GCC_CONFIG_FLAGS) && \
	  grep -v 'maybe-[a-z]*-target-\(libgcc\|libiberty\|libgomp\)' <Makefile >Makefile.tmp && \
	  mv -f Makefile.tmp Makefile && \
	  if ! $(MAKE) $(MAKE_FLAGS); then \
	    perl -pi.bak -n -e \
	      's|^LIBICONV .*|LIBICONV = -L/usr/lib -liconv|g' gcc/Makefile && \
               $(MAKE) $(MAKE_FLAGS); \
          fi && \
          $(MAKE) install)

##### Simulator #####

$(SOURCES)/mgsim-$(MGSIM_REV)/configure:
	mkdir -p $(SOURCES)
	(cd $(SOURCES) && \
	    $(SVN) export -r$(MGSIM_REV) $(MGSIM_REPO) mgsim-$(MGSIM_REV) && \
	    cd mgsim-$(MGSIM_REV) && \
	    ./bootstrap)


$(SLDIR)/bin/mgsim-alpha $(SLDIR)/bin/mgsim-sparc: $(SOURCES)/mgsim-$(MGSIM_REV)/configure
	mkdir -p $(BUILD)/mgsim-$(MGSIM_REV)
	rm -f $(PREFIX)/slreqs-current
	ln -s $(REQDIR) $(PREFIX)/slreqs-current
	(SRC=$$PWD/$(SOURCES)/mgsim-$(MGSIM_REV); cd $(BUILD)/mgsim-$(MGSIM_REV) && \
	 PATH=$(PREFIX)/slreqs-current/bin:$$PATH $$SRC/configure --prefix=$(SLDIR) && \
	 $(MAKE) $(MAKE_FLAGS) && \
	 $(MAKE) install)
	rm -f $(PREFIX)/sl-current
	ln -s $(SLDIR) $(PREFIX)/sl-current

##### SL Compiler #####

$(SOURCES)/slc-$(SLC_REV)/configure:
	mkdir -p $(SOURCES)
	(cd $(SOURCES) && \
	    $(SVN) export -r$(SLC_REV) $(SLC_REPO) slc-$(SLC_REV) && \
	    cd slc-$(SLC_REV) && \
	    ./bootstrap)

$(SLDIR)/bin/slc: $(SOURCES)/slc-$(SLC_REV)/configure $(SLDIR)/bin/mgsim-alpha
	mkdir -p $(BUILD)/slc-$(SLC_REV)
	(SRC=$$PWD/$(SOURCES)/slc-$(SLC_REV); cd $(BUILD)/slc-$(SLC_REV) && \
	 PATH=$(PREFIX)/slreqs-current/bin:$$PATH $$SRC/configure --prefix=$(SLDIR) \
	     --enable-maintainer-mode && \
	 ($(MAKE) $(MAKE_FLAGS) || true) && \
	 ($(MAKE) install || $(MAKE) -C doc install-man1 || true))
