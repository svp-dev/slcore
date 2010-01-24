# -*- makefile -*-

GCC_SRC = \
   $(SOURCES)/gcc-$(GCC_VERSION)

GCC_TARGETS = alpha-linux-gnu

GCC_CFG_TARGETS = $(foreach T,$(GCC_TARGETS),$(BUILD)/gcc-$(GCC_VERSION)-$(T)/configure_done)
GCC_BUILD_TARGETS = $(foreach T,$(GCC_TARGETS),$(BUILD)/gcc-$(GCC_VERSION)-$(T)/gcc/gcc-cross)
GCC_INST_TARGETS = $(foreach T,$(GCC_TARGETS),$(REQDIR)/bin/$(T)-gcc)

GCC_CONFIG_FLAGS = \
   --disable-bootstrap --disable-libmudflap --disable-libssp \
   --disable-coverage --enable-gdb --disable-threads --disable-nls \
   --disable-multilib --enable-languages=c 

gcc-configure: $(GCC_CFG_TARGETS)
gcc-build: $(GCC_BUILD_TARGETS)
gcc-install: $(GCC_INST_TARGETS)

$(GCC_SRC)/configure: $(GCC_ARCHIVE)
	rm -f $@
	mkdir -p $(SOURCES)
	tar -C $(SOURCES) -xjvf $(GCC_ARCHIVE)
	touch $@

$(BUILD)/gcc-$(GCC_VERSION)-%/configure_done: $(GCC_SRC)/configure $(REQDIR)/bin/%-as
	rm -f $@
	mkdir -p $(BUILD)/gcc-$(GCC_VERSION)-$*
	SRC=$$(cd $(SOURCES)/gcc-$(GCC_VERSION) && pwd) && \
           cd $(BUILD)/gcc-$(GCC_VERSION)-$* && \
	   $$SRC/configure --target=$* \
			       --prefix=$(REQDIR) \
			       CFLAGS="$$CFLAGS $(EXTRA_CFLAGS)" \
	                       LDFLAGS="$$LDFLAGS $(EXTRA_LDFLAGS)" \
	                       $(GCC_CONFIG_FLAGS) && \
	  grep -v 'maybe-[a-z]*-target-\(libgcc\|libiberty\|libgomp\|zlib\)' <Makefile >Makefile.tmp && \
	  mv -f Makefile.tmp Makefile
	touch $@

$(BUILD)/gcc-$(GCC_VERSION)-%/gcc/gcc-cross: $(BUILD)/gcc-$(GCC_VERSION)-%/configure_done
	cd $(BUILD)/gcc-$(GCC_VERSION)-$* && \
	  if ! $(MAKE) $(MAKE_FLAGS); then \
	    perl -pi.bak -n -e \
	      's|^LIBICONV .*|LIBICONV = -L/usr/lib -liconv|g' gcc/Makefile && \
               $(MAKE) $(MAKE_FLAGS); \
          fi

$(REQDIR)/bin/%-gcc: $(BUILD)/gcc-$(GCC_VERSION)-%/gcc/gcc-cross
	cd $(BUILD)/gcc-$(GCC_VERSION)-$* && $(MAKE) -j1 install


