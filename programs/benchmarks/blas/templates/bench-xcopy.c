//
// bench-xcopy.c: this file is part of the SL program suite.
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
DEF_ARRAY_IN(sx)
DEF_COUNTER(incx)
DEF_ARRAY_INOUT(sy)
DEF_COUNTER(incy)
END_VARS

BEGIN_READ
READ_COUNTER(n)
READ_ARRAY_IN(sx, n)
READ_COUNTER(incx)
READ_ARRAY_INOUT(sy, n)
READ_COUNTER(incy)
END_READ

BEGIN_PREPARE
RESET_ARRAY_INOUT(sy, n)
END_PREPARE

BEGIN_WORK
     sl_proccall(FUNCTION,
		 sl_glarg(long, , USE_VAR(n)),
		 sl_glarg(const FLOAT*, , USE_VAR(sx)),
		 sl_glarg(long, , USE_VAR(incx)),
		 sl_glarg(FLOAT*, , USE_VAR(sy)),
		 sl_glarg(long, , USE_VAR(incy)));
END_WORK

BEGIN_OUTPUT
PRINT_ARRAY(sy, n)
END_OUTPUT

BEGIN_TEARDOWN
FREE_ARRAY_IN(sx)
FREE_ARRAY_INOUT(sy)
END_TEARDOWN

BEGIN_DESC
BENCH_TITLE("BLAS: _COPY")
BENCH_AUTHOR("kena")
BENCH_DESC("Perform Y[i] := X[i]")
END_DESC
