#include <svp/slr.h>
#include <svp/testoutput.h>

slr_decl(slr_var(unsigned char, data, "array of 5120 bytes"));

// SLT_RUN: data=raw:TEST.d

sl_def(t_main, void)
{
  unsigned char *d = slr_get(data);
  size_t l = slr_len(data);
  int i;
  output_uint(l, 1);
  output_char('\n', 1);
  for (i = 0; i < l; ++i) {
    output_uint(d[i], 1);
    output_char('\n', 1);
  }
}
sl_enddef
