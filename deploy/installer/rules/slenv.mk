EXTRA_DIST += slenv

$(DSTBASE)/slenv: slenv
	rm -f $@
	$(MKDIR_P) $(DSTBASE)
	$(INSTALL_SCRIPT) $^ $@
	chmod -w $@

slenv-fetch: ; $(RULE_DONE)
slenv-configure: ; $(RULE_DONE)
slenv-build: ; $(RULE_DONE)

slenv-install: $(DSTBASE)/slenv ; $(RULE_DONE)
