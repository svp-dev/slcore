COMMON_TAG = leon2_s

include ../common.mk

nobase_pkglib_LIBRARIES += \
	libc.a \
	libm.a \
	l2mtseq_naked/libsl.a

# 	libmgos.a 
#	libm.a 

l2mtseq_naked_libsl_a_CFLAGS = -b leon2_s
l2mtseq_naked_libsl_a_SOURCES = $(LIBSL_SRC)
l2mtseq_naked_libsl_a_LIBADD = $(libslcommon_a_OBJECTS)
l2mtseq_naked_libsl_a_DEPENDENCIES = $(l2mtseq_naked_libsl_a_LIBADD)

#include ../mgos.mk
include ../libc.mk

include ../leon2mt-fpga/leon2-common.mk
