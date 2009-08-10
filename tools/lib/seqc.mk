########## Sequential-C implementation ##########

nobase_dist_pkgdata_DATA += \
	t-seqc/slimpl.m4 \
	t-seqc/include/sl_support.h

EXTRA_DIST += host-host-seqc/slrt.c host-host-seqc/load.c

nobase_pkglib_DATA += host-host-seqc/slrt.o

SLC_SEQC = $(SLC_RUN) -b seqc -nostdlib

host-host-seqc/slrt.o: $(srcdir)/host-host-seqc/slrt.c $(srcdir)/host-host-seqc/load.c
	$(AM_V_at)$(MKDIR_P) host-host-seqc
	$(slc_verbose)$(SLC_SEQC) -I$(srcdir)/t-seqc/include -c -o $@ $< $(AM_CFLAGS) $(CFLAGS)

CLEANFILES += \
	host-host-seqc/slrt.o
