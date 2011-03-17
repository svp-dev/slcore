########### MT-Alpha components ###########

EXTRA_DIST += \
	mtsparc-fpga/slrt.s \
	mtsparc-fpga/callgate.s \
	mtsparc-sim/slrt.s \
	mtsparc-sim/callgate.s

if ENABLE_SLC_MTSPARC

### MT-naked targets

# define MTS_TEMPLATE

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
# 	$(AM_V_AR)$(AR_MTSPARC) cru $@ $^
# 	$(AM_V_at)$(RANLIB_MTSPARC) $@

# $(2)/libslc.a: $($(3)_libslc_a_CONTENTS)
# $(2)/libslmain.a: $($(3)_libslmain_a_CONTENTS)

# SLC_CMD := $(SLC_RUN) -b $(1) -nostdlib -nostdmain

# $(2)/%.o: $(srcdir)/mtsparc-fpga/%.s
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

#$(eval $(call MTS_TEMPLATE,mts,mts_hybrid-mtsparc-fpga,mts_hybrid_mtsparc_fpga)

nobase_pkglib_DATA += \
	mts_hybrid-mtsparc-fpga/slrt.o \
	mts_hybrid-mtsparc-fpga/libslc.a \
	mts_hybrid-mtsparc-fpga/libslmain.a 

mts_hybrid_mtsparc_fpga_libslc_a_CONTENTS = \
	mts_hybrid-mtsparc-fpga/callgate.o 

mts_hybrid_mtsparc_fpga_libslmain_a_CONTENTS = \
	mts_hybrid-mtsparc-fpga/main.o 

mts_hybrid-mtsparc-fpga/%.a:
	$(AM_V_at)rm -f $@
	$(AM_V_AR)$(AR_MTSPARC) cru $@ $^
	$(AM_V_at)$(RANLIB_MTSPARC) $@

mts_hybrid-mtsparc-fpga/libslc.a: $(mts_hybrid_mtsparc_fpga_libslc_a_CONTENTS)
mts_hybrid-mtsparc-fpga/libslmain.a: $(mts_hybrid_mtsparc_fpga_libslmain_a_CONTENTS)

SLC_MTS := $(SLC_RUN) -b mts -nostdlib

mts_hybrid-mtsparc-fpga/%.o: $(srcdir)/mtsparc-fpga/%.s
	$(AM_V_at)$(MKDIR_P) mts_hybrid-mtsparc-fpga
	$(slc_verbose)$(SLC_MTS) -c -o $@ $<

mts_hybrid-mtsparc-fpga/%.o: $(srcdir)/%.c
	$(AM_V_at)$(MKDIR_P) mts_hybrid-mtsparc-fpga
	$(slc_verbose)$(SLC_MTS) -c -o $@ $<

CLEANFILES += \
	mts_hybrid-mtsparc-fpga/callgate.o \
	mts_hybrid-mtsparc-fpga/main.o \
	mts_hybrid-mtsparc-fpga/slrt.o \
	mts_hybrid-mtsparc-fpga/libslc.a \
	mts_hybrid-mtsparc-fpga/libslmain.a

### seq-naked targets

#$(eval $(call MTS_TEMPLATE,mts_s,seq_naked-mtsparc-fpga,seq_naked_mtsparc_fpga)

nobase_pkglib_DATA += \
	seq_naked-mtsparc-fpga/slrt.o \
	seq_naked-mtsparc-fpga/libslc.a \
	seq_naked-mtsparc-fpga/libslmain.a 

seq_naked_mtsparc_fpga_libslc_a_CONTENTS = \
	seq_naked-mtsparc-fpga/callgate.o 

seq_naked_mtsparc_fpga_libslmain_a_CONTENTS = \
	seq_naked-mtsparc-fpga/main.o 

seq_naked-mtsparc-fpga/%.a:
	$(AM_V_at)rm -f $@
	$(AM_V_AR)$(AR_MTSPARC) cru $@ $^
	$(AM_V_at)$(RANLIB_MTSPARC) $@

seq_naked-mtsparc-fpga/libslc.a: $(seq_naked_mtsparc_fpga_libslc_a_CONTENTS)
seq_naked-mtsparc-fpga/libslmain.a: $(seq_naked_mtsparc_fpga_libslmain_a_CONTENTS)

SLC_MTS_S := $(SLC_RUN) -b mts_s -nostdlib

seq_naked-mtsparc-fpga/%.o: $(srcdir)/mtsparc-fpga/%.s
	$(AM_V_at)$(MKDIR_P) seq_naked-mtsparc-fpga
	$(slc_verbose)$(SLC_MTS_S) -c -o $@ $<

seq_naked-mtsparc-fpga/%.o: $(srcdir)/%.c
	$(AM_V_at)$(MKDIR_P) seq_naked-mtsparc-fpga
	$(slc_verbose)$(SLC_MTS_S) -c -o $@ $<

CLEANFILES += \
	seq_naked-mtsparc-fpga/callgate.o \
	seq_naked-mtsparc-fpga/main.o \
	seq_naked-mtsparc-fpga/slrt.o \
	seq_naked-mtsparc-fpga/libslc.a \
	seq_naked-mtsparc-fpga/libslmain.a

### MT-naked targets

#$(eval $(call MTS_TEMPLATE,mts_n,mts_naked-mtsparc-fpga,mts_naked_mtsparc_fpga)

nobase_pkglib_DATA += \
	mts_naked-mtsparc-fpga/slrt.o \
	mts_naked-mtsparc-fpga/libslc.a \
	mts_naked-mtsparc-fpga/libslmain.a 

mts_naked_mtsparc_fpga_libslc_a_CONTENTS = \
	mts_naked-mtsparc-fpga/callgate.o 

mts_naked_mtsparc_fpga_libslmain_a_CONTENTS = \
	mts_naked-mtsparc-fpga/main.o 

mts_naked-mtsparc-fpga/%.a:
	$(AM_V_at)rm -f $@
	$(AM_V_AR)$(AR_MTSPARC) cru $@ $^
	$(AM_V_at)$(RANLIB_MTSPARC) $@

mts_naked-mtsparc-fpga/libslc.a: $(mts_naked_mtsparc_fpga_libslc_a_CONTENTS)
mts_naked-mtsparc-fpga/libslmain.a: $(mts_naked_mtsparc_fpga_libslmain_a_CONTENTS)

SLC_MTS_N := $(SLC_RUN) -b mts_n -nostdlib

mts_naked-mtsparc-fpga/%.o: $(srcdir)/mtsparc-fpga/%.s
	$(AM_V_at)$(MKDIR_P) mts_naked-mtsparc-fpga
	$(slc_verbose)$(SLC_MTS_N) -c -o $@ $<

mts_naked-mtsparc-fpga/%.o: $(srcdir)/%.c
	$(AM_V_at)$(MKDIR_P) mts_naked-mtsparc-fpga
	$(slc_verbose)$(SLC_MTS_N) -c -o $@ $<

CLEANFILES += \
	mts_naked-mtsparc-fpga/main.o \
	mts_naked-mtsparc-fpga/slrt.o \
	mts_naked-mtsparc-fpga/callgate.o \
	mts_naked-mtsparc-fpga/libslc.a \
	mts_naked-mtsparc-fpga/libslmain.a


### MT-hybrid targets

#$(eval $(call MTSN_TEMPLATE,mts,mtsn_hybrid-mtsparc-sim,mtsn_hybrid_mtsparc_sim)

nobase_pkglib_DATA += \
	mtsn_hybrid-mtsparc-sim/slrt.o \
	mtsn_hybrid-mtsparc-sim/libslc.a \
	mtsn_hybrid-mtsparc-sim/libslmain.a 

mtsn_hybrid_mtsparc_sim_libslc_a_CONTENTS = \
	mtsn_hybrid-mtsparc-sim/callgate.o 

mtsn_hybrid_mtsparc_sim_libslmain_a_CONTENTS = \
	mtsn_hybrid-mtsparc-sim/main.o 

mtsn_hybrid-mtsparc-sim/%.a:
	$(AM_V_at)rm -f $@
	$(AM_V_AR)$(AR_MTSPARC) cru $@ $^
	$(AM_V_at)$(RANLIB_MTSPARC) $@

mtsn_hybrid-mtsparc-sim/libslc.a: $(mtsn_hybrid_mtsparc_sim_libslc_a_CONTENTS)
mtsn_hybrid-mtsparc-sim/libslmain.a: $(mtsn_hybrid_mtsparc_sim_libslmain_a_CONTENTS)

SLC_MTSN := $(SLC_RUN) -b mtsn -nostdlib

mtsn_hybrid-mtsparc-sim/%.o: $(srcdir)/mtsparc-sim/%.s
	$(AM_V_at)$(MKDIR_P) mtsn_hybrid-mtsparc-sim
	$(slc_verbose)$(SLC_MTS) -c -o $@ $<

mtsn_hybrid-mtsparc-sim/%.o: $(srcdir)/%.c
	$(AM_V_at)$(MKDIR_P) mtsn_hybrid-mtsparc-sim
	$(slc_verbose)$(SLC_MTS) -c -o $@ $<

CLEANFILES += \
	mtsn_hybrid-mtsparc-sim/callgate.o \
	mtsn_hybrid-mtsparc-sim/main.o \
	mtsn_hybrid-mtsparc-sim/slrt.o \
	mtsn_hybrid-mtsparc-sim/libslc.a \
	mtsn_hybrid-mtsparc-sim/libslmain.a

### seq-naked targets

#$(eval $(call MTSN_TEMPLATE,mtsn_s,seq_naked-mtsparc-sim,seq_naked_mtsparc_sim)

nobase_pkglib_DATA += \
	seq_naked-mtsparc-sim/slrt.o \
	seq_naked-mtsparc-sim/libslc.a \
	seq_naked-mtsparc-sim/libslmain.a 

seq_naked_mtsparc_sim_libslc_a_CONTENTS = \
	seq_naked-mtsparc-sim/callgate.o 

seq_naked_mtsparc_sim_libslmain_a_CONTENTS = \
	seq_naked-mtsparc-sim/main.o 

seq_naked-mtsparc-sim/%.a:
	$(AM_V_at)rm -f $@
	$(AM_V_AR)$(AR_MTSPARC) cru $@ $^
	$(AM_V_at)$(RANLIB_MTSPARC) $@

seq_naked-mtsparc-sim/libslc.a: $(seq_naked_mtsparc_sim_libslc_a_CONTENTS)
seq_naked-mtsparc-sim/libslmain.a: $(seq_naked_mtsparc_sim_libslmain_a_CONTENTS)

SLC_MTSN_S := $(SLC_RUN) -b mtsn_s -nostdlib

seq_naked-mtsparc-sim/%.o: $(srcdir)/mtsparc-sim/%.s
	$(AM_V_at)$(MKDIR_P) seq_naked-mtsparc-sim
	$(slc_verbose)$(SLC_MTSN_S) -c -o $@ $<

seq_naked-mtsparc-sim/%.o: $(srcdir)/%.c
	$(AM_V_at)$(MKDIR_P) seq_naked-mtsparc-sim
	$(slc_verbose)$(SLC_MTSN_S) -c -o $@ $<

CLEANFILES += \
	seq_naked-mtsparc-sim/callgate.o \
	seq_naked-mtsparc-sim/main.o \
	seq_naked-mtsparc-sim/slrt.o \
	seq_naked-mtsparc-sim/libslc.a \
	seq_naked-mtsparc-sim/libslmain.a

### MT-naked targets

#$(eval $(call MTSN_TEMPLATE,mtsn_n,mtsn_naked-mtsparc-sim,mtsn_naked_mtsparc_sim)

nobase_pkglib_DATA += \
	mtsn_naked-mtsparc-sim/slrt.o \
	mtsn_naked-mtsparc-sim/libslc.a \
	mtsn_naked-mtsparc-sim/libslmain.a 

mtsn_naked_mtsparc_sim_libslc_a_CONTENTS = \
	mtsn_naked-mtsparc-sim/callgate.o 

mtsn_naked_mtsparc_sim_libslmain_a_CONTENTS = \
	mtsn_naked-mtsparc-sim/main.o 

mtsn_naked-mtsparc-sim/%.a:
	$(AM_V_at)rm -f $@
	$(AM_V_AR)$(AR_MTSPARC) cru $@ $^
	$(AM_V_at)$(RANLIB_MTSPARC) $@

mtsn_naked-mtsparc-sim/libslc.a: $(mtsn_naked_mtsparc_sim_libslc_a_CONTENTS)
mtsn_naked-mtsparc-sim/libslmain.a: $(mtsn_naked_mtsparc_sim_libslmain_a_CONTENTS)

SLC_MTSN_N := $(SLC_RUN) -b mtsn_n -nostdlib

mtsn_naked-mtsparc-sim/%.o: $(srcdir)/mtsparc-sim/%.s
	$(AM_V_at)$(MKDIR_P) mtsn_naked-mtsparc-sim
	$(slc_verbose)$(SLC_MTSN_N) -c -o $@ $<

mtsn_naked-mtsparc-sim/%.o: $(srcdir)/%.c
	$(AM_V_at)$(MKDIR_P) mtsn_naked-mtsparc-sim
	$(slc_verbose)$(SLC_MTSN_N) -c -o $@ $<

CLEANFILES += \
	mtsn_naked-mtsparc-sim/main.o \
	mtsn_naked-mtsparc-sim/slrt.o \
	mtsn_naked-mtsparc-sim/callgate.o \
	mtsn_naked-mtsparc-sim/libslc.a \
	mtsn_naked-mtsparc-sim/libslmain.a


endif

