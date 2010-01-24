# -*- makefile -*-
######## YOU SHOULD NOT NEED TO EDIT THE FOLLOWING ########
#        (see the file README for more information)

include common.mk

include tools.mk.defaults
-include tools.mk
include layout.mk.defaults
-include layout.mk
include versions.mk.defaults
-include versions.mk

REQDIR = $(PREFIX)/slreqs-$(BINUTILS_VERSION)-$(MGGCC_VERSION)-$(GCC_VERSION)-m$(M4_VERSION)-sc$(SC_VERSION)
REQTAG = $(REQDIR)/.tag
SLDIR = $(PREFIX)/sl-$(MGSIM_VERSION)-$(SLC_REV)
SLTAG = $(SLDIR)/.tag

.PRECIOUS: $(REQTAG) $(SLTAG)
%/.tag:
	mkdir -p `dirname $@`
	touch $@

BUILD = $(INST_BUILD)
SOURCES = $(INST_SOURCES)

.PHONY: all

all:

include rules/binutils.mk
include rules/m4.mk
include rules/mggcc.mk
include rules/gcc.mk
include rules/sc.mk
include rules/mgsim.mk
include rules/sl.mk

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

