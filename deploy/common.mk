DISTDIR = distfiles
META_SOURCES = sl/meta
INST_SOURCES = sl/src
INST_BUILD = sl/build

$(DISTDIR) $(META_SOURCES) $(INST_SOURCES) $(INST_BUILD):
	mkdir -p $@
