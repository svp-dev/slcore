########### MT-Alpha components ###########

EXTRA_DIST += \
	mtalpha-sim/slrt.s \
	mtalpha-sim/callgate.s

nobase_dist_pkgdata_DATA += \
	t-pppalpha/slimpl.m4 \
	t-pppalpha/include/sl_support.h

if ENABLE_MTALPHA

nobase_pkglib_DATA += mtalpha-sim/slrt.o

nobase_pkglib_LIBRARIES += mtalpha-sim/libslc.a


mtalpha_sim_libslc_a_SOURCES = 
mtalpha_sim_libslc_a_CONTENTS = \
	mtalpha-sim/callgate.o 

mtalpha-sim/libslc.a: $(mtalpha_sim_libslc_a_CONTENTS)
	$(AM_V_at)rm -f $@
	$(AM_V_AR)$(AR_MTALPHA) cru $@ $^
	$(AM_V_at)$(RANLIB_MTALPHA) $@

SLC_MTALPHA = $(SLC_RUN) -b ppp-mtalpha -nostdlib

mtalpha-sim/callgate.o: $(srcdir)/mtalpha-sim/callgate.s
	$(AM_V_at)$(MKDIR_P) mtalpha-sim
	$(slc_verbose)$(SLC_MTALPHA) -c -o $@ $<

mtalpha-sim/slrt.o: $(srcdir)/mtalpha-sim/slrt.s
	$(AM_V_at)$(MKDIR_P) mtalpha-sim
	$(slc_verbose)$(SLC_MTALPHA) -c -o $@ $<

endif

CLEANFILES += \
	mtalpha-sim/callgate.o \
	mtalpha-sim/slrt.o \
	mtalpha-sim/libslc.a
