#include "blasbench.h"

BEGIN_VARS
DEF_COUNTER(n)
DEF_ARRAY_INOUT(sx)
DEF_COUNTER(incx)
DEF_ARRAY_INOUT(sy)
DEF_COUNTER(incy)
END_VARS

BEGIN_READ
READ_COUNTER(n)
READ_ARRAY_INOUT(sx, n)
READ_COUNTER(incx)
READ_ARRAY_INOUT(sy, n)
READ_COUNTER(incy)
END_READ

BEGIN_PREPARE
RESET_ARRAY_INOUT(sx, n)
RESET_ARRAY_INOUT(sy, n)
END_PREPARE

BEGIN_WORK
     sl_proccall(FUNCTION,
		 sl_glarg(long, , USE_VAR(n)),
		 sl_glarg(FLOAT*, , USE_VAR(sx)),
		 sl_glarg(long, , USE_VAR(incx)),
		 sl_glarg(FLOAT*, , USE_VAR(sy)),
		 sl_glarg(long, , USE_VAR(incy)));
END_WORK

BEGIN_OUTPUT
PRINT_ARRAY(sx, n)
PRINT_ARRAY(sy, n)
END_OUTPUT

BEGIN_TEARDOWN
FREE_ARRAY_INOUT(sx)
FREE_ARRAY_INOUT(sy)
END_TEARDOWN

BEGIN_DESC
BENCH_TITLE("BLAS: _SWAP")
BENCH_AUTHOR("kena")
BENCH_DESC("Exchange Y[i] and X[i]")
END_DESC