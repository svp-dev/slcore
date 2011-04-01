########### MT-Sparc components (FPGA) ###########

if ENABLE_SLC_MTSPARC

nobase_dist_pkgdata_DATA += \
	mtsparc-fpga/include/sys/types.h \
	mtsparc-fpga/include/bits/float.h \
	mtsparc-fpga/include/alloca.h \
	mtsparc-fpga/include/assert.h \
	mtsparc-fpga/include/ctype.h \
	mtsparc-fpga/include/errno.h \
	mtsparc-fpga/include/float.h \
	mtsparc-fpga/include/limits.h \
	mtsparc-fpga/include/math.h \
	mtsparc-fpga/include/stdbool.h \
	mtsparc-fpga/include/stdarg.h \
	mtsparc-fpga/include/stddef.h \
	mtsparc-fpga/include/stdint.h \
	mtsparc-fpga/include/stdio.h \
	mtsparc-fpga/include/stdlib.h \
	mtsparc-fpga/include/string.h \
	mtsparc-fpga/include/strings.h \
	mtsparc-fpga/include/time.h 

MALLOC_DEFS_MTS = \
	-DPAGESIZE=0x100U

TLSMALLOC_DEFS_MTS = \
	-DARCH_TLS_SERVICES=\"tls_arch_mtsparc_fpga.h\"

MTSLIB_COBJS = \
	$(addprefix mts_,$(addsuffix .o,$(notdir $(basename $(MTALIB_CSRC))))) \
	mts_tlsmalloc.o

# NO_FLOATING_POINT to disable handling of floats in printf
SLC_MTS = $(SLC_RUN) -b mts $(SHUTUP) -DNO_FLOATING_POINT

mts_%.o: $(srcdir)/src/%.c
	$(slc_verbose)$(SLC_MTS) -c -o $@ $<

mts_tlsmalloc.o: $(srcdir)/src/tlsmalloc/tlsmalloc.c
	$(slc_verbose)$(SLC_MTS) -c -o $@ $< $(TLSMALLOC_DEFS) $(TLSMALLOC_DEFS_MTS)

mts_tlsmalloc_debug.o: $(srcdir)/src/tlsmalloc/tlsmalloc.c
	$(slc_verbose)$(SLC_MTS) -c -o $@ $< $(TLSMALLOC_DEFS) $(TLSMALLOC_DEFS_MTS) -DDEBUG_MALLOC

mts_tlsmalloc_mgdebug.o: $(srcdir)/src/tlsmalloc/tlsmalloc.c
	$(slc_verbose)$(SLC_MTS) -c -o $@ $< $(TLSMALLOC_DEFS) $(TLSMALLOC_DEFS_MTS) -DDEBUG_MGFPGA

mts_tlstack_malloc_mgdebug.o: $(srcdir)/src/tlstack_malloc.c
	$(slc_verbose)$(SLC_MTS) -c -o $@ $< -DDEBUG_MGFPGA

mts_malloc.o: $(srcdir)/src/malloc.c
	$(slc_verbose)$(SLC_MTS) -c -o $@ $< $(MALLOC_DEFS) $(MALLOC_DEFS_MTS)

mts_malloc_debug.o: $(srcdir)/src/malloc.c
	$(slc_verbose)$(SLC_MTS) -c -o $@ $< $(MALLOC_DEFS) -DDEBUG=1 -DABORT_ON_ASSERT_FAILURE=0 -DFOOTERS=1


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
   mtsparc-fpga/libm.a \
   mtsparc-fpga/libmalloc_notls.a

mtsparc_fpga_libm_a_CONTENTS = \
   mts_empty.o 

# FIXME: not supported on MTS: no exclusive creates yet
mtsparc_fpga_libmalloc_notls_a_CONTENTS = \
   mts_empty.o 

# we do not use mtsparc_fpga_libm_a_CONTENTS in CLEANFILES below
# because we want to preserve the MTAMATHOBJS
CLEANFILES += \
	mts_missing_uclibc_math.o \
	$(mtsparc_fpga_libmalloc_notls_a_CONTENTS) \
	mtsparc-fpga/libm.a \
	mtsparc-fpga/libmalloc_notls.a

mtsparc-fpga/%.a:
	$(AM_V_at)rm -f $@
	$(AM_V_at)$(MKDIR_P) mtsparc-fpga
	$(AM_V_AR)$(AR_MTSPARC) cru $@ $^
	$(AM_V_at)$(RANLIB_MTSPARC) $@

mtsparc-fpga/libm.a: $(mtsparc_fpga_libm_a_CONTENTS)
mtsparc-fpga/libmalloc_notls.a: $(mtsparc_fpga_libmalloc_notls_a_CONTENTS)

### MT-hybrid targets

#$(eval $(call MTS_TEMPLATE,mta,mts_hybrid-mtsparc-fpga,mts_hybrid_mtsparc_fpga)

nobase_pkglib_DATA += \
   mts_hybrid-mtsparc-fpga/libsl.a

mts_hybrid_mtsparc_fpga_libsl_a_CONTENTS = \
	$(MTSLIB_COBJS) \
	$(addprefix mts_hybrid-mtsparc-fpga/,$(addsuffix .o,$(notdir $(basename $(MTALIB_SRC)))))

CLEANFILES += \
	$(mts_hybrid_mtsparc_fpga_libsl_a_CONTENTS) \
	mts_hybrid-mtsparc-fpga/libsl.a

mts_hybrid-mtsparc-fpga/%.o: $(srcdir)/src/%.c
	$(AM_V_at)$(MKDIR_P) mts_hybrid-mtsparc-fpga
	$(slc_verbose)$(SLC_MTS) -c -o $@ $<

mts_hybrid-mtsparc-fpga/%.a:
	$(AM_V_at)rm -f $@
	$(AM_V_AR)$(AR_MTSPARC) cru $@ $^
	$(AM_V_at)$(RANLIB_MTSPARC) $@

mts_hybrid-mtsparc-fpga/libsl.a: $(mts_hybrid_mtsparc_fpga_libsl_a_CONTENTS)

### seq-naked targets

#$(eval $(call MTS_TEMPLATE,mts_s,seq_naked-mtsparc-fpga,seq_naked_mtsparc_fpga)

nobase_pkglib_DATA += \
   seq_naked-mtsparc-fpga/libsl.a

seq_naked_mtsparc_fpga_libsl_a_CONTENTS = \
	$(MTSLIB_COBJS) \
	$(addprefix seq_naked-mtsparc-fpga/,$(addsuffix .o,$(notdir $(basename $(MTALIB_SRC)))))

CLEANFILES += \
	$(seq_naked_mtsparc_fpga_libsl_a_CONTENTS) \
	seq_naked-mtsparc-fpga/libsl.a

SLC_MTS_S = $(SLC_RUN) -b mts_s $(SHUTUP)

seq_naked-mtsparc-fpga/%.o: $(srcdir)/src/%.c
	$(AM_V_at)$(MKDIR_P) seq_naked-mtsparc-fpga
	$(slc_verbose)$(SLC_MTS_S) -c -o $@ $<

seq_naked-mtsparc-fpga/%.a:
	$(AM_V_at)rm -f $@
	$(AM_V_AR)$(AR_MTSPARC) cru $@ $^
	$(AM_V_at)$(RANLIB_MTSPARC) $@

seq_naked-mtsparc-fpga/libsl.a: $(seq_naked_mtsparc_fpga_libsl_a_CONTENTS)

### MT-naked targets
# DISABLED ON FPGA: allocate/suspend not supported

# #$(eval $(call MTS_TEMPLATE,mts_n,mts_naked-mtsparc-fpga,mts_naked_mtsparc_fpga)

# nobase_pkglib_DATA += \
#    mts_naked-mtsparc-fpga/libsl.a

# mts_naked_mtsparc_fpga_libsl_a_CONTENTS = \
# 	$(MTSLIB_COBJS) \
# 	$(addprefix mts_naked-mtsparc-fpga/,$(addsuffix .o,$(notdir $(basename $(MTALIB_SRC))))) 

# CLEANFILES += \
# 	$(mts_naked_mtsparc_fpga_libsl_a_CONTENTS) \
# 	mts_naked-mtsparc-fpga/libsl.a

# SLC_MTS_N = $(SLC_RUN) -b mts_n $(SHUTUP)

# mts_naked-mtsparc-fpga/%.o: $(srcdir)/src/%.c
# 	$(AM_V_at)$(MKDIR_P) mts_naked-mtsparc-fpga
# 	$(slc_verbose)$(SLC_MTS_N) -c -o $@ $<

# mts_naked-mtsparc-fpga/%.a:
# 	$(AM_V_at)rm -f $@
# 	$(AM_V_AR)$(AR_MTSPARC) cru $@ $^
# 	$(AM_V_at)$(RANLIB_MTSPARC) $@

# mts_naked-mtsparc-fpga/libsl.a: $(mts_naked_mtsparc_fpga_libsl_a_CONTENTS)



endif

