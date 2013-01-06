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
#include <assert.h>
#include <svp/fibre.h>
#include <svp/testoutput.h>

// SLT_RUN: -f TEST.d

// FIXME: uClibc's tgamma may be broken!

sl_def(t_main, void)
{
  int i;
  for (i = 0; fibre_tag(i) != -1; ++i)
  {
      assert(fibre_tag(i) == 2 && fibre_rank(i) == 0);
      double x = *(double*)fibre_data(i);
      
      double t1 = tgamma(x);
      float t2 = tgammaf(x);
      output_float(x, 1, 4);
      output_char(' ', 1);
      output_float(t1, 1, 4);
      output_char(' ', 1);
      output_float(t2, 1, 4);
      output_char('\n', 1);
  }
}
sl_enddef
