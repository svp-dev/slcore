########### MT-Alpha components ###########

MTALIB_CSRC = \
	src/abort.c \
	src/atoi.c \
	src/atol.c \
	src/atoll.c \
	src/bcopy.c \
	src/bzero.c \
	src/ctype.c \
	src/dtoa_simple.c \
	src/errno.c \
	src/ffs.c \
	src/fprintf.c \
	src/fputc.c \
	src/fputs.c \
	src/fwrite.c \
	src/getenv.c \
	src/_hdtoa.c \
	src/heap.c \
	src/malloc.c \
	src/malloc_wrappers.c \
	src/memcpy.c \
	src/memmove.c \
	src/memset.c \
	src/mtconf.c \
	src/mtstdio.c \
	src/perf.c \
	src/printf.c \
	src/printf-pos.c \
	src/putc.c \
	src/putchar.c \
	src/puts.c \
	src/snprintf.c \
	src/sprintf.c \
	src/stpcpy.c \
	src/stpncpy.c \
	src/strcat.c \
	src/strchr.c \
	src/strcpy.c \
	src/strdup.c \
	src/strerror.c \
	src/strlcat.c \
	src/strlcpy.c \
	src/strlen.c \
	src/strncat.c \
	src/strncmp.c \
	src/strncpy.c \
	src/strnlen.c \
	src/strtol.c \
	src/strtoll.c \
	src/strtoul.c \
	src/strtoull.c \
	src/sys_errlist.c \
	src/vfprintf.c \
	src/vprintf.c \
	src/vsnprintf.c

MTALIB_SRC = \
	src/div.c \
	src/roman.c \
	src/io.c \
	src/mtinit.c \
	src/mtsep.c

MTALIB_EXTRA = \
	src/floatio.h \
	src/fpmath.h \
	src/heap.h \
	src/missing_uclibc_math.c \
	src/mtconf.h \
	src/mtstdio.h \
	src/printfcommon.h \
	src/printflocal.h

EXTRA_DIST += $(MTALIB_CSRC) $(MTALIB_SRC) $(MTALIB_EXTRA)

nobase_dist_pkgdata_DATA += \
	mtalpha-sim/include/svp_os.h \
	mtalpha-sim/include/svp_asmdiv.h


include $(srcdir)/gdtoa.mk

include $(srcdir)/src/mtamathobjs.mk
EXTRA_DIST += $(MTAMATHOBJS)

if ENABLE_SLC_MTALPHA

SHUTUP = -Dshutup_cstring_h -Dshutup_cstdlib_h -Dshutup_cstdio_h -Dshutup_cstrings_h

MALLOC_DEFS = -DLACKS_SYS_TYPES_H \
	-DUSE_DL_PREFIX \
	-DHAVE_MMAP=0 \
	-DMORECORE_CANNOT_TRIM \
	-DMALLOC_FAILURE_ACTION="" \
	-DLACKS_UNISTD_H \
	-DLACKS_ERRNO_H \
	-DLACKS_STDLIB_H \
	-DLACKS_STRING_H \
	-DLACKS_STRINGS_H \
	-DUSE_BUILTIN_FFS=1 -Dffs=__builtin_ffs \
	-DLACKS_SYS_PARAM_H \
	-DLACKS_TIME \
	-DPAGESIZE=0x40000000U

MTALIB_COBJS = \
	$(addprefix mta_,$(addsuffix .o,$(notdir $(basename $(MTALIB_CSRC))))) \
	$(addprefix mta_gdtoa_,$(addsuffix .o,$(notdir $(basename $(GDTOA_SRC)))))

SLC_MTA = $(SLC_RUN) -b mta $(SHUTUP)

mta_%.o: $(srcdir)/src/%.c
	$(slc_verbose)$(SLC_MTA) -c -o $@ $<

mta_gdtoa_%.o: $(srcdir)/src/gdtoa/%.c
	$(slc_verbose)$(SLC_MTA) -c -o $@ $<

mta_malloc.o: $(srcdir)/src/malloc.c
	$(slc_verbose)$(SLC_MTA) -c -o $@ $< $(MALLOC_DEFS)

mta_malloc_debug.o: $(srcdir)/src/malloc.c
	$(slc_verbose)$(SLC_MTA) -c -o $@ $< $(MALLOC_DEFS) -DDEBUG -DABORT_ON_ASSERT_FAILURE=0


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

### MT-hybrid targets

#$(eval $(call MTA_TEMPLATE,mta,mta_hybrid-mtalpha-sim,mta_hybrid_mtalpha_sim)

nobase_pkglib_DATA += \
   mta_hybrid-mtalpha-sim/libm.a \
   mta_hybrid-mtalpha-sim/libsl.a

mta_hybrid_mtalpha_sim_libsl_a_CONTENTS = \
	$(MTALIB_COBJS) \
	$(addprefix mta_hybrid-mtalpha-sim/,$(addsuffix .o,$(notdir $(basename $(MTALIB_SRC)))))

CLEANFILES += \
	$(mta_hybrid_mtalpha_sim_libsl_a_CONTENTS) \
	mta_hybrid-mtalpha-sim/libsl.a

mta_hybrid_mtalpha_sim_libm_a_CONTENTS = \
	$(MTAMATHOBJS) \
	mta_hybrid-mtalpha-sim/missing_uclibc_math.o

CLEANFILES += \
	mta_hybrid-mtalpha-sim/missing_uclibc_math.o \
	mta_hybrid-mtalpha-sim/libm.a

mta_hybrid-mtalpha-sim/%.o: $(srcdir)/src/%.c
	$(AM_V_at)$(MKDIR_P) mta_hybrid-mtalpha-sim
	$(slc_verbose)$(SLC_MTA) -c -o $@ $<

mta_hybrid-mtalpha-sim/%.a:
	$(AM_V_at)rm -f $@
	$(AM_V_AR)$(AR_MTALPHA) cru $@ $^
	$(AM_V_at)$(RANLIB_MTALPHA) $@

mta_hybrid-mtalpha-sim/libsl.a: $(mta_hybrid_mtalpha_sim_libsl_a_CONTENTS)
mta_hybrid-mtalpha-sim/libm.a: $(mta_hybrid_mtalpha_sim_libm_a_CONTENTS)

### seq-naked targets

#$(eval $(call MTA_TEMPLATE,mta_s,seq_naked-mtalpha-sim,seq_naked_mtalpha_sim)

nobase_pkglib_DATA += \
   seq_naked-mtalpha-sim/libm.a \
   seq_naked-mtalpha-sim/libsl.a

seq_naked_mtalpha_sim_libsl_a_CONTENTS = \
	$(MTALIB_COBJS) \
	$(addprefix seq_naked-mtalpha-sim/,$(addsuffix .o,$(notdir $(basename $(MTALIB_SRC)))))

CLEANFILES += \
	$(seq_naked_mtalpha_sim_libsl_a_CONTENTS) \
	seq_naked-mtalpha-sim/libsl.a

seq_naked_mtalpha_sim_libm_a_CONTENTS = \
	$(MTAMATHOBJS) \
	seq_naked-mtalpha-sim/missing_uclibc_math.o

CLEANFILES += \
	seq_naked-mtalpha-sim/missing_uclibc_math.o \
	seq_naked-mtalpha-sim/libm.a

SLC_MTA_S = $(SLC_RUN) -b mta_s $(SHUTUP)

seq_naked-mtalpha-sim/%.o: $(srcdir)/src/%.c
	$(AM_V_at)$(MKDIR_P) seq_naked-mtalpha-sim
	$(slc_verbose)$(SLC_MTA_S) -c -o $@ $<

seq_naked-mtalpha-sim/%.a:
	$(AM_V_at)rm -f $@
	$(AM_V_AR)$(AR_MTALPHA) cru $@ $^
	$(AM_V_at)$(RANLIB_MTALPHA) $@

seq_naked-mtalpha-sim/libsl.a: $(seq_naked_mtalpha_sim_libsl_a_CONTENTS)
seq_naked-mtalpha-sim/libm.a: $(seq_naked_mtalpha_sim_libm_a_CONTENTS)

### MT-naked targets

#$(eval $(call MTA_TEMPLATE,mta_n,mta_naked-mtalpha-sim,mta_naked_mtalpha_sim)

nobase_pkglib_DATA += \
   mta_naked-mtalpha-sim/libm.a \
   mta_naked-mtalpha-sim/libsl.a

mta_naked_mtalpha_sim_libsl_a_CONTENTS = \
	$(MTALIB_COBJS) \
	$(addprefix mta_naked-mtalpha-sim/,$(addsuffix .o,$(notdir $(basename $(MTALIB_SRC))))) 

CLEANFILES += \
	$(mta_naked_mtalpha_sim_libsl_a_CONTENTS) \
	mta_naked-mtalpha-sim/libsl.a

mta_naked_mtalpha_sim_libm_a_CONTENTS = \
	$(MTAMATHOBJS) \
	mta_naked-mtalpha-sim/missing_uclibc_math.o

CLEANFILES += \
	mta_naked-mtalpha-sim/missing_uclibc_math.o \
	mta_naked-mtalpha-sim/libm.a

SLC_MTA_N = $(SLC_RUN) -b mta_n $(SHUTUP)

mta_naked-mtalpha-sim/%.o: $(srcdir)/src/%.c
	$(AM_V_at)$(MKDIR_P) mta_naked-mtalpha-sim
	$(slc_verbose)$(SLC_MTA_N) -c -o $@ $<

mta_naked-mtalpha-sim/%.a:
	$(AM_V_at)rm -f $@
	$(AM_V_AR)$(AR_MTALPHA) cru $@ $^
	$(AM_V_at)$(RANLIB_MTALPHA) $@

mta_naked-mtalpha-sim/libsl.a: $(mta_naked_mtalpha_sim_libsl_a_CONTENTS)
mta_naked-mtalpha-sim/libm.a: $(mta_naked_mtalpha_sim_libm_a_CONTENTS)


endif

