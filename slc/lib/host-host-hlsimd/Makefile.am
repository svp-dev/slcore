COMMON_TAG = hlsd

include ../common.mk

nobase_pkglib_LIBRARIES += \
	libmgos.a \
	ptl_naked/libsl.a \
	ptl_hybrid/libsl.a \
	seq_naked/libsl.a

seq_naked_libsl_a_CFLAGS = -b hlsd_s
seq_naked_libsl_a_SOURCES = $(SPEC_SOURCES)
seq_naked_libsl_a_LIBADD = $(libslcommon_a_OBJECTS)
seq_naked_libsl_a_DEPENDENCIES = $(seq_naked_libsl_a_LIBADD)

ptl_naked_libsl_a_CFLAGS = -b hlsd_n
ptl_naked_libsl_a_SOURCES = $(SPEC_SOURCES)
ptl_naked_libsl_a_LIBADD = $(libslcommon_a_OBJECTS)
ptl_naked_libsl_a_DEPENDENCIES = $(ptl_naked_libsl_a_LIBADD)

ptl_hybrid_libsl_a_CFLAGS = -b hlsd
ptl_hybrid_libsl_a_SOURCES = $(SPEC_SOURCES)
ptl_hybrid_libsl_a_LIBADD = $(libslcommon_a_OBJECTS)
ptl_hybrid_libsl_a_DEPENDENCIES = $(ptl_hybrid_libsl_a_LIBADD)

include ../mgos-host.mk
