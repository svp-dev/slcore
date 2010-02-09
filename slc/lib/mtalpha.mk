########### MT-Alpha components ###########

EXTRA_DIST += \
	src/abort.c \
	src/atoi.c \
	src/atol.c \
	src/atoll.c \
	src/bcopy.c \
	src/ctype.c \
	src/dtoa_simple.c \
	src/errno.c \
	src/floatio.h \
	src/fpmath.h \
	src/fprintf.c \
	src/fputc.c \
	src/fputs.c \
	src/fwrite.c \
	src/getenv.c \
	src/_hdtoa.c \
	src/heap.c \
	src/malloc.c \
	src/malloc_wrappers.c \
	src/missing_uclibc_math.c \
	src/mtalpha/divide.S \
	src/mtalpha/memcpy.s \
	src/mtalpha/memmove.S \
	src/mtalpha/memset.S \
	src/mtalpha/mtdiv.s \
	src/mtinit.c \
	src/mtsep.c \
	src/mtstdio.h \
	src/mtstdio.c \
	src/printf.c \
	src/printflocal.h \
	src/printfcommon.h \
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

nobase_dist_pkgdata_DATA += \
	mtalpha-sim/include/svp_os.h \
	mtalpha-sim/include/svp_asmdiv.h

if ENABLE_SLC_MTALPHA

nobase_pkglib_DATA += \
   mtalpha-sim/libm.a \
   mtalpha-sim/libsl.a

mtalpha_sim_libsl_a_CONTENTS = \
	mtalpha-sim/abort.o \
	mtalpha-sim/atoi.o \
	mtalpha-sim/atol.o \
	mtalpha-sim/atoll.o \
	mtalpha-sim/bcopy.o \
	mtalpha-sim/ctype.o \
	mtalpha-sim/divl.o \
	mtalpha-sim/divq.o \
	mtalpha-sim/dtoa_simple.o \
	mtalpha-sim/errno.o \
	mtalpha-sim/fprintf.o \
	mtalpha-sim/fputc.o \
	mtalpha-sim/fputs.o \
	mtalpha-sim/fwrite.o \
	mtalpha-sim/getenv.o \
	mtalpha-sim/_hdtoa.o \
	mtalpha-sim/heap.o \
	mtalpha-sim/io.o \
	mtalpha-sim/malloc.o \
	mtalpha-sim/malloc_wrappers.o \
	mtalpha-sim/memcpy.o \
	mtalpha-sim/memmove.o \
	mtalpha-sim/memset.o \
	mtalpha-sim/mtclock.o \
	mtalpha-sim/mtdiv.o \
	mtalpha-sim/mtinit.o \
	mtalpha-sim/mtsep.o \
	mtalpha-sim/mtstdio.o \
	mtalpha-sim/perf.o \
	mtalpha-sim/printf.o \
	mtalpha-sim/printf-pos.o \
	mtalpha-sim/putc.o \
	mtalpha-sim/putchar.o \
	mtalpha-sim/puts.o \
	mtalpha-sim/reml.o \
	mtalpha-sim/remq.o \
	mtalpha-sim/roman.o \
	mtalpha-sim/snprintf.o \
	mtalpha-sim/sprintf.o \
	mtalpha-sim/stpcpy.o \
	mtalpha-sim/stpncpy.o \
	mtalpha-sim/strcat.o \
	mtalpha-sim/strchr.o \
	mtalpha-sim/strcpy.o \
	mtalpha-sim/strdup.o \
	mtalpha-sim/strerror.o \
	mtalpha-sim/strlcat.o \
	mtalpha-sim/strlcpy.o \
	mtalpha-sim/strlen.o \
	mtalpha-sim/strncat.o \
	mtalpha-sim/strncmp.o \
	mtalpha-sim/strncpy.o \
	mtalpha-sim/strnlen.o \
	mtalpha-sim/strtol.o \
	mtalpha-sim/strtoul.o \
	mtalpha-sim/strtoll.o \
	mtalpha-sim/strtoul.o \
	mtalpha-sim/sys_errlist.o \
	mtalpha-sim/vprintf.o \
	mtalpha-sim/vsnprintf.o \
	mtalpha-sim/vfprintf.o

include $(srcdir)/gdtoa.mk

CLEANFILES += $(mtalpha_sim_libsl_a_CONTENTS) \
	mtalpha-sim/libsl.a

include $(srcdir)/src/mtamathobjs.mk
EXTRA_DIST += $(MTAMATHOBJS)

mtalpha_sim_libm_a_CONTENTS = \
	$(MTAMATHOBJS) \
	mtalpha-sim/missing_uclibc_math.o

CLEANFILES += mtalpha-sim/missing_uclibc_math.o \
	mtalpha-sim/libm.a

SLC_MTALPHA = $(SLC_RUN) -b ppp-mtalpha -nostdlib -Dshutup_cstring_h -Dshutup_cstdlib_h -Dshutup_cstdio_h

mtalpha-sim/%.o: $(srcdir)/src/%.c
	$(AM_V_at)$(MKDIR_P) mtalpha-sim
	$(slc_verbose)$(SLC_MTALPHA) -c -o $@ $<

MALLOC_DEFS = -DLACKS_SYS_TYPES_H \
	-DUSE_DL_PREFIX \
	-DHAVE_MMAP=0 \
	-DMORECORE_CANNOT_TRIM \
	-DMALLOC_FAILURE_ACTION="" \
	-DLACKS_UNISTD_H \
	-DLACKS_ERRNO_H \
	-DLACKS_STDLIB_H \
	-DLACKS_STRING_H \
	-DLACKS_SYS_PARAM_H \
	-DLACKS_TIME \
	-DPAGESIZE=0x40000000U

mtalpha-sim/malloc.o: $(srcdir)/src/malloc.c
	$(AM_V_at)$(MKDIR_P) mtalpha-sim
	$(slc_verbose)$(SLC_MTALPHA) -c -o $@ $< $(MALLOC_DEFS)

mtalpha-sim/malloc_debug.o: $(srcdir)/src/malloc.c
	$(AM_V_at)$(MKDIR_P) mtalpha-sim
	$(slc_verbose)$(SLC_MTALPHA) -c -o $@ $< $(MALLOC_DEFS) -DDEBUG -DABORT_ON_ASSERT_FAILURE=0

mtalpha-sim/%.o: $(srcdir)/src/mtalpha/%.s
	$(AM_V_at)$(MKDIR_P) mtalpha-sim
	$(slc_verbose)$(SLC_MTALPHA) -c -o $@ $<

mtalpha-sim/%.o: $(srcdir)/src/mtalpha/%.S
	$(AM_V_at)$(MKDIR_P) mtalpha-sim
	$(slc_verbose)$(SLC_MTALPHA) -c -o $@ $<

mtalpha-sim/divl.o: $(srcdir)/src/mtalpha/divide.S
	$(AM_V_at)$(MKDIR_P) mtalpha-sim
	$(slc_verbose)$(SLC_MTALPHA) -c -o $@ $< -DDIV -DINTSIZE

mtalpha-sim/reml.o: $(srcdir)/src/mtalpha/divide.S
	$(AM_V_at)$(MKDIR_P) mtalpha-sim
	$(slc_verbose)$(SLC_MTALPHA) -c -o $@ $< -DDIV -DINTSIZE

mtalpha-sim/divq.o: $(srcdir)/src/mtalpha/divide.S
	$(AM_V_at)$(MKDIR_P) mtalpha-sim
	$(slc_verbose)$(SLC_MTALPHA) -c -o $@ $< -DDIV

mtalpha-sim/remq.o: $(srcdir)/src/mtalpha/divide.S
	$(AM_V_at)$(MKDIR_P) mtalpha-sim
	$(slc_verbose)$(SLC_MTALPHA) -c -o $@ $< -DREM

mtalpha-sim/libsl.a: $(mtalpha_sim_libsl_a_CONTENTS)
	$(AM_V_at)rm -f $@
	$(AM_V_AR)$(AR_MTALPHA) cru $@ $^
	$(AM_V_at)$(RANLIB_MTALPHA) $@

mtalpha-sim/libm.a: $(mtalpha_sim_libm_a_CONTENTS)
	$(AM_V_at)rm -f $@
	$(AM_V_AR)$(AR_MTALPHA) cru $@ $^
	$(AM_V_at)$(RANLIB_MTALPHA) $@


endif

