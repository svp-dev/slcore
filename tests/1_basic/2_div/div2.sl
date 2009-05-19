m4_include(svp/iomacros.slh)
m4_include(svp/assert.slh)
m4_include(svp/div.slh)

sl_def(t_main, void)
{
  unsigned long x = 69, y = 5;
  divmodu(x, y);
  unsigned long x1 = x, y1 = y;
  while (x1--) putc('.'); putc('\n');
  while (y1--) putc('.'); putc('\n');
  svp_assert(x == 4);
  svp_assert(y == 13);
}
sl_enddef

