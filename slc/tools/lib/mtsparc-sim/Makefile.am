COMMON_TAG = mtsn
include ../common.mk

nobase_pkglib_LIBRARIES += \
	mtsn_hybrid/libslmain.a \
	mtsn_naked/libslmain.a \
	seq_naked/libslmain.a

mtsn_hybrid_libslmain_a_CFLAGS = -b mtsn
mtsn_hybrid_libslmain_a_SOURCES = $(MAIN_SRC)

mtsn_naked_libslmain_a_CFLAGS = -b mtsn_n
mtsn_naked_libslmain_a_SOURCES = $(MAIN_SRC)

seq_naked_libslmain_a_CFLAGS = -b mtsn_s
seq_naked_libslmain_a_SOURCES = $(MAIN_SRC)

SLRT_SRC = ../mt/mts/slrt.s
include ../mt.mk

