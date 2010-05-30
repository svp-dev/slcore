########## Sequential-C implementation ##########

nobase_dist_pkgdata_DATA += \
	t-seqc/slimpl.m4 \
	t-seqc/include/sl_support.h

EXTRA_DIST += \
	host-host-seqc/slrt.c 

nobase_pkglib_DATA += \
	host-host-seqc/slrt.o

nobase_pkglib_LIBRARIES += \
	host-host-seqc/libslc.a \
	host-host-seqc/libslmain.a

host_host_seqc_libslc_a_SOURCES = empty.c # empty for now
host_host_seqc_libslmain_a_SOURCES = empty.c # empty for now
host_host_seqc_libslmain_a_LIBADD = host-host-seqc/main.o

SLC_SEQC = $(SLC_RUN) -b seqc -nostdlib -nodefaultmain

host-host-seqc/%.o: $(srcdir)/host-host-seqc/%.c
	$(AM_V_at)$(MKDIR_P) host-host-seqc
	$(slc_verbose)$(SLC_SEQC) -I$(srcdir)/t-seqc/include -c -o $@ $< $(AM_CFLAGS) $(CFLAGS)

host-host-seqc/%.o: $(srcdir)/%.c
	$(AM_V_at)$(MKDIR_P) host-host-seqc
	$(slc_verbose)$(SLC_SEQC) -I$(srcdir)/t-seqc/include -c -o $@ $< $(AM_CFLAGS) $(CFLAGS)

CLEANFILES += \
	host-host-seqc/slrt.o 


