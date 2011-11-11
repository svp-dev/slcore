COMMON_TAG = ptl_n
include ../common.mk

nobase_pkglib_LIBRARIES += \
	ptl_naked/libslmain.a \
	seq_naked/libslmain.a

seq_naked_libslmain_a_CFLAGS = -b ptl_s
seq_naked_libslmain_a_SOURCES = $(MAIN_SRC)

ptl_naked_libslmain_a_CFLAGS = -b ptl_n
ptl_naked_libslmain_a_SOURCES = $(MAIN_SRC)

include ../host.mk
