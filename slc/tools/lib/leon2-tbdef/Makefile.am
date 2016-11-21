COMMON_TAG = leon2_s
include ../common.mk

nobase_pkglib_LIBRARIES += \
	l2mtseq_naked/libslmain.a

l2mtseq_naked_libslmain_a_CFLAGS = -b leon2_s
l2mtseq_naked_libslmain_a_SOURCES = $(MAIN_SRC)

linkrom: ../leon2mt-tbdef/linkrom
	$(LN_S) $^ $@

linkram: ../leon2mt-tbdef/linkram
	$(LN_S) $^ $@

SLRT_SRC = ../mt/l2-sim/slrt.s

include ../leon2mt-fpga/leon2-common.mk
