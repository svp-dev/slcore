//
// x_wrbv.ut.c: this file is part of the slc project.
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

ut_def(foo, int)
{
  ut_break(0);
}
ut_enddef

// XFAIL: C (break is immutable)
ut_def(t_main, void)
{
  ut_create(f,,,,,, int, foo);
  ut_sync(f);
  ut_getbr(f) = 10;
}
ut_enddef
