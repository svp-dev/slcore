COMMON_TAG = mts
include ../common.mk

nobase_pkglib_LIBRARIES += \
	mts_hybrid/libslmain.a \
	seq_naked/libslmain.a

mts_hybrid_libslmain_a_CFLAGS = -b mts
mts_hybrid_libslmain_a_SOURCES = $(MAIN_SRC)

#UTLEON3 does not support suspending allocate for now
#mts_naked_libslmain_a_CFLAGS = -b mts_n
#mts_naked_libslmain_a_SOURCES = $(MAIN_SRC)

seq_naked_libslmain_a_CFLAGS = -b mts_s
seq_naked_libslmain_a_SOURCES = $(MAIN_SRC)

SLRT_SRC = ../mt/mts/slrt.s
include ../mt.mk

