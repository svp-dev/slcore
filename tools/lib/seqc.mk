########## Sequential-C implementation ##########

nobase_dist_pkgdata_DATA += \
	t-seqc/slimpl.m4 \
	t-seqc/include/sl_support.h

EXTRA_DIST += host-host-seqc/slrt.c 

nobase_pkglib_DATA += \
	host-host-seqc/slrt.o \
	host-host-seqc/libslc.a

host_host_seqc_libslc_a_CONTENTS = \
	host-host-seqc/load.o \
	host-host-seqc/lib_main.o \
	host-host-seqc/main.o

host-host-seqc/%.a:
	$(AM_V_at)rm -f $@
	$(AM_V_AR)$(AR) cru $@ $^
	$(AM_V_at)$(RANLIB) $@

host-host-seqc/libslc.a: $(host_host_seqc_libslc_a_CONTENTS)

SLC_SEQC = $(SLC_RUN) -b seqc -nostdlib

host-host-seqc/%.o: $(srcdir)/host-host-seqc/%.c
	$(AM_V_at)$(MKDIR_P) host-host-seqc
	$(slc_verbose)$(SLC_SEQC) -I$(srcdir)/t-seqc/include -c -o $@ $< $(AM_CFLAGS) $(CFLAGS)

CLEANFILES += \
	host-host-seqc/slrt.o \
	host-host-seqc/load.o \
	host-host-seqc/main.o \
	host-host-seqc/lib_main.o \
	host-host-seqc/libslc.a 
