########### MT-Alpha components ###########

EXTRA_DIST += \
	mtalpha-sim/slrt.s \
	mtalpha-sim/mtdiv.s \
	mtalpha-sim/callgate.s \
	mtalpha-sim/include/svp_asmdiv.h \
	mtalpha-sim/include/svp_os.h 

nobase_dist_pkgdata_DATA += \
	t-pppalpha/slimpl.m4 \
	t-pppalpha/include/sl_support.h \
	mtalpha-sim/include/svp_os.h \
	mtalpha-sim/include/svp_asmdiv.h

if ENABLE_MTALPHA

nobase_pkglib_DATA += mtalpha-sim/slrt.o

nobase_pkglib_DATA += mtalpha-sim/libslos.a

mtalpha_sim_libslos_a_CONTENTS = \
	mtalpha-sim/mtdiv.o \
	mtalpha-sim/roman.o \
	mtalpha-sim/callgate.o \
	mtalpha-sim/io.o

SLC_MTALPHA = $(SLC) -b ppp-mtalpha -nostdlib

mtalpha-sim/io.o: $(srcdir)/svplib/io.sl
	$(AM_V_at)$(MKDIR_P) mtalpha-sim
	$(slc_verbose)$(SLC_MTALPHA) -c -o $@ $<

mtalpha-sim/div.o: $(srcdir)/svplib/div.sl
	$(AM_V_at)$(MKDIR_P) mtalpha-sim
	$(slc_verbose)$(SLC_MTALPHA) -c -o $@ $<

mtalpha-sim/roman.o: $(srcdir)/svplib/roman.sl
	$(AM_V_at)$(MKDIR_P) mtalpha-sim
	$(slc_verbose)$(SLC_MTALPHA) -c -o $@ $<

mtalpha-sim/mtdiv.o: $(srcdir)/mtalpha-sim/mtdiv.s
	$(AM_V_at)$(MKDIR_P) mtalpha-sim
	$(slc_verbose)$(SLC_MTALPHA) -c -o $@ $<

mtalpha-sim/callgate.o: $(srcdir)/mtalpha-sim/callgate.s
	$(AM_V_at)$(MKDIR_P) mtalpha-sim
	$(slc_verbose)$(SLC_MTALPHA) -c -o $@ $<

mtalpha-sim/slrt.o: $(srcdir)/mtalpha-sim/slrt.s
	$(AM_V_at)$(MKDIR_P) mtalpha-sim
	$(slc_verbose)$(SLC_MTALPHA) -c -o $@ $<

mtalpha-sim/libslos.a: $(mtalpha_sim_libslos_a_CONTENTS)
	$(AM_V_at)rm -f $@
	$(AM_V_AR)$(AR_MTALPHA) cru $@ $^
	$(AM_V_at)$(RANLIB_MTALPHA) $@

endif

CLEANFILES += \
	mtalpha-sim/io.o \
	mtalpha-sim/roman.o \
	mtalpha-sim/div.o \
	mtalpha-sim/mtdiv.o \
	mtalpha-sim/callgate.o \
	mtalpha-sim/slrt.o \
	mtalpha-sim/libslos.a

