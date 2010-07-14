$(DSTBASE)/slenv: slenv
	rm -f $@
	$(MKDIR_P) $(DSTBASE)
	$(INSTALL_SCRIPT) $^ $@
	chmod -w $@

$(DSTBASE)/deslenv: deslenv
	rm -f $@
	$(MKDIR_P) $(DSTBASE)
	$(INSTALL_SCRIPT) $^ $@
	chmod -w $@

$(DSTBASE)/slversion: slversion
	rm -f $@
	$(MKDIR_P) $(DSTBASE)
	$(INSTALL_SCRIPT) $^ $@
	chmod -w $@

slenv-fetch: ; $(RULE_DONE)
slenv-configure: ; $(RULE_DONE)
slenv-build: ; $(RULE_DONE)

slenv-install: $(DSTBASE)/slenv $(DSTBASE)/deslenv $(DSTBASE)/slversion; $(RULE_DONE)
