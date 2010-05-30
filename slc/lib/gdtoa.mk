GDTOA_SRC = \
	src/gdtoa/dmisc.c \
	src/gdtoa/dtoa.c \
	src/gdtoa/g__fmt.c \
	src/gdtoa/g_ddfmt.c \
	src/gdtoa/g_dfmt.c \
	src/gdtoa/g_ffmt.c \
	src/gdtoa/g_Qfmt.c \
	src/gdtoa/g_xfmt.c \
	src/gdtoa/g_xLfmt.c \
	src/gdtoa/gdtoa.c \
	src/gdtoa/gethex.c \
	src/gdtoa/gmisc.c \
	src/gdtoa/hd_init.c \
	src/gdtoa/hexnan.c \
	src/gdtoa/misc.c \
	src/gdtoa/smisc.c \
	src/gdtoa/strtod.c \
	src/gdtoa/strtodg.c \
	src/gdtoa/strtodI.c \
	src/gdtoa/strtodnrp.c \
	src/gdtoa/strtof.c \
	src/gdtoa/strtoId.c \
	src/gdtoa/strtoIdd.c \
	src/gdtoa/strtoIf.c \
	src/gdtoa/strtoIg.c \
	src/gdtoa/strtoIQ.c \
	src/gdtoa/strtoIx.c \
	src/gdtoa/strtoIxL.c \
	src/gdtoa/strtopd.c \
	src/gdtoa/strtopdd.c \
	src/gdtoa/strtopf.c \
	src/gdtoa/strtopQ.c \
	src/gdtoa/strtopx.c \
	src/gdtoa/strtopxL.c \
	src/gdtoa/strtord.c \
	src/gdtoa/strtordd.c \
	src/gdtoa/strtorf.c \
	src/gdtoa/strtorQ.c \
	src/gdtoa/strtorx.c \
	src/gdtoa/strtorxL.c \
	src/gdtoa/sum.c \
	src/gdtoa/ulp.c

GDTOA_EXTRA = \
	src/gdtoa/arith.h \
	src/gdtoa/arith-mtalpha.h \
	src/gdtoa/gd_qnan.h \
	src/gdtoa/gd_qnan-mtalpha.h \
	src/gdtoa/gdtoa_fltrnds.h \
	src/gdtoa/gdtoaimp.h \
	src/gdtoa/gdtoa.h \
	src/gdtoa/qnan.c \
	src/gdtoa/arithchk.c \
	src/gdtoa/makefile \
	src/gdtoa/README

EXTRA_DIST += $(GDTOA_SRC) $(GDTOA_EXTRA)

