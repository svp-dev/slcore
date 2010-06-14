//
// tgamma.c: this file is part of the SL toolchain.
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

#include <math.h>
#include <svp/slr.h>
#include <svp/testoutput.h>

slr_decl(slr_var(double, x));

// SLT_RUN: x=0
// SLT_RUN: x=1
// SLT_RUN: x=.42
// SLT_RUN: x=-.69
// SLT_RUN: x=10

// FIXME: uClibc's tgamma may be broken!
// XIGNORE: ppp*:D

sl_def(t_main, void)
{
  double x = slr_get(x)[0];
  double t1 = tgamma(x);
  float t2 = tgammaf(x);
  output_float(x, 1, 4);
  output_char(' ', 1);
  output_float(t1, 1, 4);
  output_char(' ', 1);
  output_float(t2, 1, 4);
  output_char('\n', 1);
}
sl_enddef
