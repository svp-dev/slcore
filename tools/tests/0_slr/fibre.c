#include <svp/fibre.h>
#include <svp/testoutput.h>

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
    output_int(x, 1);
    output_char('\n', 1);
  }
}
sl_enddef
