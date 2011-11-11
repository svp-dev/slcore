COMMON_TAG = ptld_n
include ../common.mk

nobase_pkglib_LIBRARIES += \
	ptl_naked/libslmain.a \
	seq_naked/libslmain.a

seq_naked_libslmain_a_CFLAGS = -b ptld_s
seq_naked_libslmain_a_SOURCES = $(MAIN_SRC)

ptl_naked_libslmain_a_CFLAGS = -b ptld_n
ptl_naked_libslmain_a_SOURCES = $(MAIN_SRC)

include ../host.mk
