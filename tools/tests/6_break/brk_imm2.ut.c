//
// brk_imm2.ut.c: this file is part of the slc project.
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
// $Id$
//

#include <libutc.h>

//test break with some control flow
int e,x;

ut_def(foo, int)
{
    x=0;

    x = x + 1;
    e = x - 1;
    ut_break(254);
    e = e - 1;
}
ut_enddef

ut_def(t_main, void)
{
  // do nothing
}
ut_enddef
