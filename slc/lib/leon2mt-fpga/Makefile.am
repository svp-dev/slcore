COMMON_TAG = l2mt

include ../common.mk

nobase_pkglib_LIBRARIES += \
	libc.a \
	libm.a \
	l2mtseq_naked/libsl.a \
	l2mt_naked/libsl.a \
	l2mt_hybrid/libsl.a 

# 	libmgos.a 
#	libm.a 

l2mtseq_naked_libsl_a_CFLAGS = -b l2mt_s
l2mtseq_naked_libsl_a_SOURCES = $(LIBSL_SRC)
l2mtseq_naked_libsl_a_LIBADD = $(libslcommon_a_OBJECTS)
l2mtseq_naked_libsl_a_DEPENDENCIES = $(l2mtseq_naked_libsl_a_LIBADD)

l2mt_naked_libsl_a_CFLAGS = -b l2mt_n
l2mt_naked_libsl_a_SOURCES = $(LIBSL_SRC)
l2mt_naked_libsl_a_LIBADD = $(libslcommon_a_OBJECTS)
l2mt_naked_libsl_a_DEPENDENCIES = $(l2mt_naked_libsl_a_LIBADD)

l2mt_hybrid_libsl_a_CFLAGS = -b l2mt
l2mt_hybrid_libsl_a_SOURCES = $(LIBSL_SRC)
l2mt_hybrid_libsl_a_LIBADD = $(libslcommon_a_OBJECTS)
l2mt_hybrid_libsl_a_DEPENDENCIES = $(l2mt_hybrid_libsl_a_LIBADD)

# include ../mgos.mk
include ../libc.mk

#include mtamathobjs.mk
libm_a_CFLAGS  = -b $(COMMON_TAG)
libm_a_SOURCES = ../libc/math/e_sqrtf.c ../libc/math/s_roundf.c
#libm_a_SOURCES = ../libc/math/missing_uclibc_math.c 
#libm_a_LIBADD  = $(MTAMATHOBJS)
#libm_a_DEPENDENCIES = $(libm_a_LIBADD)

include ../smallmalloc.mk

libc_a_LIBADD += \
	$(libsmallmalloc_a_OBJECTS)
libc_a_DEPENDENCIES = $(libc_a_LIBADD)
