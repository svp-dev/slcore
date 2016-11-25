COMMON_TAG = spr-l2mtseq_naked-host-host-l2mtseq
include ../common.mk

nobase_pkglib_LIBRARIES += \
	libmgos.a \
	l2mtseq_naked/libsl.a 

l2mtseq_naked_libsl_a_CFLAGS = -b seqc
l2mtseq_naked_libsl_a_SOURCES = $(LIBSL_SRC)
l2mtseq_naked_libsl_a_LIBADD = $(libslcommon_a_OBJECTS)
l2mtseq_naked_libsl_a_DEPENDENCIES = $(l2mtseq_naked_libsl_a_LIBADD)

include ../mgos-host.mk
