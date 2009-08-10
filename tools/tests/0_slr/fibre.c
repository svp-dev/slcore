//
// fibre.c: this file is part of the SL toolchain.
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

#include <svp/fibre.h>
#include <svp/testoutput.h>

/*
 * SLT_RUN:
 * SLT_RUN: -f TEST.d
 */

sl_def(t_main, void)
{
  if (fibre_tag(0) != -1) {
    int x = 0;
    x += *(int*)fibre_data(0);
    x += *(int*)fibre_data(1);
    x += *(int*)fibre_data(2);
    x += *(int*)fibre_data(3);
    output_int(x, 1);
    output_char('\n', 1);
  }
}
sl_enddef
