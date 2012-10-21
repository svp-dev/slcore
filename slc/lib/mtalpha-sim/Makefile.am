COMMON_TAG = mta

include ../common.mk

nobase_pkglib_LIBRARIES += \
	libmgos.a \
	libc.a \
	libm.a \
	seq_naked/libsl.a \
	mta_naked/libsl.a \
	mta_hybrid/libsl.a

seq_naked_libsl_a_CFLAGS = -b mta_s
seq_naked_libsl_a_SOURCES = $(LIBSL_SRC)
seq_naked_libsl_a_LIBADD = $(libslcommon_a_OBJECTS)
seq_naked_libsl_a_DEPENDENCIES = $(seq_naked_libsl_a_LIBADD)

mta_naked_libsl_a_CFLAGS = -b mta_n
mta_naked_libsl_a_SOURCES = $(LIBSL_SRC)
mta_naked_libsl_a_LIBADD = $(libslcommon_a_OBJECTS)
mta_naked_libsl_a_DEPENDENCIES = $(mta_naked_libsl_a_LIBADD)

mta_hybrid_libsl_a_CFLAGS = -b mta
mta_hybrid_libsl_a_SOURCES = $(LIBSL_SRC)
mta_hybrid_libsl_a_LIBADD = $(libslcommon_a_OBJECTS)
mta_hybrid_libsl_a_DEPENDENCIES = $(mta_hybrid_libsl_a_LIBADD)

include ../mgos.mk

libmgos_a_SOURCES += \
	../mgos/posix-compat/gtod_mgsim.c \
	../mgos/posix-compat/time_mgsim.c

include ../libc.mk

include mtamathobjs.mk
libm_a_CFLAGS  = -b $(COMMON_TAG)
libm_a_SOURCES = ../libc/math/missing_uclibc_math.c 
libm_a_LIBADD  = $(MTAMATHOBJS)
libm_a_DEPENDENCIES = $(libm_a_LIBADD)

MALLOC_DEFS_TARGET    = -DPAGESIZE=0x8000U
TLSMALLOC_DEFS_TARGET = -DARCH_TLS_SERVICES=\"tls_arch_mtalpha.h\" 
include ../malloc.mk
include ../tlsmalloc.mk

libc_a_LIBADD += \
	$(libmalloc_a_OBJECTS) \
	$(libtlsmalloc_a_OBJECTS)
libc_a_DEPENDENCIES = $(libc_a_LIBADD)

