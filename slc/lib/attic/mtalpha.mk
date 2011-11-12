########### MT-Alpha components ###########

MTALIB_CSRC = \
	libc/bsd/libc/gdtoa/_hdtoa.c \
	libc/bsd/libc/gen/errlst.c \
	libc/bsd/libc/stdio/fprintf.c \
	libc/bsd/libc/stdio/printf-pos.c \
	libc/bsd/libc/stdio/printf.c \
	libc/bsd/libc/stdio/sprintf.c \
	libc/bsd/libc/stdlib//atoll.c \
	libc/bsd/libc/stdlib/atoi.c \
	libc/bsd/libc/stdlib/atol.c \
	libc/bsd/libc/stdlib/getenv.c \
	libc/bsd/libc/stdlib/strtol.c \
	libc/bsd/libc/stdlib/strtoll.c \
	libc/bsd/libc/stdlib/strtoul.c \
	libc/bsd/libc/stdlib/strtoull.c \
	libc/bsd/libc/string/memchr.c \
	libc/bsd/libc/string/memcmp.c \
	libc/bsd/libc/string/memcpy.c \
	libc/bsd/libc/string/memmove.c \
	libc/bsd/libc/string/memset.c \
	libc/bsd/libc/string/stpcpy.c \
	libc/bsd/libc/string/stpncpy.c \
	libc/bsd/libc/string/strcat.c \
	libc/bsd/libc/string/strchr.c \
	libc/bsd/libc/string/strcmp.c \
	libc/bsd/libc/string/strcpy.c \
	libc/bsd/libc/string/strdup.c \
	libc/bsd/libc/string/strerror.c \
	libc/bsd/libc/string/strlcat.c \
	libc/bsd/libc/string/strlcpy.c \
	libc/bsd/libc/string/strlen.c \
	libc/bsd/libc/string/strncat.c \
	libc/bsd/libc/string/strncmp.c \
	libc/bsd/libc/string/strncpy.c \
	libc/bsd/libc/string/strnlen.c \
	libc/bsd/libc/stringsrc/bcopy.c \
	libc/bsd/libc/stringsrc/bzero.c \
	libc/malloc/malloc.c \
	libc/misc/ctype.c \
	libc/misc/dtoa_simple.c \
	libc/misc/errno.c \
	libc/stdio/fputc.c \
	libc/stdio/fputs.c \
	libc/stdio/fwrite.c \
	libc/stdio/mtstdio.c \
	libc/stdio/perror.c \
	libc/stdio/putc.c \
	libc/stdio/putchar.c \
	libc/stdio/puts.c \
	libc/stdio/snprintf.c \
	libc/stdio/vprintf.c \
	libc/stdio/vsnprintf.c \
	libc/string/ffs.c \
	mgos/input/mtinput.c \
	mgos/posix-compat/abort.c \
	mgos/posix-compat/exit.c \
	mgos/posix-compat/heap.c \
	mgos/posix-compat/time.c \
	mgos/posix-compat/time_virtual.c \
	mgos/tlstack_malloc.c \
	slsys/perf/perf.c \
	slsys/perf/perf_wrappers.c

MTALIB_SRC = \
	libc/malloc/malloc_excl.c \
	libc/bsd/libc/stdio/vfprintf.c \
	slsys/deprecated/div.c \
	slsys/deprecated/roman.c \
	slsys/deprecated/io.c

MTALIB_SIM_SRC = \
	mgos/posix-compat/gtod.c \
	mgos/gfx/mtgfx.c \
	mgos/sep/mtsep.c \
	mgos/uart/mtuart.c \
	mgos/posix-compat/mtunix.c

MTALIB_SIM_CSRC = \
	mgos/posix-compat/gtod_virtual.c \
	mgos/mgsim/mtconf.c \
	mgos/input/mtargs.c \
	mgos/init/mtinit.c 



MTALIB_EXTRA = \
	libc/bsd/libc/include/fpmath.h \
	libc/bsd/libc/stdio/floatio.h \
	libc/bsd/libc/stdio/printfcommon.h \
	libc/bsd/libc/stdio/printflocal.h \
	libc/malloc/malloc_wrappers.c \
	libc/math/missing_uclibc_math.c \
	libc/stdio/mtstdio.h \
	mgos/mgsim/RPCServiceDatabase.h \
	mgos/mgsim/mgsim.h \
	mgos/mgsim/mtconf.h \
	mgos/posix-compat/heap.h

EXTRA_DIST += $(MTALIB_CSRC) $(MTALIB_SRC) $(MTALIB_SIM_SRC) $(MTALIB_SIM_CSRC) $(MTALIB_EXTRA)

MALLOC_DEFS = -DLACKS_SYS_TYPES_H \
	-DUSE_DL_PREFIX \
	-DHAVE_MMAP=0 \
	-DMORECORE_CANNOT_TRIM \
	-DMALLOC_FAILURE_ACTION="" \
	-DLACKS_UNISTD_H \
	-DUSE_BUILTIN_FFS=1 \
	-DLACKS_SYS_PARAM_H \
	-DLACKS_TIME

TLSMALLOC_DEFS = \
	-Dtls_malloc=malloc \
	-Dtls_free=free \
	-Dtls_realloc=realloc \
	-Dtls_calloc=calloc

include $(srcdir)/gdtoa.mk

include $(srcdir)/src/mtamathobjs.mk
EXTRA_DIST += $(MTAMATHOBJS)

if ENABLE_SLC_MTALPHA

nobase_dist_pkgdata_DATA += \
	mtalpha-sim/include/sys/stat.h \
	mtalpha-sim/include/sys/time.h \
	mtalpha-sim/include/sys/types.h \
	mtalpha-sim/include/bits/float.h \
	mtalpha-sim/include/alloca.h \
	mtalpha-sim/include/assert.h \
	mtalpha-sim/include/ctype.h \
	mtalpha-sim/include/errno.h \
	mtalpha-sim/include/fcntl.h \
	mtalpha-sim/include/float.h \
	mtalpha-sim/include/limits.h \
	mtalpha-sim/include/math.h \
	mtalpha-sim/include/stdbool.h \
	mtalpha-sim/include/stdarg.h \
	mtalpha-sim/include/stddef.h \
	mtalpha-sim/include/stdint.h \
	mtalpha-sim/include/stdio.h \
	mtalpha-sim/include/stdlib.h \
	mtalpha-sim/include/string.h \
	mtalpha-sim/include/strings.h \
	mtalpha-sim/include/time.h \
	mtalpha-sim/include/unistd.h

SHUTUP = \
	-Dshutup_cstring_h \
	-Dshutup_cstdlib_h \
	-Dshutup_cstdio_h \
	-Dshutup_cstrings_h \
	-Dshutup_ctime_h

MALLOC_DEFS_MTA = \
	-DPAGESIZE=0x40000000U

TLSMALLOC_DEFS_MTA = \
	-DARCH_TLS_SERVICES=\"tls_arch_mtalpha.h\" 

MTALIB_COBJS = \
	$(addprefix mta_,$(addsuffix .o,$(notdir $(basename $(MTALIB_CSRC))))) \
	$(addprefix mta_,$(addsuffix .o,$(notdir $(basename $(MTALIB_SIM_CSRC))))) \
	$(addprefix mta_gdtoa_,$(addsuffix .o,$(notdir $(basename $(GDTOA_CSRC))))) \
	mta_tlsmalloc.o \
	mta_malloc_excl.o

SLC_MTA = $(SLC_RUN) -b mta $(SHUTUP)

mta_%.o: $(srcdir)/src/%.c
	$(slc_verbose)$(SLC_MTA) -c -o $@ $<

mta_gdtoa_%.o: $(srcdir)/src/gdtoa/%.c
	$(slc_verbose)$(SLC_MTA) -c -o $@ $<



mta_tlsmalloc_fast.o: $(srcdir)/src/tlsmalloc/tlsmalloc.c
	$(slc_verbose)$(SLC_MTA) -c -o $@ $< $(TLSMALLOC_DEFS) $(TLSMALLOC_DEFS_MTA) -DNDEBUG

mta_tlsmalloc.o: $(srcdir)/src/tlsmalloc/tlsmalloc.c
	$(slc_verbose)$(SLC_MTA) -c -o $@ $< $(TLSMALLOC_DEFS) $(TLSMALLOC_DEFS_MTA) -DFOOTERS

mta_tlsmalloc_debug.o: $(srcdir)/src/tlsmalloc/tlsmalloc.c
	$(slc_verbose)$(SLC_MTA) -c -o $@ $< $(TLSMALLOC_DEFS) $(TLSMALLOC_DEFS_MTA) -DDEBUG_MALLOC -DFOOTERS

mta_tlsmalloc_mgdebug.o: $(srcdir)/src/tlsmalloc/tlsmalloc.c
	$(slc_verbose)$(SLC_MTA) -c -o $@ $< $(TLSMALLOC_DEFS) $(TLSMALLOC_DEFS_MTA) -DDEBUG_MGSIM -DFOOTERS


mta_tlsmalloc_fast_nogc.o: $(srcdir)/src/tlsmalloc/tlsmalloc.c
	$(slc_verbose)$(SLC_MTA) -c -o $@ $< $(TLSMALLOC_DEFS) $(TLSMALLOC_DEFS_MTA) -DNDEBUG -DDISABLE_GC

mta_tlsmalloc_nogc.o: $(srcdir)/src/tlsmalloc/tlsmalloc.c
	$(slc_verbose)$(SLC_MTA) -c -o $@ $< $(TLSMALLOC_DEFS) $(TLSMALLOC_DEFS_MTA) -DFOOTERS -DDISABLE_GC

mta_tlsmalloc_nogc_debug.o: $(srcdir)/src/tlsmalloc/tlsmalloc.c
	$(slc_verbose)$(SLC_MTA) -c -o $@ $< $(TLSMALLOC_DEFS) $(TLSMALLOC_DEFS_MTA) -DDEBUG_MALLOC -DFOOTERS -DDISABLE_GC

mta_tlsmalloc_nogc_mgdebug.o: $(srcdir)/src/tlsmalloc/tlsmalloc.c
	$(slc_verbose)$(SLC_MTA) -c -o $@ $< $(TLSMALLOC_DEFS) $(TLSMALLOC_DEFS_MTA) -DDEBUG_MGSIM -DFOOTERS -DDISABLE_GC



mta_tlstack_malloc_mgdebug.o: $(srcdir)/src/tlstack_malloc.c
	$(slc_verbose)$(SLC_MTA) -c -o $@ $< -DDEBUG_MGSIM


mta_malloc.o: $(srcdir)/src/malloc.c
	$(slc_verbose)$(SLC_MTA) -c -o $@ $< $(MALLOC_DEFS) $(MALLOC_DEFS_MTA)

mta_malloc_debug.o: $(srcdir)/src/malloc.c
	$(slc_verbose)$(SLC_MTA) -c -o $@ $< $(MALLOC_DEFS) $(MALLOC_DEFS_MTA) -DDEBUG=1 -DABORT_ON_ASSERT_FAILURE=0 -DFOOTERS=1

# install the debug objects with other package data
pkglib_DATA += \
	mta_tlsmalloc_fast.o \
	mta_tlsmalloc_debug.o \
	mta_tlsmalloc_mgdebug.o \
	mta_tlsmalloc_fast_nogc.o \
	mta_tlsmalloc_nogc.o \
	mta_tlsmalloc_nogc_debug.o \
	mta_tlsmalloc_nogc_mgdebug.o \
	mta_tlstack_malloc_mgdebug.o \
	mta_malloc_debug.o

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
#	$(addprefix $(2)/gdtoa_,$(addsuffix .o,$(notdir $(basename $(GDTOA_SRC))))) \
# 	$(addprefix $(2)/,$(addsuffix .o,$(notdir $(basename $(MTALIB_SRC))))) \
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
   mtalpha-sim/libm.a \
   mtalpha-sim/libmalloc_notls.a

mtalpha_sim_libm_a_CONTENTS = \
	$(MTAMATHOBJS) \
	mta_missing_uclibc_math.o

mtalpha_sim_libmalloc_notls_a_CONTENTS = \
	mta_malloc_wrappers.o

# we do not use mtalpha_sim_libm_a_CONTENTS in CLEANFILES below
# because we want to preserve the MTAMATHOBJS
CLEANFILES += \
	mta_missing_uclibc_math.o \
	$(mtalpha_sim_libmalloc_notls_a_CONTENTS) \
	mtalpha-sim/libm.a \
	mtalpha-sim/libmalloc_notls.a

mtalpha-sim/%.a:
	$(AM_V_at)rm -f $@
	$(AM_V_at)$(MKDIR_P) mtalpha-sim
	$(AM_V_AR)$(AR_MTALPHA) cru $@ $^
	$(AM_V_at)$(RANLIB_MTALPHA) $@

mtalpha-sim/libm.a: $(mtalpha_sim_libm_a_CONTENTS)
mtalpha-sim/libmalloc_notls.a: $(mtalpha_sim_libmalloc_notls_a_CONTENTS)

### MT-hybrid targets

#$(eval $(call MTA_TEMPLATE,mta,mta_hybrid-mtalpha-sim,mta_hybrid_mtalpha_sim)

nobase_pkglib_DATA += \
   mta_hybrid-mtalpha-sim/libsl.a

mta_hybrid_mtalpha_sim_libsl_a_CONTENTS = \
	$(MTALIB_COBJS) \
	$(addprefix mta_hybrid-mtalpha-sim/gdtoa_,$(addsuffix .o,$(notdir $(basename $(GDTOA_SRC))))) \
	$(addprefix mta_hybrid-mtalpha-sim/,$(addsuffix .o,$(notdir $(basename $(MTALIB_SRC))))) \
	$(addprefix mta_hybrid-mtalpha-sim/,$(addsuffix .o,$(notdir $(basename $(MTALIB_SIM_SRC)))))

CLEANFILES += \
	$(mta_hybrid_mtalpha_sim_libsl_a_CONTENTS) \
	mta_hybrid-mtalpha-sim/libsl.a

mta_hybrid-mtalpha-sim/%.o: $(srcdir)/src/%.c
	$(AM_V_at)$(MKDIR_P) mta_hybrid-mtalpha-sim
	$(slc_verbose)$(SLC_MTA) -c -o $@ $<

mta_hybrid-mtalpha-sim/gdtoa_%.o: $(srcdir)/src/gdtoa/%.c
	$(AM_V_at)$(MKDIR_P) mta_hybrid-mtalpha-sim
	$(slc_verbose)$(SLC_MTA) -c -o $@ $<

mta_hybrid-mtalpha-sim/%.a:
	$(AM_V_at)rm -f $@
	$(AM_V_AR)$(AR_MTALPHA) cru $@ $^
	$(AM_V_at)$(RANLIB_MTALPHA) $@

mta_hybrid-mtalpha-sim/libsl.a: $(mta_hybrid_mtalpha_sim_libsl_a_CONTENTS)

### seq-naked targets

#$(eval $(call MTA_TEMPLATE,mta_s,seq_naked-mtalpha-sim,seq_naked_mtalpha_sim)

nobase_pkglib_DATA += \
   seq_naked-mtalpha-sim/libsl.a

seq_naked_mtalpha_sim_libsl_a_CONTENTS = \
	$(MTALIB_COBJS) \
	$(addprefix seq_naked-mtalpha-sim/gdtoa_,$(addsuffix .o,$(notdir $(basename $(GDTOA_SRC))))) \
	$(addprefix seq_naked-mtalpha-sim/,$(addsuffix .o,$(notdir $(basename $(MTALIB_SRC))))) \
	$(addprefix seq_naked-mtalpha-sim/,$(addsuffix .o,$(notdir $(basename $(MTALIB_SIM_SRC)))))

CLEANFILES += \
	$(seq_naked_mtalpha_sim_libsl_a_CONTENTS) \
	seq_naked-mtalpha-sim/libsl.a

SLC_MTA_S = $(SLC_RUN) -b mta_s $(SHUTUP)

seq_naked-mtalpha-sim/%.o: $(srcdir)/src/%.c
	$(AM_V_at)$(MKDIR_P) seq_naked-mtalpha-sim
	$(slc_verbose)$(SLC_MTA_S) -c -o $@ $<

seq_naked-mtalpha-sim/gdtoa_%.o: $(srcdir)/src/gdtoa/%.c
	$(AM_V_at)$(MKDIR_P) seq_naked-mtalpha-sim
	$(slc_verbose)$(SLC_MTA_S) -c -o $@ $<

seq_naked-mtalpha-sim/%.a:
	$(AM_V_at)rm -f $@
	$(AM_V_AR)$(AR_MTALPHA) cru $@ $^
	$(AM_V_at)$(RANLIB_MTALPHA) $@

seq_naked-mtalpha-sim/libsl.a: $(seq_naked_mtalpha_sim_libsl_a_CONTENTS)

### MT-naked targets

#$(eval $(call MTA_TEMPLATE,mta_n,mta_naked-mtalpha-sim,mta_naked_mtalpha_sim)

nobase_pkglib_DATA += \
   mta_naked-mtalpha-sim/libsl.a

mta_naked_mtalpha_sim_libsl_a_CONTENTS = \
	$(MTALIB_COBJS) \
	$(addprefix mta_naked-mtalpha-sim/gdtoa_,$(addsuffix .o,$(notdir $(basename $(GDTOA_SRC))))) \
	$(addprefix mta_naked-mtalpha-sim/,$(addsuffix .o,$(notdir $(basename $(MTALIB_SRC))))) \
	$(addprefix mta_naked-mtalpha-sim/,$(addsuffix .o,$(notdir $(basename $(MTALIB_SIM_SRC)))))

CLEANFILES += \
	$(mta_naked_mtalpha_sim_libsl_a_CONTENTS) \
	mta_naked-mtalpha-sim/libsl.a

SLC_MTA_N = $(SLC_RUN) -b mta_n $(SHUTUP)

mta_naked-mtalpha-sim/%.o: $(srcdir)/src/%.c
	$(AM_V_at)$(MKDIR_P) mta_naked-mtalpha-sim
	$(slc_verbose)$(SLC_MTA_N) -c -o $@ $<

mta_naked-mtalpha-sim/gdtoa_%.o: $(srcdir)/src/gdtoa/%.c
	$(AM_V_at)$(MKDIR_P) mta_naked-mtalpha-sim
	$(slc_verbose)$(SLC_MTA_N) -c -o $@ $<

mta_naked-mtalpha-sim/%.a:
	$(AM_V_at)rm -f $@
	$(AM_V_AR)$(AR_MTALPHA) cru $@ $^
	$(AM_V_at)$(RANLIB_MTALPHA) $@

mta_naked-mtalpha-sim/libsl.a: $(mta_naked_mtalpha_sim_libsl_a_CONTENTS)


endif

