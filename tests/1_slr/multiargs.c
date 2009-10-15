//
// multiargs.c: this file is part of the SL toolchain.
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

#include <svp/slr.h>
#include <svp/testoutput.h>

slr_decl(slr_var(int, a),
	 slr_var(int, b));
slr_decl(slr_var(int, c));

// SLT_RUN: a=22 b=10 c=10

sl_def(t_main, void)
{
  output_int(slr_get(a)[0], 1);
  output_char('\n', 1);
  output_int(slr_get(b)[0], 1);
  output_char('\n', 1);
  output_int(slr_get(c)[0], 1);
  output_char('\n', 1);
}
sl_enddef
