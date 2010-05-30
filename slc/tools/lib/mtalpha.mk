########### MT-Alpha components ###########

EXTRA_DIST += \
	mtalpha-sim/slrt.s \
	mtalpha-sim/callgate.s

nobase_dist_pkgdata_DATA += \
	t-spr/slimpl.m4 \
	t-spr/include/sl_support.h \
	t-sppalpha/slimpl.m4 \
	t-sppalpha/include/sl_support.h

if ENABLE_SLC_MTALPHA

nobase_pkglib_DATA += \
	mtalpha-sim/slrt.o \
	mtalpha-sim/libslc.a \
	mtalpha-sim/libslmain.a \
	mtalpha-sim/libslmain_seq.a


mtalpha_sim_libslc_a_CONTENTS = \
	mtalpha-sim/callgate.o 

mtalpha_sim_libslmain_a_CONTENTS = \
	mtalpha-sim/main.o 

mtalpha_sim_libslmain_seq_a_CONTENTS = \
	mtalpha-sim/main_seq.o

mtalpha-sim/%.a:
	$(AM_V_at)rm -f $@
	$(AM_V_AR)$(AR_MTALPHA) cru $@ $^
	$(AM_V_at)$(RANLIB_MTALPHA) $@

mtalpha-sim/libslc.a: $(mtalpha_sim_libslc_a_CONTENTS)
mtalpha-sim/libslmain.a: $(mtalpha_sim_libslmain_a_CONTENTS)
mtalpha-sim/libslmain_seq.a: $(mtalpha_sim_libslmain_seq_a_CONTENTS)

SLC_MTALPHA = $(SLC_RUN) -b mtalpha -nostdlib
SLC_MTA_SEQ = $(SLC_RUN) -b spr+seq-mtalpha-sim -nostdlib

mtalpha-sim/%.o: $(srcdir)/mtalpha-sim/%.s
	$(AM_V_at)$(MKDIR_P) mtalpha-sim
	$(slc_verbose)$(SLC_MTALPHA) -c -o $@ $<

mtalpha-sim/%.o: $(srcdir)/%.c
	$(AM_V_at)$(MKDIR_P) mtalpha-sim
	$(slc_verbose)$(SLC_MTALPHA) -c -o $@ $<

mtalpha-sim/main_seq.o: $(srcdir)/main.c
	$(AM_V_at)$(MKDIR_P) mtalpha-sim
	$(slc_verbose)$(SLC_MTA_SEQ) -c -o $@ $<

CLEANFILES += \
	mtalpha-sim/callgate.o \
	mtalpha-sim/main.o \
	mtalpha-sim/main_seq.o \
	mtalpha-sim/slrt.o \
	mtalpha-sim/libslc.a \
	mtalpha-sim/libslmain.a \
	mtalpha-sim/libslmain_seq.a

endif

