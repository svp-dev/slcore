########## Sequential-C implementation ##########

nobase_dist_pkgdata_DATA += \
	host-host-seqc/include/svp_os.h 

nobase_pkglib_LIBRARIES += host-host-seqc/libsl.a

host_host_seqc_libsl_a_SOURCES = 
host_host_seqc_libsl_a_LIBADD = \
	host-host-seqc/div.o \
	host-host-seqc/roman.o \
	host-host-seqc/io.o

SLC_SEQC = $(SLC) -b seqc -nostdlib $(AM_CFLAGS) $(CFLAGS)

host-host-seqc/io.o: $(srcdir)/src/io.sl
	$(AM_V_at)$(MKDIR_P) host-host-seqc
	$(slc_verbose)$(SLC_SEQC) -c -o $@ $<

host-host-seqc/div.o: $(srcdir)/src/div.sl
	$(AM_V_at)$(MKDIR_P) host-host-seqc
	$(slc_verbose)$(SLC_SEQC) -c -o $@ $<

host-host-seqc/roman.o: $(srcdir)/src/roman.sl
	$(AM_V_at)$(MKDIR_P) host-host-seqc
	$(slc_verbose)$(SLC_SEQC) -c -o $@ $<

CLEANFILES += \
	host-host-seqc/io.o \
	host-host-seqc/div.o \
	host-host-seqc/roman.o 
