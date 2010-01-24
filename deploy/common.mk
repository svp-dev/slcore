DISTDIR = distfiles
META_SOURCES = sl/meta
INST_SOURCES = sl/src
INST_BUILD = sl/build

$(DISTDIR): ; mkdir -p $@
$(META_SOURCES): ; mkdir -p $@
$(INST_SOURCES): ; mkdir -p $@
$(INST_BUILD): ; mkdir -p $@
$(REQDIR): ; mkdir -p $@

