
sl_def(foo, void, sl_shparm(int, a)) 
{
  sl_setp(a, sl_getp(a));
}
sl_enddef

sl_def(t_main, void)
{
  sl_create(,,,,,,, foo, sl_sharg(int, x));
  sl_seta(x, 10);
  sl_sync();
}
sl_enddef
