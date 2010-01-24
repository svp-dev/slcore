# -*- makefile -*-

GCC_SRC = $(SOURCES)/gcc-$(GCC_VERSION)
GCC_BUILD = $(BUILD)/gcc-$(GCC_VERSION)
GCC_TARGETS = alpha-linux-gnu

GCC_CFG_TARGETS = $(foreach T,$(GCC_TARGETS),$(GCC_BUILD)-$(T)/configure_done)
GCC_BUILD_TARGETS = $(foreach T,$(GCC_TARGETS),$(GCC_BUILD)-$(T)/build_done)
GCC_INST_TARGETS = $(foreach T,$(GCC_TARGETS),$(REQDIR)/.gcc-installed-$(T))

GCC_CONFIG_FLAGS = \
   --disable-bootstrap --disable-libmudflap --disable-libssp \
   --disable-coverage --enable-gdb --disable-threads --disable-nls \
   --disable-multilib --enable-languages=c 

.PRECIOUS: $(GCC_CFG_TARGETS) $(GCC_BUILD_TARGETS) $(GCC_INST_TARGETS)

gcc-configure: $(GCC_CFG_TARGETS)
gcc-build: $(GCC_BUILD_TARGETS)
gcc-install: $(GCC_INST_TARGETS)

$(GCC_SRC)/configure: $(GCC_ARCHIVE)
	rm -f $@
	mkdir -p $(SOURCES)
	tar -C $(SOURCES) -xjvf $(GCC_ARCHIVE)
	touch $@

$(GCC_BUILD)-%/configure_done: $(GCC_SRC)/configure $(REQDIR)/.binutils-installed-%
	rm -f $@
	mkdir -p $(GCC_BUILD)-$*
	SRC=$$(cd $(GCC_SRC) && pwd) && \
           cd $(GCC_BUILD)-$* && \
	   find . -name config.cache -exec rm '{}' \; && \
	   $$SRC/configure --target=$* \
			       --prefix=$(REQDIR) \
			       CFLAGS="$$CFLAGS $(EXTRA_CFLAGS)" \
	                       LDFLAGS="$$LDFLAGS $(EXTRA_LDFLAGS)" \
	                       $(GCC_CONFIG_FLAGS) && \
	  grep -v 'maybe-[a-z]*-target-\(libgcc\|libiberty\|libgomp\|zlib\)' <Makefile >Makefile.tmp && \
	  mv -f Makefile.tmp Makefile
	touch $@

$(GCC_BUILD)-%/build_done: $(GCC_BUILD)-%/configure_done
	rm -f $@
	cd $(GCC_BUILD)-$* && \
	  if ! $(MAKE) $(MAKE_FLAGS); then \
	    perl -pi.bak -n -e \
	      's|^LIBICONV .*|LIBICONV = -L/usr/lib -liconv|g' gcc/Makefile && \
               $(MAKE) $(MAKE_FLAGS); \
          fi
	touch $@

$(REQDIR)/.gcc-installed-%: $(GCC_BUILD)-%/build_done
	rm -f $*
	cd $(GCC_BUILD)-$* && $(MAKE) -j1 install
	touch $@


