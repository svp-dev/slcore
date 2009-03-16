m4_include(svp/iomacros.slh)
m4_include(svp/roman.slh)

sl_def(foo, void, sl_shparm(int, a)) 
{
   sl_setp(a, sl_getp(a) + 1);

   putc('.');
} 
sl_enddef

sl_def(t_main, void) 
{
  sl_create(,, 0, 10, 1, 0,,
            foo, sl_sharg(int, x));
  sl_seta(x, 0);
  sl_sync();

  int ax = sl_geta(x);

  putc('\n');
  putn(ax);
  putc('\n');
} 
sl_enddef
