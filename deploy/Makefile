# -*- makefile -*-
######## YOU SHOULD NOT NEED TO EDIT THE FOLLOWING ########
#        (see the file README for more information)

include tools.mk.defaults
-include tools.mk
include layout.mk.defaults
-include layout.mk
include versions.mk.defaults
-include versions.mk

REQDIR = $(PREFIX)/slreqs-$(BINUTILS_REV)-$(UTC_GCC_REV)-$(GCC_VERSION)p-m$(M4_VERSION)p-tx$(TX_VERSION)
SLDIR = $(PREFIX)/sl-$(MGSIM_REV)-$(SLC_REV)

BUILD = $(LOCAL_BUILD)
SOURCES = $(LOCAL_SOURCES)

.PHONY: all reqs-src sl-src fetch reqs

all:

include rules/*.mk

reqs-src: \
	$(BINUTILS_SRC) \
	$(GCC_SRC) \
	$(M4_SRC) \
	$(TX_SRC) \
	$(MGGCC_SRC)

sl-src: \
	$(MGSIM_SRC) \
	$(SL_SRC)

fetch: reqs-src sl-src

reqs: \
	$(BINUTILS_TARGETS) \
	$(MGGCC_TARGETS) \
	$(GCC_TARGETS) \
	$(M4_TARGETS) \
	$(TX_TARGETS)

all: reqs \
	$(MGSIM_TARGETS) \
	$(SL_TARGETS) \
	$(PREFIX)/slenv
	@echo
	@echo "*** All dependencies are now installed. ***"
	@echo

$(PREFIX)/slenv: slenv
	mkdir -p $(PREFIX)
	rm -f $@
	cp $^ $@
	chmod -w+x $@

