
nobase_sllib_DATA += linkrom linkram

include ../mt.mk

LIBSLC_SRC += ../mt/l2mt/dbgout.c ../common/testoutput.c ../mt/l2mt/bootstrap.c \
	../mt/l2mt/init_uart.s ../mt/l2mt/putchar.s ../mt/l2mt/uart_defs.inc \
	../mt/l2mt/tls.inc ../mt/l2mt/start.inc ../mt/l2mt/filler.inc \
	../mt/l2mt/globals.inc \
	$(top_srcdir)/../../lib/libc/bsd/libc/string/memset.c \
	$(top_srcdir)/../../lib/libc/bsd/libc/string/memcpy.c \
	$(top_srcdir)/../../lib/libc/bsd/libc/string/memcmp.c \
	$(top_srcdir)/../../lib/libc/bsd/libc/string/memmove.c
AM_CCASFLAGS = -Wa,-I$(srcdir)/../mt/l2mt/
libslrt_a_CCASFLAGS = $(AM_CCASFLAGS)

# softfloat
# all of this is in libgcc's fp-bit.c
FPBIT_FUNCS = _pack_sf _unpack_sf _addsub_sf _mul_sf _div_sf \
    _fpcmp_parts_sf _compare_sf _eq_sf _ne_sf _gt_sf _ge_sf \
    _lt_sf _le_sf _unord_sf _si_to_sf _sf_to_si _negate_sf _make_sf \
    _sf_to_df _sf_to_tf _thenan_sf _sf_to_usi _usi_to_sf
AM_CPPFLAGS += -DFLOAT -DBITS_PER_UNIT=8 -DFLOAT_ONLY
LIBSLC_SRC += \
	../common/libgcc/fp-bit.c \
	../common/libgcc/fp-bit.h \
	../common/libgcc/clzsi2.c \
	../common/libgcc/ctzsi2.c \
	../common/libgcc/soft-fp/soft-fp.h \
	../common/libgcc/soft-fp/op-1.h \
	../common/libgcc/soft-fp/op-2.h \
	../common/libgcc/soft-fp/op-4.h \
	../common/libgcc/soft-fp/op-8.h \
	../common/libgcc/soft-fp/op-common.h \
	../common/libgcc/soft-fp/sfp-machine.h \
	../common/libgcc/soft-fp/longlong.h
