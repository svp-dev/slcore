########## Sequential-C implementation ##########

nobase_dist_pkgdata_DATA += \
	host-host-seqc/include/svp_os.h 

nobase_pkglib_LIBRARIES += host-host-seqc/libsl.a

host_host_seqc_libsl_a_SOURCES = 
host_host_seqc_libsl_a_LIBADD = \
	host-host-seqc/div.o \
	host-host-seqc/roman.o \
	host-host-seqc/gfx.o \
	host-host-seqc/io.o

SLC_SEQC = $(SLC_RUN) -b seqc -nostdlib $(AM_CFLAGS) $(CFLAGS)

host-host-seqc/%.o: $(srcdir)/src/%.c
	$(AM_V_at)$(MKDIR_P) host-host-seqc
	$(slc_verbose)$(SLC_SEQC) -c -o $@ $< 

CLEANFILES += \
	host-host-seqc/io.o \
	host-host-seqc/gfx.o \
	host-host-seqc/div.o \
	host-host-seqc/roman.o 
