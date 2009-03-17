
sl_def(foo, void, sl_shparm(int, a), sl_glparm(int, b)) 
{ }
sl_enddef

sl_def(t_main, void)
{
  sl_create(,,,,,,, foo, sl_sharg(int, x), sl_glarg(int, y, 10));
  sl_seta(x, 20);
  sl_sync();
  int z = sl_geta(x) + sl_geta(y);
}
sl_enddef
