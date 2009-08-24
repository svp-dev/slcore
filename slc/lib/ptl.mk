########## muTC-PTL implementation ##########

nobase_dist_pkgdata_DATA += \
	t-ptl/slimpl.m4 \
	t-ptl/include/sl_support.h \
	host-host-ptl/include/create.tpl \
	host-host-ptl/include/template.pl \
	host-host-ptl/include/ptl_create.h \
	host-host-ptl/include/ptl_config.h \
	host-host-ptl/include/ptl_debug.h \
	host-host-ptl/include/ptl_deps.h \
	host-host-ptl/include/ptl_svp.h \
	host-host-ptl/include/ptl_vars.h

EXTRA_DIST += \
	host-host-ptl/slrt.c \
	host-host-ptl/include/create.tpl \
	host-host-ptl/include/template.pl

nobase_pkglib_DATA += host-host-ptl/slrt.o
nobase_pkglib_LIBRARIES += \
	host-host-ptl/libslc.a 

host_host_ptl_libslc_a_CPPFLAGS = \
	-I$(srcdir)/host-host-ptl/include \
	-I$(builddir)/host-host-ptl/include

host_host_ptl_libslc_a_SOURCES = \
	host-host-ptl/ptl_debug.cc \
	host-host-ptl/ptl_vars.cc \
	host-host-ptl/ptl_svp.cc \
	host-host-seqc/main.c \
	host-host-seqc/load.c

BUILT_SOURCES += \
	host-host-ptl/include/ptl_create.h


SLC_PTL = $(SLC_RUN) -b ptl -nostdlib

host-host-ptl/%.o: $(srcdir)/host-host-ptl/%.c
	$(AM_V_at)$(MKDIR_P) host-host-ptl
	$(slc_verbose)$(SLC_PTL) -I$(srcdir)/t-ptl/include -c -o $@ $< $(AM_CXXFLAGS) $(CXXFLAGS)

CLEANFILES += \
	host-host-ptl/slrt.o

PTL_CREATE_SUBST = $(srcdir)/host-host-ptl/include/template.pl
PTL_CREATE_TEMPL = $(srcdir)/host-host-ptl/include/create.tpl

host-host-ptl/include/ptl_create.h: $(PTL_CREATE_SUBST) $(PTL_CREATE_TEMPL)
	$(AM_V_at)$(MKDIR_P) host-host-ptl/include
	$(AM_V_GEN)$(PERL) $(PTL_CREATE_SUBST) <$(PTL_CREATE_TEMPL) >$@

MAINTAINERCLEANFILES += host-host-ptl/include/ptl_create.h
