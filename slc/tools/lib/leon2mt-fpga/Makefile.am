
COMMON_TAG = l2mt
include ../common.mk

nobase_pkglib_LIBRARIES += \
	l2mt_hybrid/libslmain.a \
	l2mt_naked/libslmain.a \
	l2mtseq_naked/libslmain.a

l2mt_hybrid_libslmain_a_CFLAGS = -b l2mt
l2mt_hybrid_libslmain_a_SOURCES = $(MAIN_SRC)

l2mt_naked_libslmain_a_CFLAGS = -b l2mt_n
l2mt_naked_libslmain_a_SOURCES = $(MAIN_SRC)

l2mtseq_naked_libslmain_a_CFLAGS = -b l2mt_s
l2mtseq_naked_libslmain_a_SOURCES = $(MAIN_SRC)

SLRT_SRC = ../mt/l2mt/slrt.s
include ../mt.mk

LIBSLC_SRC += ../mt/l2mt/dbgout.c ../common/testoutput.c
