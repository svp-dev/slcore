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

### MT-naked targets

# define MTA_TEMPLATE

# # arg 1 = slc target
# # arg 2 = libdir
# # arg 3 = libdir_undescores

# nobase_pkglib_DATA += \
# 	$(2)/slrt.o \
# 	$(2)/libslc.a \
# 	$(2)/libslmain.a 

# $(3)_libslc_a_CONTENTS = \
# 	$(2)/callgate.o 

# $(3)_libslmain_a_CONTENTS = \
# 	$(2)/main.o 

# $(2)/%.a:
# 	$(AM_V_at)rm -f $@
# 	$(AM_V_AR)$(AR_MTALPHA) cru $@ $^
# 	$(AM_V_at)$(RANLIB_MTALPHA) $@

# $(2)/libslc.a: $($(3)_libslc_a_CONTENTS)
# $(2)/libslmain.a: $($(3)_libslmain_a_CONTENTS)

# SLC_CMD := $(SLC_RUN) -b $(1) -nostdlib -nostdmain

# $(2)/%.o: $(srcdir)/mtalpha-sim/%.s
# 	$(AM_V_at)$(MKDIR_P) $(2)
# 	$(slc_verbose)$(SLC_CMD) -c -o $@ $<

# $(2)/%.o: $(srcdir)/%.c
# 	$(AM_V_at)$(MKDIR_P) $(2)
# 	$(slc_verbose)$(SLC_CMD) -c -o $@ $<

# CLEANFILES += \
# 	$(2)/callgate.o \
# 	$(2)/main.o \
# 	$(2)/slrt.o \
# 	$(2)/libslc.a \
# 	$(2)/libslmain.a

# endef

### MT-hybrid targets

#$(eval $(call MTA_TEMPLATE,mta,mta_hybrid-mtalpha-sim,mta_hybrid_mtalpha_sim)

nobase_pkglib_DATA += \
	mta_hybrid-mtalpha-sim/slrt.o \
	mta_hybrid-mtalpha-sim/libslc.a \
	mta_hybrid-mtalpha-sim/libslmain.a 

mta_hybrid_mtalpha_sim_libslc_a_CONTENTS = \
	mta_hybrid-mtalpha-sim/callgate.o 

mta_hybrid_mtalpha_sim_libslmain_a_CONTENTS = \
	mta_hybrid-mtalpha-sim/main.o 

mta_hybrid-mtalpha-sim/%.a:
	$(AM_V_at)rm -f $@
	$(AM_V_AR)$(AR_MTALPHA) cru $@ $^
	$(AM_V_at)$(RANLIB_MTALPHA) $@

mta_hybrid-mtalpha-sim/libslc.a: $(mta_hybrid_mtalpha_sim_libslc_a_CONTENTS)
mta_hybrid-mtalpha-sim/libslmain.a: $(mta_hybrid_mtalpha_sim_libslmain_a_CONTENTS)

SLC_MTA := $(SLC_RUN) -b mta 

mta_hybrid-mtalpha-sim/%.o: $(srcdir)/mtalpha-sim/%.s
	$(AM_V_at)$(MKDIR_P) mta_hybrid-mtalpha-sim
	$(slc_verbose)$(SLC_MTA) -c -o $@ $<

mta_hybrid-mtalpha-sim/%.o: $(srcdir)/%.c
	$(AM_V_at)$(MKDIR_P) mta_hybrid-mtalpha-sim
	$(slc_verbose)$(SLC_MTA) -c -o $@ $<

CLEANFILES += \
	mta_hybrid-mtalpha-sim/callgate.o \
	mta_hybrid-mtalpha-sim/main.o \
	mta_hybrid-mtalpha-sim/slrt.o \
	mta_hybrid-mtalpha-sim/libslc.a \
	mta_hybrid-mtalpha-sim/libslmain.a

### seq-naked targets

#$(eval $(call MTA_TEMPLATE,mta_s,seq_naked-mtalpha-sim,seq_naked_mtalpha_sim)

nobase_pkglib_DATA += \
	seq_naked-mtalpha-sim/slrt.o \
	seq_naked-mtalpha-sim/libslc.a \
	seq_naked-mtalpha-sim/libslmain.a 

seq_naked_mtalpha_sim_libslc_a_CONTENTS = \
	seq_naked-mtalpha-sim/callgate.o 

seq_naked_mtalpha_sim_libslmain_a_CONTENTS = \
	seq_naked-mtalpha-sim/main.o 

seq_naked-mtalpha-sim/%.a:
	$(AM_V_at)rm -f $@
	$(AM_V_AR)$(AR_MTALPHA) cru $@ $^
	$(AM_V_at)$(RANLIB_MTALPHA) $@

seq_naked-mtalpha-sim/libslc.a: $(seq_naked_mtalpha_sim_libslc_a_CONTENTS)
seq_naked-mtalpha-sim/libslmain.a: $(seq_naked_mtalpha_sim_libslmain_a_CONTENTS)

SLC_MTA_S := $(SLC_RUN) -b mta_s 

seq_naked-mtalpha-sim/%.o: $(srcdir)/mtalpha-sim/%.s
	$(AM_V_at)$(MKDIR_P) seq_naked-mtalpha-sim
	$(slc_verbose)$(SLC_MTA_S) -c -o $@ $<

seq_naked-mtalpha-sim/%.o: $(srcdir)/%.c
	$(AM_V_at)$(MKDIR_P) seq_naked-mtalpha-sim
	$(slc_verbose)$(SLC_MTA_S) -c -o $@ $<

CLEANFILES += \
	seq_naked-mtalpha-sim/callgate.o \
	seq_naked-mtalpha-sim/main.o \
	seq_naked-mtalpha-sim/slrt.o \
	seq_naked-mtalpha-sim/libslc.a \
	seq_naked-mtalpha-sim/libslmain.a

### MT-naked targets

#$(eval $(call MTA_TEMPLATE,mta_n,mta_naked-mtalpha-sim,mta_naked_mtalpha_sim)

nobase_pkglib_DATA += \
	mta_naked-mtalpha-sim/slrt.o \
	mta_naked-mtalpha-sim/libslc.a \
	mta_naked-mtalpha-sim/libslmain.a 

mta_naked_mtalpha_sim_libslc_a_CONTENTS = \
	mta_naked-mtalpha-sim/callgate.o 

mta_naked_mtalpha_sim_libslmain_a_CONTENTS = \
	mta_naked-mtalpha-sim/main.o 

mta_naked-mtalpha-sim/%.a:
	$(AM_V_at)rm -f $@
	$(AM_V_AR)$(AR_MTALPHA) cru $@ $^
	$(AM_V_at)$(RANLIB_MTALPHA) $@

mta_naked-mtalpha-sim/libslc.a: $(mta_naked_mtalpha_sim_libslc_a_CONTENTS)
mta_naked-mtalpha-sim/libslmain.a: $(mta_naked_mtalpha_sim_libslmain_a_CONTENTS)

SLC_CMD := $(SLC_RUN) -b mta_n 

mta_naked-mtalpha-sim/%.o: $(srcdir)/mtalpha-sim/%.s
	$(AM_V_at)$(MKDIR_P) mta_naked-mtalpha-sim
	$(slc_verbose)$(SLC_CMD) -c -o $@ $<

mta_naked-mtalpha-sim/%.o: $(srcdir)/%.c
	$(AM_V_at)$(MKDIR_P) mta_naked-mtalpha-sim
	$(slc_verbose)$(SLC_CMD) -c -o $@ $<

CLEANFILES += \
	mta_naked-mtalpha-sim/callgate.o \
	mta_naked-mtalpha-sim/main.o \
	mta_naked-mtalpha-sim/slrt.o \
	mta_naked-mtalpha-sim/libslc.a \
	mta_naked-mtalpha-sim/libslmain.a



endif

