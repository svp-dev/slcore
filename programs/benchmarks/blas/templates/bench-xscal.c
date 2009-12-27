//
// bench-xscal.c: this file is part of the SL program suite.
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
DEF_SCALAR(a)
DEF_ARRAY_INOUT(sx)
DEF_COUNTER(incx)
END_VARS

BEGIN_READ
READ_COUNTER(n)
READ_SCALAR(a)
READ_ARRAY_INOUT(sx, n)
READ_COUNTER(incx)
END_READ

BEGIN_PREPARE
RESET_ARRAY_INOUT(sx, n)
END_PREPARE

BEGIN_WORK
     sl_proccall(FUNCTION,
		 sl_glarg(long, , USE_VAR(n)),
		 sl_glfarg(FLOAT, , USE_VAR(a)),
		 sl_glarg(FLOAT*, , USE_VAR(sx)),
		 sl_glarg(long, , USE_VAR(incx)));
END_WORK

BEGIN_OUTPUT
PRINT_ARRAY(sx, n)
END_OUTPUT

BEGIN_TEARDOWN
FREE_ARRAY_INOUT(sx)
END_TEARDOWN

BEGIN_DESC
BENCH_TITLE("BLAS: _SCAL")
BENCH_AUTHOR("kena")
BENCH_DESC("Compute X[i] = A * X[i]")
END_DESC
