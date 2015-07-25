COMMON_TAG = seqc

include ../common.mk

nobase_pkglib_LIBRARIES += \
	libmgos.a \
	seq_naked/libsl.a \
	seqc_o/libsl.a

seq_naked_libsl_a_CFLAGS = -b seqc
seq_naked_libsl_a_SOURCES = $(LIBSL_SRC)
seq_naked_libsl_a_LIBADD = $(libslcommon_a_OBJECTS)
seq_naked_libsl_a_DEPENDENCIES = $(seq_naked_libsl_a_LIBADD)

seqc_o_libsl_a_CFLAGS = -b seqc_o
seqc_o_libsl_a_SOURCES = $(LIBSL_SRC)
seqc_o_libsl_a_LIBADD = $(libslcommon_a_OBJECTS)
seqc_o_libsl_a_DEPENDENCIES = $(seqc_o_libsl_a_LIBADD)

include ../mgos-host.mk
