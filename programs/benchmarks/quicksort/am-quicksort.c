//
// am-quicksort.c: this file is part of the SL toolchain.
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

#include <cstddef.h>
#include <svp/perf.h>
#include <svp/iomacros.h>
#include <svp/slr.h>

#define INT int
#define SIZE unsigned

sl_def(quicksort2, void,
       sl_glparm(INT*, data),
       sl_glparm(INT*, result),
       sl_shparm(SIZE, lower),
       sl_shparm(SIZE, greater))
{
  sl_index(i);

  INT* d = sl_getp(data);

  if (d[i] < d[0]) {
    sl_setp(greater, sl_getp(greater));

    SIZE l1 = sl_getp(lower);
    sl_setp(lower, l1+1);
    sl_getp(result)[l1] = d[i];
  } else {
    sl_setp(lower, sl_getp(lower));

    SIZE l2 = sl_getp(greater);
    sl_setp(greater, l2-1);
    sl_getp(result)[l2] = d[i];
  }
}
sl_enddef

sl_def(quicksort3, void,
       sl_glparm(INT*, data),
       sl_glparm(INT*, result))
{
  sl_index(i);
  sl_getp(data)[i] = sl_getp(result)[i];
}
sl_enddef

sl_decl(quicksort, void,
	sl_glparm(INT*, data),
	sl_glparm(SIZE, len));

sl_def(quicksort_split, void,
       sl_glparm(INT*, data),
       sl_glparm(SIZE, len1),
       sl_glparm(SIZE, len2))
{
  sl_index(i);
  if (!i) {
    __asm__ __volatile__("#nothing" : : "rf"(1+sl_getp(len2)));
    sl_create(,,,,,,,
	      quicksort,
	      sl_glarg(INT*, gdata, sl_getp(data)),
	      sl_glarg(SIZE, glen, sl_getp(len1)));
    sl_sync();
  } else {
    SIZE l1 = sl_getp(len1) + 1;
    SIZE l2 = sl_getp(len2) - l1;
    sl_create(,,,,,,,
	      quicksort,
	      sl_glarg(INT*, gdata, sl_getp(data) + l1),
	      sl_glarg(SIZE, glen, l2));
    sl_sync();
  }
}
sl_enddef

sl_def(quicksort, void,
       sl_glparm(INT*, data),
       sl_glparm(SIZE, len))
{
  SIZE l = sl_getp(len);
  INT* d = sl_getp(data);
  INT result[l];
  if (l > 1) {
    sl_create(,,1,l,,,,
	      quicksort2,
	      sl_glarg(INT*, gdata, d),
	      sl_glarg(INT*, gres, result),
	      sl_sharg(SIZE, lower),
	      sl_sharg(SIZE, greater));
    sl_seta(lower, 0);
    sl_seta(greater, l-1);
    sl_sync();
    SIZE la = sl_geta(lower);
    result[la] = d[0];
    sl_create(,,0,l,,,,
	      quicksort3,
	      sl_glarg(INT*, gdata2, d),
	      sl_glarg(INT*, gres2, result));
    sl_sync();
    if (l > 2) {
      sl_create(,,,2,,,,
		quicksort_split,
		sl_glarg(INT*, gdata, d),
		sl_glarg(SIZE, glen1, la),
		sl_glarg(SIZE, glen2, l));
      sl_sync();
    }
  }
}
sl_enddef

sl_def(print_list, void,
       sl_glparm(INT*, data),
       sl_shparm(int, guard))
{
  sl_index(i);
  INT n = sl_getp(data)[i];

  int g = sl_getp(guard);
  putn(n); putc('\n');
  sl_setp(guard, g);
}
sl_enddef


slr_decl(slr_var(INT, data, "data to sort"),
	 slr_var(int, Pr, "if nonempty: print results afterwards"),
	 slr_var(int, Pc, "if nonempty: print counter afterwards"));

sl_def(t_main, void)
{
  INT *d = slr_get(data);
  SIZE l = slr_len(data);
  int64_t p1, p2;

  p1 = get_cycles();
  sl_create(,,,,,,,
	    quicksort,
	    sl_glarg(INT*, data, d),
	    sl_glarg(SIZE, len, l));
  sl_sync();
  p2 = get_cycles();

  if (slr_len(Pc))
    printf("%d\n", p2 - p1);

  if (slr_len(Pr)) {
    puts("Sorted data:\n");
    sl_create(,,,l,,,, print_list,
	      sl_glarg(INT*, data, d),
	      sl_sharg(int, guard, 0));
    sl_sync();
  }
}
sl_enddef
