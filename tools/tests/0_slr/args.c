//
// args.c: this file is part of the SL toolchain.
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
	 slr_var(int, b, "2nd variable"));

/*
 * SLT_RUN:  a=10  b=-10
 */

sl_def(t_main, void)
{
  int a = slr_get(a)[0];
  int b = slr_get(b)[0];
  int c = a + b;
  output_int(c, 1);
  output_char('\n', 1);
}
sl_enddef
