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
	host-host-ptl/include/ptl_vars.h \
	host-host-ptl/include/svp_os.h

EXTRA_DIST += \
	host-host-ptl/slrt.c \
	host-host-ptl/include/create.tpl \
	host-host-ptl/include/template.pl

nobase_pkglib_DATA += host-host-ptl/slrt.o

nobase_pkglib_LIBRARIES += host-host-ptl/libslos.a 

host_host_ptl_libslos_a_CPPFLAGS = \
	-I$(srcdir)/host-host-ptl/include \
	-I$(builddir)/host-host-ptl/include

host_host_ptl_libslos_a_SOURCES = \
	host-host-ptl/ptl_debug.cc \
	host-host-ptl/ptl_vars.cc \
	host-host-ptl/ptl_svp.cc
host_host_ptl_libslos_a_LIBADD = \
	host-host-ptl/io.o \
	host-host-ptl/roman.o \
	host-host-ptl/div.o

BUILT_SOURCES += \
	host-host-ptl/include/ptl_create.h

SLC_PTL = $(SLC) -b ptl -nostdlib

host-host-ptl/io.o: $(srcdir)/svplib/io.sl
	$(MKDIR_P) host-host-ptl
	$(SLC_PTL) -c -o $@ $<

host-host-ptl/div.o: $(srcdir)/svplib/div.sl
	$(MKDIR_P) host-host-ptl
	$(SLC_PTL) -c -o $@ $<

host-host-ptl/roman.o: $(srcdir)/svplib/roman.sl
	$(MKDIR_P) host-host-ptl
	$(SLC_PTL) -c -o $@ $<

host-host-ptl/slrt.o: $(srcdir)/host-host-ptl/slrt.c
	$(MKDIR_P) host-host-ptl
	$(CC) $(CFLAGS) -I$(srcdir)/t-ptl/include -c -o $@ $<

CLEANFILES += \
	host-host-ptl/io.o \
	host-host-ptl/div.o \
	host-host-ptl/roman.o \
	host-host-ptl/slrt.o

PTL_CREATE_SUBST = $(srcdir)/host-host-ptl/include/template.pl
PTL_CREATE_TEMPL = $(srcdir)/host-host-ptl/include/create.tpl

host-host-ptl/include/ptl_create.h: $(PTL_CREATE_SUBST) $(PTL_CREATE_TEMPL)
	$(MKDIR_P) host-host-ptl/include
	$(PERL) $(PTL_CREATE_SUBST) <$(PTL_CREATE_TEMPL) >$@

MAINTAINERCLEANFILES += host-host-ptl/include/ptl_create.h

