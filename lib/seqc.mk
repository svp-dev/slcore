########## Sequential-C implementation ##########

nobase_dist_pkgdata_DATA += \
	t-seqc/slimpl.m4 \
	t-seqc/include/sl_support.h \
	host-host-seqc/include/svp_os.h 

EXTRA_DIST += host-host-seqc/slrt.c 

nobase_pkglib_DATA += host-host-seqc/slrt.o

nobase_pkglib_LIBRARIES += host-host-seqc/libslos.a

host_host_seqc_libslos_a_SOURCES = 
host_host_seqc_libslos_a_LIBADD = \
	host-host-seqc/div.o \
	host-host-seqc/roman.o \
	host-host-seqc/io.o

SLC_SEQC = $(SLC) -b seqc -nostdlib

host-host-seqc/io.o: $(srcdir)/svplib/io.sl
	$(MKDIR_P) host-host-seqc
	$(SLC_SEQC) -c -o $@ $<

host-host-seqc/div.o: $(srcdir)/svplib/div.sl
	$(MKDIR_P) host-host-seqc
	$(SLC_SEQC) -c -o $@ $<

host-host-seqc/roman.o: $(srcdir)/svplib/roman.sl
	$(MKDIR_P) host-host-seqc
	$(SLC_SEQC) -c -o $@ $<

host-host-seqc/slrt.o: $(srcdir)/host-host-seqc/slrt.c
	$(MKDIR_P) host-host-seqc
	$(CC) $(CFLAGS) -I$(srcdir)/t-seqc/include -c -o $@ $<

CLEANFILES += \
	host-host-seqc/io.o \
	host-host-seqc/div.o \
	host-host-seqc/roman.o \
	host-host-seqc/slrt.o
