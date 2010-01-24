# -*- makefile -*-

GCC_SRC = $(SRCBASE)/gcc-$(GCC_VERSION)
GCC_BUILD = $(BLDBASE)/gcc-$(GCC_VERSION)
GCC_TARGETS = alpha-linux-gnu

GCC_CFG_TARGETS = $(foreach T,$(GCC_TARGETS),$(GCC_BUILD)-$(T)/configure_done)
GCC_BUILD_TARGETS = $(foreach T,$(GCC_TARGETS),$(GCC_BUILD)-$(T)/build_done)
GCC_INST_TARGETS = $(foreach T,$(GCC_TARGETS),$(REQDIR)/.gcc-installed-$(T))

GCC_CONFIG_FLAGS = \
   --disable-bootstrap --disable-libmudflap --disable-libssp \
   --disable-coverage --enable-gdb --disable-threads --disable-nls \
   --disable-multilib --enable-languages=c 

.PRECIOUS: $(GCC_ARCHIVE) $(GCC_CFG_TARGETS) $(GCC_BUILD_TARGETS) $(GCC_INST_TARGETS)

gcc-fetch: $(GCC_ARCHIVE) ; $(RULE_DONE)
gcc-configure: $(GCC_CFG_TARGETS) ; $(RULE_DONE)
gcc-build: $(GCC_BUILD_TARGETS) ; $(RULE_DONE)
gcc-install: $(GCC_INST_TARGETS) ; $(RULE_DONE)

$(GCC_SRC)/configure: $(GCC_ARCHIVE)
	rm -f $@
	$(UNTAR) $(SRCBASE) $(GCC_ARCHIVE)
	touch $@

$(GCC_BUILD)-%/configure_done: $(GCC_SRC)/configure $(REQDIR)/.binutils-installed-%
	rm -f $@
	$(MKDIR_P) $(GCC_BUILD)-$*
	SRC=$$(cd $(GCC_SRC) && pwd) && \
           cd $(GCC_BUILD)-$* && \
	   find . -name config.cache -exec rm '{}' \; && \
	   $$SRC/configure --target=$* \
			   --prefix=$(REQDIR) \
			       CC="$(CC)" \
			       CFLAGS="$(CPPFLAGS) $(CFLAGS)" \
	                       LDFLAGS="$(CFLAGS) $(LDFLAGS)" \
	                       $(GCC_CONFIG_FLAGS) && \
	  $(GREP) -v 'maybe-[a-z]*-target-\(libgcc\|libiberty\|libgomp\|zlib\)' <Makefile >Makefile.tmp && \
	  mv -f Makefile.tmp Makefile
	touch $@

$(GCC_BUILD)-%/build_done: $(GCC_BUILD)-%/configure_done
	rm -f $@
	cd $(GCC_BUILD)-$* && \
	  if ! $(MAKE) ; then \
	    $(SED) -e 's|^LIBICONV .*|LIBICONV = -L/usr/lib -liconv|g' <gcc/Makefile >gcc/Makefile.tmp && \
	    mv -f gcc/Makefile.tmp gcc/Makefile && \
	    $(MAKE) ; \
          fi
	touch $@

$(REQDIR)/.gcc-installed-%: $(GCC_BUILD)-%/build_done
	rm -f $*
	cd $(GCC_BUILD)-$* && $(MAKE) -j1 install
	touch $@


