//
// roman.c: this file is part of the SL toolchain.
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

#include <svp/roman.h>
#include <svp/iomacros.h>
#include <svp/slr.h>

slr_decl(slr_var(short, N, "number to print"));

// SLT_RUN:  N=42

sl_def(t_main, void)
{
  if (!slr_len(N))
    puts("no number specified!\n");
  else {
    sl_proccall(roman, sl_glarg(short, x, slr_get(N)[0]));
    putc('\n');
  }
}
sl_enddef
