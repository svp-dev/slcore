# -*- makefile -*-

SC_SRC = $(SRCBASE)/systemc-$(SC_VERSION)
SC_BUILD = $(BLDBASE)/systemc-$(SC_VERSION)
SC_CFG_TARGETS = $(SC_BUILD)/configure_done
SC_BUILD_TARGETS = $(SC_BUILD)/build_done
SC_INST_TARGETS = $(REQDIR)/.systemc-installed

.PRECIOUS: $(SC_ARCHIVE) $(SC_CFG_TARGETS) $(SC_BUILD_TARGETS) $(SC_INST_TARGETS)

$(SC_ARCHIVE):
	@echo
	@echo "* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *"
	@echo "*"
	@echo "* For licensing reasons, SystemC cannot be publicly re-distributed."
	@echo "*"
	@echo "* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *"
	@echo
	@echo "$(SC_ARCHIVE) not found."
	@echo $(ECHO_N) "Enter full path to separately-downloaded archive: $(ECHO_C)" && \
	 read arch && \
	 $(MKDIR_P) $(ARCHIVEDIR) && \
	 if test -r "$$arch"; then \
	    echo && echo "Copying $$arch -> $@..." && \
	    cp "$$arch" $@; \
	 else \
	    echo && echo "Not found or not readable: $$arch" >&2 && exit 1; \
	 fi

sc-fetch: $(SC_ARCHIVE) ; $(RULE_DONE)
sc-configure: $(SC_CFG_TARGETS) ; $(RULE_DONE)
sc-build: $(SC_BUILD_TARGETS) ; $(RULE_DONE)
sc-install: $(SC_INST_TARGETS) ; $(RULE_DONE)

# used by mggcc
sc-flags:
	@SRC=`cd $(SC_SRC) && pwd` && echo CPPFLAGS=\"-I$$SRC/src $(CPPFLAGS)\" CFLAGS=\"-I$$SRC/src $(CFLAGS)\"
	@BLD=`cd $(BLDBASE)/systemc-$(SC_VERSION) && pwd` && echo LDFLAGS=\"-L$$BLD/src $(LDFLAGS)\"

$(SC_SRC)/configure: $(SC_ARCHIVE)
	rm -f $@
	$(UNTAR) $(SRCBASE) $(SC_ARCHIVE)
	touch $@

# sysc configure needs $(REQDIR) to already exist
$(SC_BUILD)/configure_done: $(SC_SRC)/configure $(REQTAG)
	rm -f $@
	$(MKDIR_P) $(SC_BUILD)
	SRC=$$(cd $(SC_SRC) && pwd) && \
	  cd $(SC_BUILD) && \
	  find . -name config.cache -exec rm '{}' \; && \
	  $$SRC/configure --prefix=$(REQDIR) \
			       CC="$(CC)" \
			       CFLAGS="$(CPPFLAGS) $(CFLAGS)" \
	                       LDFLAGS="$(CFLAGS) $(LDFLAGS)" 
	touch $@

$(SC_BUILD)/build_done: $(SC_BUILD)/configure_done
	rm -f $@
	cd $(SC_BUILD) && \
	   if ! $(MAKE) pthreads; then \
                $(MAKE) clean; \
	        $(MAKE) ; \
           fi
	touch $@

$(REQDIR)/.systemc-installed: $(SC_BUILD)/build_done
	rm -f $@
	cd $(SC_BUILD) && $(MAKE) install
	touch $@

