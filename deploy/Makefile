# -*- makefile -*-
######## YOU SHOULD NOT NEED TO EDIT THE FOLLOWING ########
#        (see the file README for more information)

include tools.mk.defaults
-include tools.mk
include layout.mk.defaults
-include layout.mk
include versions.mk.defaults
-include versions.mk

REQDIR = $(PREFIX)/slreqs-$(BINUTILS_REV)-$(UTC_GCC_REV)-$(GCC_VERSION)p-m$(M4_VERSION)p
SLDIR = $(PREFIX)/sl-$(MGSIM_REV)-$(SLC_REV)

BUILD = $(LOCAL_BUILD)
SOURCES = $(LOCAL_SOURCES)

all:

include rules/*.mk

all: \
	$(BINUTILS_TARGETS) \
	$(MGGCC_TARGETS) \
	$(GCC_TARGETS) \
	$(M4_TARGETS) \
	$(MGSIM_TARGETS) \
	$(SL_TARGETS) \
	$(PREFIX)/slenv
	@echo
	@echo "*** All dependencies are now installed. ***"
	@echo

$(PREFIX)/slenv: slenv
	mkdir -p $(PREFIX)
	cp $^ $@
	chmod -w+x $@

