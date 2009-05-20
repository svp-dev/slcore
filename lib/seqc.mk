########## Sequential-C implementation ##########

nobase_dist_pkgdata_DATA += \
	t-seqc/slimpl.m4 \
	t-seqc/include/sl_support.h \
	host-host-seqc/include/svp_os.h 

EXTRA_DIST += host-host-seqc/slrt.c host-host-seqc/load.c

nobase_pkglib_DATA += host-host-seqc/slrt.o

nobase_pkglib_LIBRARIES += host-host-seqc/libslos.a

host_host_seqc_libslos_a_SOURCES = 
host_host_seqc_libslos_a_LIBADD = \
	host-host-seqc/div.o \
	host-host-seqc/roman.o \
	host-host-seqc/io.o

SLC_SEQC = $(SLC) -b seqc -nostdlib $(AM_CFLAGS) $(CFLAGS)

host-host-seqc/io.o: $(srcdir)/svplib/io.sl
	$(AM_V_at)$(MKDIR_P) host-host-seqc
	$(slc_verbose)$(SLC_SEQC) -c -o $@ $<

host-host-seqc/div.o: $(srcdir)/svplib/div.sl
	$(AM_V_at)$(MKDIR_P) host-host-seqc
	$(slc_verbose)$(SLC_SEQC) -c -o $@ $<

host-host-seqc/roman.o: $(srcdir)/svplib/roman.sl
	$(AM_V_at)$(MKDIR_P) host-host-seqc
	$(slc_verbose)$(SLC_SEQC) -c -o $@ $<

host-host-seqc/slrt.o: $(srcdir)/host-host-seqc/slrt.c $(srcdir)/host-host-seqc/load.c
	$(AM_V_at)$(MKDIR_P) host-host-seqc
	$(slc_verbose)$(CC) $(CFLAGS) -I$(srcdir)/t-seqc/include -c -o $@ $< $(AM_CFLAGS) $(CFLAGS)

CLEANFILES += \
	host-host-seqc/io.o \
	host-host-seqc/div.o \
	host-host-seqc/roman.o \
	host-host-seqc/slrt.o
