DSTBASE = $(DESTDIR)$(prefix)

REQBASENAME = b$(BINUTILS_VERSION)-u$(MGGCC_VERSION)-g$(GCC_VERSION)-m$(M4_VERSION)-sc$(SC_VERSION)
REQNAME = slreqs-$(REQBASENAME)
REQDIR = $(DSTBASE)/$(REQNAME)
REQTAG = $(REQDIR)/.tag

SLBASENAME = s$(MGSIM_VERSION)-c$(SLC_VERSION)
SLNAME = sl-$(SLBASENAME)
SLDIR = $(DSTBASE)/$(SLNAME)
SLTAG = $(SLDIR)/.tag

REQCURRENT = $(DSTBASE)/slreqs-$(SLNAME)
SLCURRENT = $(DSTBASE)/$(SLNAME)

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
	$(AM_V_at)$(MKDIR_P) $(ARCHIVEDIR)
	$(AM_V_at)rm -f "$@" "$@".tmp
	$(AM_V_GEN)$(FETCH) $(DIST_SITE)/$* >"$@".tmp
	$(AM_V_at)mv -f "$@".tmp "$@"

.PHONY: help
help:
	@echo "Usage: $(MAKE) <rule> -jN"
	@echo
	@echo "General rules:"
	@echo "   fetch          fetch all archives"
	@echo "   reqs-install   build and install requirements"
	@echo "   install        build and install everything"
	@echo "   moretags       detect installed configuration tags"
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

EXTRA_DIST += $(RULEFILES) latest.dist

latest.dist:
	$(AM_V_at)rm -f latest latest.dist
	$(AM_V_GEN)$(FETCH) $(TAGS_SITE)/latest >latest
	$(AM_V_at)mv -f latest latest.dist

reqs-fetch: sc-fetch
	$(AM_V_at)$(MAKE) $(foreach P,$(REQPARTS),$(P)-fetch)
	$(RULE_DONE)

reqs-install: reqs-fetch
	$(AM_V_at)$(MAKE) $(foreach P,$(REQPARTS),$(P)-install)
	$(RULE_DONE)

fetch: sc-fetch
	$(AM_V_at)$(MAKE) $(foreach P,$(PARTS),$(P)-fetch)
	$(RULE_DONE)

install-data-local: fetch tags/.installed
	$(MAKE) $(foreach P,$(PARTS),$(P)-install)
	$(MAKE) moretags
	$(MKDIR_P) $(DSTBASE)/tags
	$(INSTALL_DATA) tags/* $(DSTBASE)/tags/
	$(INSTALL_DATA) tags/.installed $(DSTBASE)/tags/installed-`date +%Y%m%d%H%M%S`
	$(INSTALL_DATA) tags/.installed $(DSTBASE)/tags/last-installed
	$(RULE_DONE)

tags/.installed: Makefile
	$(AM_V_at)$(MKDIR_P) tags
	$(AM_V_at)rm -f $@
	$(AM_V_GEN)(echo "# `date +'%Y-%m%d %H:%M:%S'`"; \
	 echo "sc:$(SC_VERSION)"; \
	 echo "m4:$(M4_VERSION)"; \
	 echo "gcc:$(GCC_VERSION)"; \
	 echo "mggcc:$(MGGCC_VERSION)"; \
	 echo "binutils:$(BINUTILS_VERSION)"; \
	 echo "mgsim:$(MGSIM_VERSION)"; \
	 echo "slc:$(SLC_VERSION)"; \
	 echo "reqtag:$(REQBASENAME)"; \
	 echo "sltag:$(SLBASENAME)" ) >$@

uninstall-local:
	rm -rf $(REQDIR) $(SLDIR) $(REQCURRENT)
	for tag in $(DSTBASE)/tags/*; do \
	   test -r "$$tag" && $(MAKE) check-tag CHECKTAG="$$tag"; \
	done; true
	found= ; set -e; \
	  for d in $(DSTBASE)/tags/* $(DSTBASE)/sl-* $(DSTBASE)/slreqs-*; do \
	     if test -r $$d; then found=1; fi; \
	  done; \
	  rmdir $(DSTBASE)/tags || true; \
	  if test -z "$$found"; then \
	     rm -f $(DSTBASE)/slenv $(DSTBASE)/deslenv; \
	  fi; \
	  true

check-tag:
	$(AM_V_at)echo $(ECHO_N) "Trying tag: $(CHECKTAG)... $(ECHO_C)"; \
	   sltag=`grep sltag "$(CHECKTAG)"|cut -d: -f2-`; \
	   reqtag=`grep reqtag "$(CHECKTAG)"|cut -d: -f2-`; \
	   if test -d $(DSTBASE)/sl-$$sltag && test -d $(DSTBASE)/slreqs-$$reqtag; then \
	      echo "$(ECHO_T)good, keep"; \
	   else \
	      echo "$(ECHO_T)not installed, ignore"; \
	      rm -f "$(CHECKTAG)"; \
	   fi; true

moretags:
	$(AM_V_at)$(MKDIR_P) tags
	$(AM_V_at)for tag in `($(FETCH) "$(TAGS_SITE)/?F=0" || true) | grep 'href' | sed -e 's/^.*"\([^"]*\)".*$$/\1/g'|grep -v /`; do \
	   if ! $(FETCH) $(TAGS_SITE)/"$$tag" >"tags/$$tag"; then rm -f tags/"$$tag"; continue; fi; \
	   $(MAKE) check-tag CHECKTAG=tags/$$tag; \
	done; true

ALL_RULES = fetch reqs-install install moretags check-tag $(foreach P,$(PARTS),$(foreach R,fetch configure build install,$(P)-$(R)))
.PHONY: $(ALL_RULES)

