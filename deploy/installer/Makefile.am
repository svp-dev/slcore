DSTBASE = $(DESTDIR)$(prefix)

REQNAME = slreqs-b$(BINUTILS_VERSION)-u$(MGGCC_VERSION)-g$(GCC_VERSION)-m$(M4_VERSION)-sc$(SC_VERSION)
REQDIR = $(DSTBASE)/$(REQNAME)
REQTAG = $(REQDIR)/.tag

SLNAME = sl-s$(MGSIM_VERSION)-c$(SLC_VERSION)
SLDIR = $(DSTBASE)/$(SLNAME)
SLTAG = $(SLDIR)/.tag

REQCURRENT = $(DSTBASE)/slreqs-$(SLNAME)
SLCURRENT = $(DSTBASE)/sl-current

ARCHIVEDIR = distfiles
SRCBASE = sources
BLDBASE = build

REQPARTS = sc binutils m4 mggcc gcc slenv
SLPARTS = mgsim slc
PARTS = $(REQPARTS) $(SLPARTS)

.PRECIOUS: $(REQTAG) $(SLTAG)
%/.tag:
	$(MKDIR_P) `dirname $@`
	touch $@

.PRECIOUS: $(ARCHIVEDIR)/%
$(ARCHIVEDIR)/%:
	$(MKDIR_P) $(ARCHIVEDIR)
	cd $(ARCHIVEDIR) && $(FETCH) $(DIST_SITE)/$*

.PHONY: help
help:
	@echo "Usage: $(MAKE) <rule> -jN"
	@echo
	@echo "General rules:"
	@echo "   fetch          fetch all archives"
	@echo "   reqs-install   build and install requirements"
	@echo "   install        build and install everything"
	@echo
	@echo "Part-specific rules rules:"
	@echo "   XXX-fetch      fetch archive for part XXX"
	@echo "   XXX-configure  configure build for part XXX"
	@echo "   XXX-build      build part XXX"
	@echo "   XXX-install    install part XXX"
	@echo
	@echo "Parts:"
	@echo "   m4 binutils mggcc gcc sc"
	@echo "   mgsim slc slenv"

RULE_DONE = @echo "*** $@: done. ***"; echo
UNTAR = do_extract() { \
     $(MKDIR_P) "$$1" && \
     case "$$2" in \
        *.tar.gz|*.tgz) GZIP=$(GZIP_ENV) gzip -dc "$$2" | ( cd "$$1" && $(am__untar) ) ;; \
	*.tar.bz2*|*.tbz) bzip2 -dc "$$2" | ( cd "$$1" && $(am__untar) ) ;; \
     esac; }; do_extract

RULEFILES = $(foreach P,$(PARTS),$(srcdir)/rules/$(P).mk)

include $(srcdir)/build-aux/version.mk

include $(RULEFILES)

EXTRA_DIST += $(RULEFILES)

reqs-fetch: sc-fetch
	$(MAKE) $(foreach P,$(REQPARTS),$(P)-fetch)
	$(RULE_DONE)

reqs-install: reqs-fetch
	$(MAKE) $(foreach P,$(REQPARTS),$(P)-install)
	$(RULE_DONE)

fetch: sc-fetch
	$(MAKE) $(foreach P,$(PARTS),$(P)-fetch)
	$(RULE_DONE)

install-data-local: fetch
	$(MAKE) $(foreach P,$(PARTS),$(P)-install)
	$(RULE_DONE)

uninstall-local:
	rm -rf $(REQDIR) $(SLDIR)
	found= ; \
	  for d in $(prefix)/sl-* $(prefix)/slreqs-*; do \
	     if test -r $$d; then found=1; fi; \
	  done; \
	  if test -z "$found"; then \
	     rm -f $(prefix)/slenv $(prefix)/sl-current $(prefix)/slreqs-current; \
	  fi


ALL_RULES = fetch reqs-install install $(foreach P,$(PARTS),$(foreach R,fetch configure build install,$(P)-$(R)))
.PHONY: $(ALL_RULES)

