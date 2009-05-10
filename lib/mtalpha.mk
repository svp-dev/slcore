########### MT-Alpha components ###########

EXTRA_DIST += \
	mtalpha-sim/slrt.s \
	mtalpha-sim/mtdiv.s \
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
	mtalpha-sim/io.o

SLC_MTALPHA = $(SLC) -b ppp-mtalpha

mtalpha-sim/io.o: $(srcdir)/svplib/io.sl
	$(MKDIR_P) mtalpha-sim
	$(SLC_MTALPHA) -c -o $@ $<

mtalpha-sim/div.o: $(srcdir)/svplib/div.sl
	$(MKDIR_P) mtalpha-sim
	$(SLC_MTALPHA) -c -o $@ $<

mtalpha-sim/roman.o: $(srcdir)/svplib/roman.sl
	$(MKDIR_P) mtalpha-sim
	$(SLC_MTALPHA) -c -o $@ $<

mtalpha-sim/mtdiv.o: $(srcdir)/mtalpha-sim/mtdiv.s
	$(MKDIR_P) mtalpha-sim
	$(SLC_MTALPHA) -c -o $@ $<

mtalpha-sim/slrt.o: $(srcdir)/mtalpha-sim/slrt.s
	$(MKDIR_P) mtalpha-sim
	$(SLC_MTALPHA) -c -o $@ $<

mtalpha-sim/libslos.a: $(mtalpha_sim_libslos_a_CONTENTS)
	rm -f $@
	$(AR_MTALPHA) cru $@ $^
	$(RANLIB_MTALPHA) $@

endif

CLEANFILES += \
	mtalpha-sim/io.o \
	mtalpha-sim/roman.o \
	mtalpha-sim/div.o \
	mtalpha-sim/mtdiv.o \
	mtalpha-sim/slrt.o \
	mtalpha-sim/libslos.a

