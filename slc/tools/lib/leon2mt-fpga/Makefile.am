COMMON_TAG = l2mt_f
include ../common.mk

nobase_pkglib_LIBRARIES += \
	l2mt_hybrid/libslmain.a \
	l2mt_naked/libslmain.a \
	l2mtseq_naked/libslmain.a

l2mt_hybrid_libslmain_a_CFLAGS = -b l2mt_f
l2mt_hybrid_libslmain_a_SOURCES = $(MAIN_SRC)

l2mt_naked_libslmain_a_CFLAGS = -b l2mt_nf
l2mt_naked_libslmain_a_SOURCES = $(MAIN_SRC)

l2mtseq_naked_libslmain_a_CFLAGS = -b l2mt_sf
l2mtseq_naked_libslmain_a_SOURCES = $(MAIN_SRC)

SLRT_SRC = ../mt/l2mt/slrt.s

include leon2-common.mk
