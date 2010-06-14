//
// clock.c: this file is part of the SL toolchain.
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

#include <time.h>
#include <stdio.h>

// XIGNORE: *:D

sl_def(t_main, void)
{
  clock_t c1 = clock();
  puts("hello, world!");
  clock_t c2 = clock();
  printf("time to print message: %d clocks = %g seconds\n",
	 (int)(c2-c1),
	 (float)(c2-c1)/(float)CLOCKS_PER_SEC);
}
sl_enddef
