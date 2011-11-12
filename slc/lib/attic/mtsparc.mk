########### MT-Sparc components ###########

if ENABLE_SLC_MTSPARC

nobase_dist_pkgdata_DATA += \
	mtsparc-sim/include/sys/stat.h \
	mtsparc-sim/include/sys/types.h \
	mtsparc-sim/include/sys/time.h \
	mtsparc-sim/include/bits/float.h \
	mtsparc-sim/include/alloca.h \
	mtsparc-sim/include/assert.h \
	mtsparc-sim/include/ctype.h \
	mtsparc-sim/include/errno.h \
	mtsparc-sim/include/fcntl.h \
	mtsparc-sim/include/float.h \
	mtsparc-sim/include/limits.h \
	mtsparc-sim/include/math.h \
	mtsparc-sim/include/stdbool.h \
	mtsparc-sim/include/stdarg.h \
	mtsparc-sim/include/stddef.h \
	mtsparc-sim/include/stdint.h \
	mtsparc-sim/include/stdio.h \
	mtsparc-sim/include/stdlib.h \
	mtsparc-sim/include/string.h \
	mtsparc-sim/include/strings.h \
	mtsparc-sim/include/time.h \
	mtsparc-sim/include/unistd.h

MALLOC_DEFS_MTSN = \
	-DPAGESIZE=0x400000U

TLSMALLOC_DEFS_MTSN = \
	-DARCH_TLS_SERVICES=\"tls_arch_mtsparc_sim.h\"

MTSNLIB_COBJS = \
	$(addprefix mtsn_,$(addsuffix .o,$(notdir $(basename $(MTALIB_CSRC))))) \
	$(addprefix mtsn_,$(addsuffix .o,$(notdir $(basename $(MTALIB_SIM_CSRC))))) \
	mtsn_tlsmalloc.o

# NO_FLOATING_POINT to disable handling of floats in printf
SLC_MTSN = $(SLC_RUN) -b mtsn $(SHUTUP) -DNO_FLOATING_POINT

mtsn_%.o: $(srcdir)/src/%.c
	$(slc_verbose)$(SLC_MTSN) -c -o $@ $<

mtsn_tlsmalloc_fast.o: $(srcdir)/src/tlsmalloc/tlsmalloc.c
	$(slc_verbose)$(SLC_MTSN) -c -o $@ $< $(TLSMALLOC_DEFS) $(TLSMALLOC_DEFS_MTSN) -DNDEBUG

mtsn_tlsmalloc.o: $(srcdir)/src/tlsmalloc/tlsmalloc.c
	$(slc_verbose)$(SLC_MTSN) -c -o $@ $< $(TLSMALLOC_DEFS) $(TLSMALLOC_DEFS_MTSN) -DFOOTERS

mtsn_tlsmalloc_debug.o: $(srcdir)/src/tlsmalloc/tlsmalloc.c
	$(slc_verbose)$(SLC_MTSN) -c -o $@ $< $(TLSMALLOC_DEFS) $(TLSMALLOC_DEFS_MTSN) -DDEBUG_MALLOC -DFOOTERS

mtsn_tlsmalloc_mgdebug.o: $(srcdir)/src/tlsmalloc/tlsmalloc.c
	$(slc_verbose)$(SLC_MTSN) -c -o $@ $< $(TLSMALLOC_DEFS) $(TLSMALLOC_DEFS_MTSN) -DDEBUG_MGSIM -DFOOTERS


mtsn_tlsmalloc_fast_nogc.o: $(srcdir)/src/tlsmalloc/tlsmalloc.c
	$(slc_verbose)$(SLC_MTSN) -c -o $@ $< $(TLSMALLOC_DEFS) $(TLSMALLOC_DEFS_MTSN) -DNDEBUG -DDISABLE_GC

mtsn_tlsmalloc_nogc.o: $(srcdir)/src/tlsmalloc/tlsmalloc.c
	$(slc_verbose)$(SLC_MTSN) -c -o $@ $< $(TLSMALLOC_DEFS) $(TLSMALLOC_DEFS_MTSN) -DFOOTERS -DDISABLE_GC

mtsn_tlsmalloc_nogc_debug.o: $(srcdir)/src/tlsmalloc/tlsmalloc.c
	$(slc_verbose)$(SLC_MTSN) -c -o $@ $< $(TLSMALLOC_DEFS) $(TLSMALLOC_DEFS_MTSN) -DDEBUG_MALLOC -DFOOTERS -DDISABLE_GC

mtsn_tlsmalloc_nogc_mgdebug.o: $(srcdir)/src/tlsmalloc/tlsmalloc.c
	$(slc_verbose)$(SLC_MTSN) -c -o $@ $< $(TLSMALLOC_DEFS) $(TLSMALLOC_DEFS_MTSN) -DDEBUG_MGSIM -DFOOTERS -DDISABLE_GC


mtsn_tlstack_malloc_mgdebug.o: $(srcdir)/src/tlstack_malloc.c
	$(slc_verbose)$(SLC_MTSN) -c -o $@ $< -DDEBUG_MGSIM

mtsn_malloc.o: $(srcdir)/src/malloc.c
	$(slc_verbose)$(SLC_MTSN) -c -o $@ $< $(MALLOC_DEFS) $(MALLOC_DEFS_MTSN)

mtsn_malloc_debug.o: $(srcdir)/src/malloc.c
	$(slc_verbose)$(SLC_MTSN) -c -o $@ $< $(MALLOC_DEFS) -DDEBUG=1 -DABORT_ON_ASSERT_FAILURE=0 -DFOOTERS=1

pkglib_DATA += \
	mtsn_tlsmalloc_fast.o \
	mtsn_tlsmalloc_debug.o \
	mtsn_tlsmalloc_mgdebug.o \
	mtsn_tlsmalloc_fast_nogc.o \
	mtsn_tlsmalloc_nogc.o \
	mtsn_tlsmalloc_nogc_debug.o \
	mtsn_tlsmalloc_nogc_mgdebug.o \
	mtsn_tlstack_malloc_mgdebug.o \
	mtsn_malloc_debug.o


### Common rules ###

# define MTA_TEMPLATE

# # # arg 1 = slc target
# # # arg 2 = libdir
# # # arg 3 = libdir_undescores

# nobase_pkglib_DATA += \
#    $(2)/libm.a \
#    $(2)/libsl.a

# $(3)_libsl_a_CONTENTS = \
#	$(MTALIB_COBJS) \
# 	$(addprefix $(2)/,$(addsuffix .o,$(notdir $(basename $(MTALIB_SRC)))))
# 	$(addprefix $(2)/,$(addsuffix .o,$(notdir $(basename $(MTALIB_SIM_SRC)))))

# CLEANFILES += \
# 	$($(3)_libsl_a_CONTENTS) \
# 	$(2)/libsl.a

# $(3)_libm_a_CONTENTS = \
# 	$(MTAMATHOBJS) \
# 	$(2)/missing_uclibc_math.o

# CLEANFILES += \
# 	$(2)/missing_uclibc_math.o \
# 	$(2)/libm.a

# SLC_CMD = $(SLC_RUN) -b $(1) $(SHUTUP)

# $(2)/%.o: $(srcdir)/src/%.c
# 	$(AM_V_at)$(MKDIR_P) $(2)
# 	$(slc_verbose)$(SLC_CMD) -c -o $@ $<

# $(2)/%.a:
#	$(AM_V_at)rm -f $@
#	$(AM_V_AR)$(AR_MTALPHA) cru $@ $^
#	$(AM_V_at)$(RANLIB_MTALPHA) $@

# $(2)/libsl.a: $($(3)_libsl_a_CONTENTS)
# $(2)/libm.a: $($(3)_libm_a_CONTENTS)

# enddef

### Target-neutral libraries

nobase_pkglib_DATA += \
   mtsparc-sim/libm.a \
   mtsparc-sim/libmalloc_notls.a

mtsparc_sim_libm_a_CONTENTS = \
   mtsn_empty.o 

mtsparc_sim_libmalloc_notls_a_CONTENTS = \
   mtsn_malloc_wrappers.o 


# we do not use mtsparc_sim_libm_a_CONTENTS in CLEANFILES below
# because we want to preserve the MTAMATHOBJS
CLEANFILES += \
	mtsn_missing_uclibc_math.o \
	$(mtsparc_sim_libmalloc_notls_a_CONTENTS) \
	mtsparc-sim/libm.a \
	mtsparc-sim/libmalloc_notls.a

mtsparc-sim/%.a:
	$(AM_V_at)rm -f $@
	$(AM_V_at)$(MKDIR_P) mtsparc-sim
	$(AM_V_AR)$(AR_MTSPARC) cru $@ $^
	$(AM_V_at)$(RANLIB_MTSPARC) $@

mtsparc-sim/libm.a: $(mtsparc_sim_libm_a_CONTENTS)
mtsparc-sim/libmalloc_notls.a: $(mtsparc_sim_libmalloc_notls_a_CONTENTS)

### MT-hybrid targets

#$(eval $(call MTSN_TEMPLATE,mta,mtsn_hybrid-mtsparc-sim,mtsn_hybrid_mtsparc_sim)

nobase_pkglib_DATA += \
   mtsn_hybrid-mtsparc-sim/libsl.a

mtsn_hybrid_mtsparc_sim_libsl_a_CONTENTS = \
	$(MTSNLIB_COBJS) \
	$(addprefix mtsn_hybrid-mtsparc-sim/,$(addsuffix .o,$(notdir $(basename $(MTALIB_SRC))))) \
	$(addprefix mtsn_hybrid-mtsparc-sim/,$(addsuffix .o,$(notdir $(basename $(MTALIB_SIM_SRC)))))

CLEANFILES += \
	$(mtsn_hybrid_mtsparc_sim_libsl_a_CONTENTS) \
	mtsn_hybrid-mtsparc-sim/libsl.a

mtsn_hybrid-mtsparc-sim/%.o: $(srcdir)/src/%.c
	$(AM_V_at)$(MKDIR_P) mtsn_hybrid-mtsparc-sim
	$(slc_verbose)$(SLC_MTSN) -c -o $@ $<

mtsn_hybrid-mtsparc-sim/%.a:
	$(AM_V_at)rm -f $@
	$(AM_V_AR)$(AR_MTSPARC) cru $@ $^
	$(AM_V_at)$(RANLIB_MTSPARC) $@

mtsn_hybrid-mtsparc-sim/libsl.a: $(mtsn_hybrid_mtsparc_sim_libsl_a_CONTENTS)

### seq-naked targets

#$(eval $(call MTSN_TEMPLATE,mtsn_s,seq_naked-mtsparc-sim,seq_naked_mtsparc_sim)

nobase_pkglib_DATA += \
   seq_naked-mtsparc-sim/libsl.a

seq_naked_mtsparc_sim_libsl_a_CONTENTS = \
	$(MTSNLIB_COBJS) \
	$(addprefix seq_naked-mtsparc-sim/,$(addsuffix .o,$(notdir $(basename $(MTALIB_SRC))))) \
	$(addprefix seq_naked-mtsparc-sim/,$(addsuffix .o,$(notdir $(basename $(MTALIB_SIM_SRC)))))

CLEANFILES += \
	$(seq_naked_mtsparc_sim_libsl_a_CONTENTS) \
	seq_naked-mtsparc-sim/libsl.a

SLC_MTSN_S = $(SLC_RUN) -b mtsn_s $(SHUTUP)

seq_naked-mtsparc-sim/%.o: $(srcdir)/src/%.c
	$(AM_V_at)$(MKDIR_P) seq_naked-mtsparc-sim
	$(slc_verbose)$(SLC_MTSN_S) -c -o $@ $<

seq_naked-mtsparc-sim/%.a:
	$(AM_V_at)rm -f $@
	$(AM_V_AR)$(AR_MTSPARC) cru $@ $^
	$(AM_V_at)$(RANLIB_MTSPARC) $@

seq_naked-mtsparc-sim/libsl.a: $(seq_naked_mtsparc_sim_libsl_a_CONTENTS)

### MT-naked targets

#$(eval $(call MTSN_TEMPLATE,mtsn_n,mtsn_naked-mtsparc-sim,mtsn_naked_mtsparc_sim)

nobase_pkglib_DATA += \
   mtsn_naked-mtsparc-sim/libsl.a

mtsn_naked_mtsparc_sim_libsl_a_CONTENTS = \
	$(MTSNLIB_COBJS) \
	$(addprefix mtsn_naked-mtsparc-sim/,$(addsuffix .o,$(notdir $(basename $(MTALIB_SRC)))))  \
	$(addprefix mtsn_naked-mtsparc-sim/,$(addsuffix .o,$(notdir $(basename $(MTALIB_SIM_SRC)))))

CLEANFILES += \
	$(mtsn_naked_mtsparc_sim_libsl_a_CONTENTS) \
	mtsn_naked-mtsparc-sim/libsl.a

SLC_MTSN_N = $(SLC_RUN) -b mtsn_n $(SHUTUP)

mtsn_naked-mtsparc-sim/%.o: $(srcdir)/src/%.c
	$(AM_V_at)$(MKDIR_P) mtsn_naked-mtsparc-sim
	$(slc_verbose)$(SLC_MTSN_N) -c -o $@ $<

mtsn_naked-mtsparc-sim/%.a:
	$(AM_V_at)rm -f $@
	$(AM_V_AR)$(AR_MTSPARC) cru $@ $^
	$(AM_V_at)$(RANLIB_MTSPARC) $@

mtsn_naked-mtsparc-sim/libsl.a: $(mtsn_naked_mtsparc_sim_libsl_a_CONTENTS)




endif

