EXTRA_DIST = \
	host-host-seqc/slrt.c \
	host-host-ptl/slrt.c \
	host-host-ptl/include/create.tpl \
	host-host-ptl/include/template.pl \
	mtalpha-sim/slrt.s \
	mtalpha-sim/include/svp_asmdiv.h \
	mtalpha-sim/include/svp_os.h \
	svplib/io.sl \
	svplib/div.sl

nobase_dist_pkglib_SCRIPTS = \
	r-host \
	r-mtalpha-sim

nobase_dist_pkgdata_DATA = \
	t-seqc/slimpl.m4 \
	t-seqc/include/sl_support.h \
	t-ptl/slimpl.m4 \
	t-ptl/include/sl_support.h \
	t-utc/slimpl.m4 \
	t-utc0/slimpl.m4 \
	t-utc0/include/sl_support.h \
	t-pppalpha/slimpl.m4 \
	t-pppalpha/include/sl_support.h \
	host-host-seqc/include/svp_os.h \
	host-host-ptl/include/ptl_create.h \
	host-host-ptl/include/ptl_config.h \
	host-host-ptl/include/ptl_debug.h \
	host-host-ptl/include/ptl_deps.h \
	host-host-ptl/include/ptl_svp.h \
	host-host-ptl/include/ptl_vars.h \
	host-host-ptl/include/svp_os.h \
	mtalpha-sim/include/svp_os.h \
	mtalpha-sim/include/svp_asmdiv.h

nobase_pkglib_DATA = \
	host-host-seqc/slrt.o \
	host-host-ptl/slrt.o \
	mtalpha-sim/slrt.o

nobase_pkglib_LIBRARIES = \
	host-host-ptl/libslos.a \
	host-host-seqc/libslos.a

nobase_pkglib_DATA += \
	mtalpha-sim/libslos.a
mtalpha_sim_libslos_a_CONTENTS = \
	mtalpha-sim/div.o \
	mtalpha-sim/io.o

host_host_seqc_libslos_a_SOURCES = 
host_host_seqc_libslos_a_LIBADD = \
	host-host-seqc/div.o \
	host-host-seqc/io.o

host_host_ptl_libslos_a_CPPFLAGS = \
	-I$(srcdir)/host-host-ptl/include \
	-I$(builddir)/host-host-ptl/include

host_host_ptl_libslos_a_SOURCES = \
	host-host-ptl/ptl_debug.cc \
	host-host-ptl/ptl_vars.cc \
	host-host-ptl/ptl_svp.cc
host_host_ptl_libslos_a_LIBADD = \
	host-host-ptl/io.o \
	host-host-ptl/div.o

BUILT_SOURCES = \
	host-host-ptl/include/ptl_create.h

CLEANFILES = 
SLC = $(top_builddir)/bin/slc.local

######## mtalpha-specific objects #######
mtalpha-sim/io.o: $(srcdir)/svplib/io.sl
	$(MKDIR_P) mtalpha-sim
	$(SLC) -b ppp-mtalpha -c -o $@ $<

mtalpha-sim/div.o: $(srcdir)/svplib/div.sl
	$(MKDIR_P) mtalpha-sim
	$(SLC) -b ppp-mtalpha -c -o $@ $<

mtalpha-sim/slrt.o: $(srcdir)/mtalpha-sim/slrt.s
	$(MKDIR_P) mtalpha-sim
	$(SLC) -b ppp-mtalpha -c -o $@ $<

mtalpha-sim/libslos.a: $(mtalpha_sim_libslos_a_CONTENTS)
	rm -f $@
	$(MTALPHA_AR) cru $@ $^
	$(MTALPHA_RANLIB) $@

CLEANFILES += \
	mtalpha-sim/io.o \
	mtalpha-sim/div.o \
	mtalpha-sim/slrt.o \
	mtalpha-sim/libslos.a

######## seqc-specific objects ########

host-host-seqc/io.o: $(srcdir)/svplib/io.sl
	$(MKDIR_P) host-host-seqc
	$(SLC) -b seqc -c -o $@ $<

host-host-seqc/div.o: $(srcdir)/svplib/div.sl
	$(MKDIR_P) host-host-seqc
	$(SLC) -b seqc -c -o $@ $<

host-host-seqc/slrt.o: $(srcdir)/host-host-seqc/slrt.c
	$(MKDIR_P) host-host-seqc
	$(CC) $(CFLAGS) -I$(srcdir)/t-seqc/include -c -o $@ $<

CLEANFILES += \
	host-host-seqc/io.o \
	host-host-seqc/div.o \
	host-host-seqc/slrt.o

######## ptl-specific objects ########

host-host-ptl/io.o: $(srcdir)/svplib/io.sl
	$(MKDIR_P) host-host-ptl
	$(SLC) -b ptl -c -o $@ $<

host-host-ptl/div.o: $(srcdir)/svplib/div.sl
	$(MKDIR_P) host-host-ptl
	$(SLC) -b ptl -c -o $@ $<

host-host-ptl/slrt.o: $(srcdir)/host-host-ptl/slrt.c
	$(MKDIR_P) host-host-ptl
	$(CC) $(CFLAGS) -I$(srcdir)/t-ptl/include -c -o $@ $<

CLEANFILES += \
	host-host-ptl/io.o \
	host-host-ptl/div.o \
	host-host-ptl/slrt.o

PTL_CREATE_SUBST = $(srcdir)/host-host-ptl/include/template.pl
PTL_CREATE_TEMPL = $(srcdir)/host-host-ptl/include/create.tpl

host-host-ptl/include/ptl_create.h: $(PTL_CREATE_SUBST) $(PTL_CREATE_TEMPL)
	$(MKDIR_P) host-host-ptl/include
	$(PERL) $(PTL_CREATE_SUBST) <$(PTL_CREATE_TEMPL) >$@

MAINTAINERCLEANFILES = host-host-ptl/include/ptl_create.h
