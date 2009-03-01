EXTRA_DIST = \
	r-host \
	t-seqc/protoimpl.m4 \
	host-host-seqc/slrt.c \
	t-ptl/protoimpl.m4 \
	host-host-ptl/slrt.c \
	host-host-ptl/include/create.tpl \
	host-host-ptl/include/template.pl \
	svplib/io.sl

nobase_dist_pkgdata_DATA = \
	host-host-seqc/include/sl_support.h \
	host-host-seqc/include/svp_os.h \
	host-host-ptl/include/ptl_create.h \
	host-host-ptl/include/ptl_config.h \
	host-host-ptl/include/ptl_debug.h \
	host-host-ptl/include/ptl_deps.h \
	host-host-ptl/include/ptl_svp.h \
	host-host-ptl/include/ptl_vars.h \
	host-host-ptl/include/sl_support.h \
	host-host-ptl/include/svp_os.h

nobase_pkgdata_DATA = \
	host-host-seqc/slrt.o \
	host-host-ptl/slrt.o

nobase_pkgdata_LIBRARIES = \
	host-host-ptl/libslos.a \
	host-host-seqc/libslos.a

host_host_seqc_libslos_a_SOURCES = 
host_host_seqc_libslos_a_LIBADD = \
	host-host-seqc/io.o

host_host_ptl_libslos_a_CPPFLAGS = \
	-I$(srcdir)/host-host-ptl/include \
	-I$(builddir)/host-host-ptl/include

host_host_ptl_libslos_a_SOURCES = \
	host-host-ptl/ptl_debug.cc \
	host-host-ptl/ptl_vars.cc \
	host-host-ptl/ptl_svp.cc
host_host_ptl_libslos_a_LIBADD = \
	host-host-ptl/io.o

BUILT_SOURCES = \
	host-host-ptl/include/ptl_create.h

CLEANFILES = 
SLC = $(top_builddir)/bin/slc.local

######## seqc-specific objects ########

host-host-seqc/io.o: $(srcdir)/svplib/io.sl
	$(MKDIR_P) host-host-seqc
	$(SLC) -b seqc -c -o $@ $<

host-host-seqc/slrt.o: $(srcdir)/host-host-seqc/slrt.c
	$(MKDIR_P) host-host-seqc
	$(CC) $(CFLAGS) -c -o $@ $<

CLEANFILES += \
	host-host-seqc/io.o \
	host-host-seqc/slrt.o

######## ptl-specific objects ########

host-host-ptl/io.o: $(srcdir)/svplib/io.sl
	$(MKDIR_P) host-host-ptl
	$(SLC) -b ptl -c -o $@ $<

host-host-ptl/slrt.o: $(srcdir)/host-host-ptl/slrt.c
	$(MKDIR_P) host-host-seqc
	$(CC) $(CFLAGS) -c -o $@ $<

CLEANFILES += \
	host-host-ptl/io.o \
	host-host-ptl/slrt.o

PTL_CREATE_SUBST = $(srcdir)/host-host-ptl/include/template.pl
PTL_CREATE_TEMPL = $(srcdir)/host-host-ptl/include/create.tpl

host-host-ptl/include/ptl_create.h: $(PTL_CREATE_SUBST) $(PTL_CREATE_TEMPL)
	$(MKDIR_P) host-host-ptl/include
	$(PERL) $(PTL_CREATE_SUBST) <$(PTL_CREATE_TEMPL) >$@

MAINTAINERCLEANFILES = host-host-ptl/include/ptl_create.h
