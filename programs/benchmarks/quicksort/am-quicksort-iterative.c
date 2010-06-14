//
// am-quicksort-iterative.c: this file is part of the SL program suite.
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

#include <stddef.h>
#include <svp/perf.h>
#include <svp/iomacros.h>
#include <svp/slr.h>


#define INT int
#define SIZE unsigned

#define NMAX (1<<20)

static const int level_stripe_size = 10;  // the number of threads to
                 // trigger speculatively to emulate a while construct

struct interval {
  int l, r;
}intervals[2][NMAX];  // the list of intervals for current and the next
                     // level
int num_intervals[2];  // the number of intervals in the list for the
                       // current and the next level

sl_def(copy_array, void,
       sl_glparm(INT*, destination),
       sl_glparm(INT*, source))
{
  sl_index(i);
  sl_getp(destination)[i] = sl_getp(source)[i];
}
sl_enddef

// partitions an interval using data[0] as the pivot
// After all threads run, the value of "lower" reported to the parent
// is the position where the pivot should stay.
sl_def(do_partition_interval, void,
       sl_glparm(INT*, data),
       sl_glparm(INT*, result),
       sl_shparm(SIZE, lower),
       sl_shparm(SIZE, greater)) {
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

int workaround;  // used to work around a bug in the compiler present
                // at the time of this writing that allows a read
                // from a synchronizing register to be optimized away
                // if the value read isn't used

// done: used by a thread to signal it's right sibling when it finished
sl_def(partition_interval, void,
       sl_glparm(INT*, data),
       sl_glparm(INT*, result),
       sl_glparm(SIZE, level),
       sl_shparm(int, done)
       ) {
  sl_index(i);

  INT* data = sl_getp(data);
  INT* result = sl_getp(result);
  SIZE level = sl_getp(level);

  int l = intervals[level%2][i].l;
  int r = intervals[level%2][i].r;

  if (l == r) {
    // this interval is sorted (1 element), so don't copy it to the
    // next level
    result[l] = data[l];
    sl_setp(done, sl_getp(done));
  } else {
    sl_create(,,1,r - l + 1,,,,
	      do_partition_interval,
	      sl_glarg(INT*, gdata, data + l),
	      sl_glarg(INT*, gres, result + l),
	      sl_sharg(SIZE, lower, 0),
	      sl_sharg(SIZE, greater, r - l));
    sl_sync();
    SIZE la = sl_geta(lower);
    la = la + l;
    result[la] = data[l];  // put the pivot in the right place
    // copy the 2 new intervals to next level
    // but after the left sibling has done doing the same
    int left_done = sl_getp(done);
    workaround += left_done;  // use the value, so the read doesn't get
                       // optimized away. See comment for workaround.

    if (l < la) {  // don't copy an interval of len=1
      intervals[(level+1)%2][num_intervals[(level+1) % 2]].l = l;
      intervals[(level+1)%2][num_intervals[(level+1) % 2]].r = la;
      num_intervals[(level+1) % 2]++;
    }
    if (la + 1 < r) {  // don't copy an interval of len=1
      intervals[(level+1)%2][num_intervals[(level+1) % 2]].l = la + 1;
      intervals[(level+1)%2][num_intervals[(level+1) % 2]].r = r;
      num_intervals[(level+1) % 2]++;
    }

    //signal to the right sibling that I'm done
    sl_setp(done, 0);
  }
}
sl_enddef

/* Further partitions a list of intervals
 * level: level of the current list to be partitioned
 */
sl_def(partition_list_of_intervals, void,
       sl_glparm(INT*, data),
       sl_glparm(SIZE, len),
       sl_glparm(INT*, scratch),
       sl_shparm(INT, level),
       sl_shparm(INT, done)
      ) {
  sl_index(i);

  INT done = sl_getp(done);


  if (!done) {
    INT* d = sl_getp(data);
    SIZE l = sl_getp(len);
    INT level = sl_getp(level);
    INT* scratch = sl_getp(scratch);
    //printf("PARTIITON LIST: level = %d\n", level);

    num_intervals[(level+1)%2] = 0;

    // partition the intervals
    sl_create(,,0,num_intervals[level%2],,,,partition_interval,
        sl_glarg(INT*, gdata, d),
        sl_glarg(INT*, gres, scratch),
        sl_glarg(SIZE, level, level),
        sl_sharg(int, done, 1));
    sl_sync();

    // copy partitoned values back to d
    sl_create(,,0,l,,,,
        copy_array,
        sl_glarg(INT*, gdestination, d),
        sl_glarg(INT*, gsource, scratch));
    sl_sync();
    int j = 0;

    if (num_intervals[(level+1)%2] == 0) {
      // no intervals for the next level => we're done
      sl_setp(level, 0);  // value doesn't matter, just unblock sibling
      sl_setp(done, 1);
    } else {
      // trigger the next sibling to start
      sl_setp(level, level+1);
      sl_setp(done, 0);
    }
  } else {  // if (!done)
    sl_setp(level, 0);  // value doesn't matter, just unblock sibling
    sl_setp(done, 1);
  }
}
sl_enddef


sl_def(print_list, void,
       sl_glparm(INT*, data),
       sl_glparm(INT, data_len),
       sl_shparm(int, guard))
{
  sl_index(i);
  INT n = sl_getp(data)[i];
  INT size = sl_getp(data_len);

  int g = sl_getp(guard);

  putn(n);
  if (i == size - 1)
    putc('\n');
  else
    putc(' ');
  sl_setp(guard, g);
}
sl_enddef


slr_decl(slr_var(INT, data, "data to sort"),
	 slr_var(INT, Pc, "if nonempty: print number of cycles"),
	 slr_var(INT, Pr, "if nonempty: print sorted vector"),
	 slr_var(INT, Prb, "if nonempty: print data vector"),
   slr_var(INT, N, "if nonempty: data will be extended to the specified size and the remaining slots filled with values (N - original size)..1"));

sl_def(t_main, void) {
  int64_t p1, p2;

  INT *input_vector = slr_get(data);
  SIZE input_size = slr_len(data);

  SIZE size = input_size;
  if (slr_len(N))
    size = slr_get(N)[0];

  INT data[size];  // this has to be defined in the top most scope of
                   // of main, that's why I've taken it out of the if
                   // block. On seqc and ptl, the vector's locations
                   // on the stack were overwritten once it was
                   // getting out of scope. On ppp this didn't happen
                   // TODO(andreimatei): investigate why
  INT scratch[size];  // TODO(andreimatei): maybe not allocate these
                      // on the stack... The intervals are globals
                      // anyway... Or fix everything when malloc happens.


  if (slr_len(N)) {
    SIZE original_input_size = input_size;
    input_size = size;
    int i;
    for (i = 0; i < original_input_size; i++) data[i] = input_vector[i];
    for (; i < input_size; i++) data[i] = input_size - i;
    input_vector = data;
  }

  if (slr_len(Prb)) {
    puts("Initial data:\n");
    sl_create(,,,input_size,,,, print_list,
        sl_glarg(INT*, aadata, input_vector),
        sl_glarg(INT, data_len, input_size),
        sl_sharg(int, guard, 0));
    sl_sync();
  }

  p1 = get_cycles();  // start the timer

  // build the list of intervals for level 0... one interval: 0->N-1
  num_intervals[1] = 0;
  num_intervals[0] = 1;
  intervals[0][0].l = 0;
  intervals[0][0].r = input_size - 1;

  sl_family_t fam;

  // while (list for current level not empty)
  //   create list for next level and trigger the next thread to start
  int done = 0;
  int i = 0;
  for (i = 0; !done; i += level_stripe_size) {  // as at the time of this
                          // writing "break" isn't working, we execute the
                          // while by speculatively launching
                          // level_stripe_size threads at a time
    sl_create(fam,,i,i + level_stripe_size,1,,,
        partition_list_of_intervals,
        sl_glarg(INT*, data, input_vector),
        sl_glarg(SIZE, len, input_size),
        sl_glarg(INT*, result, scratch),
        sl_sharg(INT, level, i),
        sl_sharg(INT, done, 0));
    sl_sync();
    done = sl_geta(done);
  }

  p2 = get_cycles();  // end the timer
  if (slr_len(Pc))
    printf("%d\n", p2 - p1);

  if (slr_len(Pr)) {
    puts("Sorted data:\n");
    sl_create(,,,input_size,,,, print_list,
        sl_glarg(INT*, aadata, input_vector),
        sl_glarg(INT, data_len, input_size),
        sl_sharg(int, guard, 0));
    sl_sync();
  }
}
sl_enddef
