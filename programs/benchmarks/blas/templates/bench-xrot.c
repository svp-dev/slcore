//
// bench-xrot.c: this file is part of the SL program suite.
//
// Copyright (C) 2009 The SL project.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//

#include "blasbench.h"

BEGIN_VARS
DEF_COUNTER(n)
DEF_ARRAY_INOUT(sx)
DEF_COUNTER(incx)
DEF_ARRAY_INOUT(sy)
DEF_COUNTER(incy)
DEF_SCALAR(c)
DEF_SCALAR(s)
END_VARS

BEGIN_READ
READ_COUNTER(n)
READ_ARRAY_INOUT(sx, n)
READ_COUNTER(incx)
READ_ARRAY_INOUT(sy, n)
READ_COUNTER(incy)
READ_SCALAR(c)
READ_SCALAR(s)
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
		 sl_glarg(long, , USE_VAR(incy)),
		 sl_glfarg(FLOAT, , USE_VAR(c)),
		 sl_glfarg(FLOAT, , USE_VAR(s)));
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
BENCH_TITLE("BLAS: _ROT")
BENCH_AUTHOR("kena")
BENCH_DESC("Compute Y[i] := C*X[i] + S*Y[i]; X[i] := C*Y[i] + S*X[i]")
END_DESC
