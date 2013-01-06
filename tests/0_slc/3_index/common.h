//
// common.h: this file is part of the slc project.
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
//
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <svp/delegate.h>

#define SAMPLES 200
int indices[SAMPLES];

sl_def(icount, void,
       sl_shparm(unsigned, count),
       sl_glparm(unsigned, max))
{
  sl_index(i);
  if (sl_getp(count) >= sl_getp(max)) {
      sl_setp(count, sl_getp(count));
      sl_break ;
  }
  indices[sl_getp(count)] = i;
  sl_setp(count, sl_getp(count) + 1);
}
sl_enddef

sl_def(iprint, int,
       sl_shparm(unsigned, count),
       sl_glparm(unsigned, refcount))
{
  sl_index(i);
  unsigned c = sl_getp(count);
  if (c >= sl_getp(refcount))
  {
      sl_setp(count, c);
      sl_break ;
  }
  printf("thread #%u -> index %d\n", c, indices[c]);
  sl_setp(count, c + 1);
}
sl_enddef

int main(int argc, char **argv)
{
  int r;
  int B = 0;
  int N = 20;
  char ch;
  while ((ch = getopt(argc, argv, "b:n:")) != -1) 
      switch (ch) {
      case 'b':
          B = atoi(optarg);
          break;
      case 'n':
          N = atoi(optarg);
          break;
      }

  assert(N <= SAMPLES);
  printf("start = %d, limit = %d, step = %d\n", test_index_params);
  sl_create(,, test_index_params,B, ,
	    icount,
	    sl_sharg(unsigned, count, 0),
            sl_glarg(unsigned, max, N));
  sl_sync(r);
  if (r == SVP_EXIT_BREAK) 
      fprintf(stderr, "broken\n");
  printf("#threads = %d\n", sl_geta(count));

  assert(sl_geta(count) <= SAMPLES);

  sl_create(,,, sl_geta(count),,B,,
	    iprint,
	    sl_sharg(unsigned, c, 0),
	    sl_glarg(unsigned, refc, sl_geta(count)));
  sl_sync();

  return 0;
}

