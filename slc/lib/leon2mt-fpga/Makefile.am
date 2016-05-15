COMMON_TAG = l2mt

include ../common.mk

nobase_pkglib_LIBRARIES += \
	libc.a \
	seq_naked/libsl.a \
	l2mt_naked/libsl.a \
	l2mt_hybrid/libsl.a

#	libmgos.a 
#	libm.a 

seq_naked_libsl_a_CFLAGS = -b l2mt_s
seq_naked_libsl_a_SOURCES = $(LIBSL_SRC)
seq_naked_libsl_a_LIBADD = $(libslcommon_a_OBJECTS)
seq_naked_libsl_a_DEPENDENCIES = $(seq_naked_libsl_a_LIBADD)

l2mt_naked_libsl_a_CFLAGS = -b l2mt_n
l2mt_naked_libsl_a_SOURCES = $(LIBSL_SRC)
l2mt_naked_libsl_a_LIBADD = $(libslcommon_a_OBJECTS)
l2mt_naked_libsl_a_DEPENDENCIES = $(mts_naked_libsl_a_LIBADD)

l2mt_hybrid_libsl_a_CFLAGS = -b l2mt
l2mt_hybrid_libsl_a_SOURCES = $(LIBSL_SRC)
l2mt_hybrid_libsl_a_LIBADD = $(libslcommon_a_OBJECTS)
l2mt_hybrid_libsl_a_DEPENDENCIES = $(mts_hybrid_libsl_a_LIBADD)

# include ../mgos.mk
include ../libc.mk

#include mtamathobjs.mk
#libm_a_CFLAGS  = -b $(COMMON_TAG)
#libm_a_SOURCES = ../libc/math/missing_uclibc_math.c 
#libm_a_LIBADD  = $(MTAMATHOBJS)
#libm_a_DEPENDENCIES = $(libm_a_LIBADD)

MALLOC_DEFS_TARGET    = -DPAGESIZE=0x1000U
TLSMALLOC_DEFS_TARGET = -DARCH_TLS_SERVICES=\"tls_arch_leon2mt_fpga.h\" 
include ../malloc.mk
include ../tlsmalloc.mk

libc_a_LIBADD += \
	$(libmalloc_a_OBJECTS) \
	$(libtlsmalloc_a_OBJECTS)
libc_a_DEPENDENCIES = $(libc_a_LIBADD)
