COMMON_TAG = ptld_n

include ../common.mk

nobase_pkglib_LIBRARIES += \
	libmgos.a \
	ptl_naked/libsl.a \
	seq_naked/libsl.a

seq_naked_libsl_a_CFLAGS = -b ptld_s
seq_naked_libsl_a_SOURCES = $(LIBSL_SRC)
seq_naked_libsl_a_LIBADD = $(libslcommon_a_OBJECTS)
seq_naked_libsl_a_DEPENDENCIES = $(seq_naked_libsl_a_LIBADD)

ptl_naked_libsl_a_CFLAGS = -b ptld_n
ptl_naked_libsl_a_SOURCES = $(LIBSL_SRC)
ptl_naked_libsl_a_LIBADD = $(libslcommon_a_OBJECTS)
ptl_naked_libsl_a_DEPENDENCIES = $(ptl_naked_libsl_a_LIBADD)

include ../mgos-host.mk
