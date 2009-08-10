#include <fibre.h>

/*
 * SLT_RUN: 
 * SLT_RUN: -f TEST.d
 */

sl_def(t_main, void)
{
  if (fibre_tag(0) != -1) {
    int x = 0;
    x += *(int*)fibre_data(0);
    x += *(int*)fibre_data(1);
    x += *(int*)fibre_data(2);
    x += *(int*)fibre_data(3);
    int *a = (int*)fibre_data(4);
    *a = x;
  }
}
sl_enddef
