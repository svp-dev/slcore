# -*- makefile -*-

MGGCC_SRC = \
  $(SOURCES)/utc-gcc-$(UTC_GCC_REV)/download_done

MGGCC_TARGETS = \
     $(REQDIR)/bin/mtalpha-linux-gnu-gcc

GCC_CONFIG_FLAGS = \
   --disable-bootstrap --disable-libmudflap --disable-libssp \
   --disable-coverage --enable-gdb --disable-threads --disable-nls \
   --disable-multilib --enable-languages=c 

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

