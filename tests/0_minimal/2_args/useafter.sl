
sl_def(foo, void, sl_shparm(int, a)) 
{
  sl_setp(a, sl_getp(a));
}
sl_enddef

sl_def(t_main, void)
{
  sl_create(,,,,,,, foo, sl_sharg(int, x, 10));
  sl_sync();
  int z = sl_geta(x);
}
sl_enddef
