# -*- makefile -*-

GCC_SRC = \
   $(SOURCES)/gcc-$(GCC_VERSION)/patch_done

GCC_TARGETS = \
     $(REQDIR)/bin/alpha-linux-gnu-gcc

GCC_CONFIG_FLAGS = \
   --disable-bootstrap --disable-libmudflap --disable-libssp \
   --disable-coverage --enable-gdb --disable-threads --disable-nls \
   --disable-multilib --enable-languages=c 

##### GCC cross-compiler #####

$(SOURCES)/$(GCC_ARCHIVE):
	mkdir -p $(SOURCES)
	(cd $(SOURCES) && $(FETCH) $(GCC_MIRROR)/$(GCC_ARCHIVE))

$(SOURCES)/gcc-$(GCC_VERSION)/extract_done: $(SOURCES)/$(GCC_ARCHIVE)
	rm -f $@
	(cd $(SOURCES) && \
	  $(TAR) -xjf $(GCC_ARCHIVE) && \
	  touch gcc-$(GCC_VERSION)/extract_done)

$(SOURCES)/gcc-$(GCC_VERSION)/patch_done: $(SOURCES)/gcc-$(GCC_VERSION)/extract_done
	rm -f $@
	(cd $(SOURCES)/gcc-$(GCC_VERSION)/gcc/config/alpha && \
	   patch -p0 <$$OLDPWD/patches/alpha.h.patch && \
	   patch -p0 <$$OLDPWD/patches/alpha.c.patch && \
	   patch -p0 <$$OLDPWD/patches/alpha.md.patch && \
	   patch -p0 <$$OLDPWD/patches/constraints.md.patch && \
	   touch ../../../patch_done)

$(REQDIR)/bin/alpha-linux-gnu-gcc: \
	$(REQDIR)/bin/alpha-linux-gnu-as \
	$(SOURCES)/gcc-$(GCC_VERSION)/patch_done
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

