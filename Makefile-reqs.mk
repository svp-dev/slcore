# -*- makefile -*-
#
# This is Makefile-reqs.mk, a companion Makefile
# for Makefile.reqs. See the comments at the start
# of Makefile-reqs.orig for details.

######## YOU SHOULD NOT NEED TO EDIT THE FOLLOWING ########

# Source URLs:
BINUTILS_REPO = https://svn.svp-home.org/sw/svp/mg/binutils/trunk
UTC_GCC_REPO = https://svn.svp-home.org/sw/svp/mg/gcc/trunk
MGSIM_REPO = https://svn.svp-home.org/sw/svp/mg/mgsim/trunk

GCC_VERSION = 4.3.3
GCC_ARCHIVE = gcc-core-$(GCC_VERSION).tar.bz2
GCC_MIRROR = http://gcc.cybermirror.org/releases/gcc-$(GCC_VERSION)

BUILD = sl-reqs/build
SOURCES = sl-reqs/src

GCC_CONFIG_FLAGS = \
   --disable-bootstrap --disable-libmudflap --disable-libssp \
   --disable-coverage --enable-gdb --disable-threads --disable-nls \
   --disable-multilib --enable-languages=c 

all: \
     $(PREFIX)/bin/alpha-linux-gnu-as \
     $(PREFIX)/bin/alpha-linux-gnu-gcc \
     $(PREFIX)/bin/mtalpha-linux-gnu-as \
     $(PREFIX)/bin/mgsim-alpha \
     $(PREFIX)/bin/mtalpha-linux-gnu-gcc \
     $(PREFIX)/bin/sparc-leon-linux-as \
     $(PREFIX)/bin/mtsparc-leon-linux-as \
     $(PREFIX)/bin/mgsim-sparc
	@echo
	@echo "*** All dependencies are now installed. ***"
	@echo


##### Binutils #####

$(SOURCES)/binutils/download_done:
	rm -f $@
	mkdir -p $(SOURCES)
	(cd $(SOURCES) && \
	  $(SVN) co $(BINUTILS_REPO) binutils && \
	  touch binutils/download_done)

$(PREFIX)/bin/alpha-linux-gnu-as: $(SOURCES)/binutils/download_done
	mkdir -p $(BUILD)/binutils-alpha
	(SRC=$$PWD/$(SOURCES)/binutils; cd $(BUILD)/binutils-alpha && \
	   $$SRC/configure --target=alpha-linux-gnu \
	                   --prefix=$(PREFIX) && \
	   $(MAKE) $(MAKE_FLAGS) && \
	   $(MAKE) install)

$(PREFIX)/bin/sparc-leon-linux-as: $(SOURCES)/binutils/download_done
	mkdir -p $(BUILD)/binutils-sparc
	(SRC=$$PWD/$(SOURCES)/binutils; cd $(BUILD)/binutils-sparc && \
	   $$SRC/configure --target=sparc-leon-linux \
	                   --prefix=$(PREFIX) && \
	   $(MAKE) $(MAKE_FLAGS) && \
	   $(MAKE) install)

$(PREFIX)/bin/mtalpha-linux-gnu-as: $(SOURCES)/binutils/download_done
	mkdir -p $(BUILD)/binutils-mtalpha
	(SRC=$$PWD/$(SOURCES)/binutils; cd $(BUILD)/binutils-mtalpha && \
	   $$SRC/configure --target=mtalpha-linux-gnu \
	                   --prefix=$(PREFIX) && \
	   $(MAKE) $(MAKE_FLAGS) && \
           $(MAKE) install)

$(PREFIX)/bin/mtsparc-leon-linux-as: $(SOURCES)/binutils/download_done
	mkdir -p $(BUILD)/binutils-mtsparc
	(SRC=$$PWD/$(SOURCES)/binutils; cd $(BUILD)/binutils-mtsparc && \
	   $$SRC/configure --target=mtsparc-leon-linux \
	                   --prefix=$(PREFIX) && \
	   $(MAKE) $(MAKE_FLAGS) && \
           $(MAKE) install)


##### muTC core compiler #####

$(SOURCES)/utc-gcc/download_done:
	rm -f $@
	mkdir -p $(SOURCES)
	(cd $(SOURCES) && \
         $(SVN) co $(UTC_GCC_REPO) utc-gcc && \
	 touch utc-gcc/download_done)

$(PREFIX)/bin/mtalpha-linux-gnu-gcc: \
	$(PREFIX)/bin/mtalpha-linux-gnu-as \
	$(SOURCES)/utc-gcc/download_done
	mkdir -p $(BUILD)/gcc-mtalpha
	(SRC=$$PWD/$(SOURCES)/utc-gcc; cd $(BUILD)/gcc-mtalpha && \
	   $$SRC/configure --target=mtalpha-linux-gnu \
			       --prefix=$(PREFIX) \
	                       $(GCC_CONFIG_FLAGS) && \
	 $(MAKE) $(MAKE_FLAGS) && \
	 $(MAKE) install)

##### GCC 4.3 cross-compiler #####

$(SOURCES)/$(GCC_ARCHIVE):
	mkdir -p $(SOURCES)
	(cd $(SOURCES) && $(FETCH) $(GCC_MIRROR)/$(GCC_ARCHIVE))

$(SOURCES)/gcc-$(GCC_VERSION)/extract_done: $(SOURCES)/$(GCC_ARCHIVE)
	rm -f $@
	(cd $(SOURCES) && \
	  $(TAR) -xjf $(GCC_ARCHIVE) && \
	  touch gcc-$(GCC_VERSION)/extract_done)

$(PREFIX)/bin/alpha-linux-gnu-gcc: \
	$(PREFIX)/bin/alpha-linux-gnu-as \
	$(SOURCES)/gcc-$(GCC_VERSION)/extract_done
	mkdir -p $(BUILD)/gcc-alpha
	(SRC=$$PWD/$(SOURCES)/gcc-$(GCC_VERSION); cd $(BUILD)/gcc-alpha && \
	  $$SRC/configure --target=alpha-linux-gnu \
	                  --prefix=$(PREFIX) \
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

$(SOURCES)/mgsim/configure:
	mkdir -p $(SOURCES)
	(cd $(SOURCES) && \
	    $(SVN) co $(MGSIM_REPO) mgsim && \
	    cd mgsim && \
	    ./bootstrap)


$(PREFIX)/bin/mgsim-alpha $(PREFIX)/bin/mgsim-sparc: $(SOURCES)/mgsim/configure
	mkdir -p $(BUILD)/mgsim
	(SRC=$$PWD/$(SOURCES)/mgsim; cd $(BUILD)/mgsim && \
	 $$SRC/configure --prefix=$(PREFIX) && \
	 $(MAKE) $(MAKE_FLAGS) && \
	 $(MAKE) install)

