//
// fibonacci.c: this file is part of the SL program suite.
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

#include <stdio.h>
#include <assert.h>
#include <svp/slr.h>

#define INT unsigned long

sl_def(fibo_compute, void,
       sl_shparm(INT, prev), sl_shparm(INT, prev2), sl_glparm(INT*, fibo))
{
  sl_index(i);

  INT n = sl_getp(prev) + sl_getp(prev2);
  sl_setp(prev2, sl_getp(prev));
  sl_setp(prev, n);
  sl_getp(fibo)[i] = n;
}
sl_enddef

sl_def(fibo_print, void,
       sl_shparm(INT, guard), sl_glparm(INT*, fibo))
{
  sl_index(i);

  INT p1 = sl_getp(fibo)[i - 2];
  INT p2 = sl_getp(fibo)[i - 1];
  INT p3 = sl_getp(fibo)[i];

  INT n = sl_getp(guard);
  printf("The %luth Fibonacci number is %lu + %lu = %lu\n", (INT)i, p1, p2, p3);
  sl_setp(guard, n);
}
sl_enddef

slr_decl(slr_var(int, N, "number of fibonacci numbers to generate"));

// SLT_RUN:  N=6
// SLT_RUN:  N=3

sl_def(t_main, void)
{
  assert(slr_len(N) >= 1);
  unsigned N = slr_get(N)[0] + 1;

  INT fibonums[N];

  // first, compute the numbers.
  fibonums[0] = fibonums[1] = 1;
  sl_create(,,2,N,,,,
	    fibo_compute,
	    sl_sharg(INT, prev, 1),
	    sl_sharg(INT, prev2, 1),
	    sl_glarg(INT*, fibo, fibonums));
  sl_sync();

  // then, print them.
  sl_create(,,2,N,,,,
	    fibo_print,
	    sl_sharg(INT, guard, 0), sl_glarg(INT*, t, fibonums));
  sl_sync();

}
sl_enddef
