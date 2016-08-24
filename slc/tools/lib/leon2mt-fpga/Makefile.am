
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

nobase_sllib_DATA += linkrom

SLRT_SRC = ../mt/l2mt/slrt.s 
include ../mt.mk

LIBSLC_SRC += ../mt/l2mt/dbgout.c ../common/testoutput.c ../mt/l2mt/bootstrap.c

# softfloat
# all of this is in libgcc's fp-bit.c
FPBIT_FUNCS = _pack_sf _unpack_sf _addsub_sf _mul_sf _div_sf \
    _fpcmp_parts_sf _compare_sf _eq_sf _ne_sf _gt_sf _ge_sf \
    _lt_sf _le_sf _unord_sf _si_to_sf _sf_to_si _negate_sf _make_sf \
    _sf_to_df _sf_to_tf _thenan_sf _sf_to_usi _usi_to_sf
AM_CPPFLAGS += -DFLOAT -DBITS_PER_UNIT=8 -DFLOAT_ONLY
LIBSLC_SRC += ../common/libgcc/fp-bit.c ../common/libgcc/clzsi2.c ../common/libgcc/ctzsi2.c
