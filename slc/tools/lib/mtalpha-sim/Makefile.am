COMMON_TAG = mta
include ../common.mk

nobase_pkglib_LIBRARIES += \
	mta_hybrid/libslmain.a \
	mta_naked/libslmain.a \
	seq_naked/libslmain.a

mta_hybrid_libslmain_a_CFLAGS = -b mta
mta_hybrid_libslmain_a_SOURCES = $(MAIN_SRC)

mta_naked_libslmain_a_CFLAGS = -b mta_n
mta_naked_libslmain_a_SOURCES = $(MAIN_SRC)

seq_naked_libslmain_a_CFLAGS = -b mta_s
seq_naked_libslmain_a_SOURCES = $(MAIN_SRC)

SLRT_SRC = ../mt/mta/slrt.s
include ../mt.mk

