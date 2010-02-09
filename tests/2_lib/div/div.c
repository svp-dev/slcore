//
// div.c: this file is part of the SL toolchain.
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

#include <svp/div.h>
#include <svp/slr.h>
#include <cstdio.h>

sl_def(testu, void,
       sl_glparm(uint64_t, x),
       sl_glparm(uint64_t, y))
{
  uint64_t lx = sl_getp(x), ly = sl_getp(y);
  printf("unsigned: %llu : %llu = ", (unsigned long long)lx, (unsigned long long)ly);
  divmodu(lx, ly);
  printf("%llu : %llu\n", (unsigned long long)lx, (unsigned long long)ly);
}
sl_enddef

sl_def(tests, void,
       sl_glparm(int64_t, x),
       sl_glparm(int64_t, y))
{
  int64_t lx = sl_getp(x), ly = sl_getp(y);
  printf("signed: %lld : %lld = ", (long long)lx, (long long)ly);
  divmods(lx, ly);
  printf("%lld : %lld\n", (long long)lx, (long long)ly);
}
sl_enddef

slr_decl(slr_var(unsigned long long, ua),
         slr_var(unsigned long long, ub),
         slr_var(signed long long, sa),
         slr_var(signed long long, sb));

// SLT_RUN:  ua=42  sa=42  ub=5  sb=5
// SLT_RUN:  ua=5  sa=5  ub=42  sb=42

// SLT_RUN:  ua=0  sa=0  ub=1  sb=1
// SLT_RUN:  ua=0  sa=0  ub=-1  sb=-1

// SLT_RUN:  ua=301  sa=301  ub=-10  sb=-10
// SLT_RUN:  ua=-301  sa=-301  ub=-10  sb=-10
// SLT_RUN:  ua=-301  sa=-301  ub=10  sb=10
// SLT_RUN:  ua=301  sa=301  ub=10  sb=10

// SLT_RUN:  ua=0xffffffffUL  sa=0xffffffffUL  ub=0x7ffffffeUL  sb=0x7ffffffeUL
// SLT_RUN:  ua=0xffffffffffffffffULL  sa=0xffffffffffffffffULL  ub=0x7ffffffffffffffeULL  sb=0x7ffffffffffffffeULL

sl_def(t_main, void)
{
  puts("----");
  sl_proccall(testu,
	      sl_glarg(uint64_t, x, slr_get(ua)[0]),
	      sl_glarg(uint64_t, y, slr_get(ub)[0]));
  sl_proccall(tests,
	      sl_glarg(int64_t, x, slr_get(sa)[0]),
	      sl_glarg(int64_t, y, slr_get(sb)[0]));
}
sl_enddef
