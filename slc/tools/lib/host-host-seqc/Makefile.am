COMMON_TAG = seqc
include ../common.mk

nobase_pkglib_LIBRARIES += \
	seq_naked/libslmain.a \
	seqc_o/libslmain.a

seq_naked_libslmain_a_CFLAGS = -b seqc
seq_naked_libslmain_a_SOURCES = $(MAIN_SRC)

seqc_o_libslmain_a_CFLAGS = -b seqc_o
seqc_o_libslmain_a_SOURCES = $(MAIN_SRC)

include ../host.mk
