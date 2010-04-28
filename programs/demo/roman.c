//
// roman.c: this file is part of the SL program suite.
//
// Copyright (C) 2009,2010 The SL project.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//

#include <svp/roman.h>
#include <svp/testoutput.h>
#include <svp/slr.h>

slr_decl(slr_var(short, N, "number to print"));

// SLT_RUN:  N=42

sl_def(t_main, void)
{
  if (!slr_len(N))
      output_string("no number specified!\n", 2);
  else {
      sl_proccall(roman, sl_glarg(short, x, slr_get(N)[0]));
      output_char('\n', 1);
  }
}
sl_enddef
