//
// a3.c: this file is part of the SL toolchain.
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

#include <calloca.h>
#include <svp/iomacros.h>

sl_def(foo, void,
       sl_glparm(char*, p1),
       sl_glparm(char*, p2))
{
  char *p1 = sl_getp(p1);
  char *p2 = sl_getp(p2);

  p1[0] = '4';
  p1[1] = '\0';
  p2[0] = '2';
  p2[1] = '\n';
  p2[2] = '\0';
}
sl_enddef

sl_def(t_main, void)
{
  char *p1;
  char p2[10];

  p1 = (char*)alloca(10);

  sl_proccall(foo,
	      sl_glarg(char*, gp1, p1),
	      sl_glarg(char*, gp2, p2));

  puts(p1); puts(p2);
}
sl_enddef
