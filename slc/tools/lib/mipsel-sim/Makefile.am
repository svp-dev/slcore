COMMON_TAG = mipsel_s
include ../common.mk

nobase_pkglib_LIBRARIES += \
	seq_naked/libslmain.a

seq_naked_libslmain_a_CFLAGS = -b mipsel_s
seq_naked_libslmain_a_SOURCES = $(MAIN_SRC)

SLRT_SRC = ../mt/mipsel/slrt.s
include ../mt.mk

