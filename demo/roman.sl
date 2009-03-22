//                                                             -*- m4 -*-
// roman.sl: this file is part of the slc project.
//
// Copyright (C) 2009 The SL project.
// All rights reserved.
//
// $Id$
//
m4_include(svp/roman.slh)
m4_include(svp/iomacros.slh)

sl_def(t_main, void)
{
  sl_proccall(roman, sl_glarg(short, x, 42));
  putc('\n');
}
sl_enddef
