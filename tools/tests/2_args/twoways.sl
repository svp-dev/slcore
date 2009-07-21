


sl_def(foo, void, sl_shparm(int, x))
{ 
  sl_setp(x, sl_getp(x));
}
sl_enddef


sl_def(bar, void, sl_shparm(int, y))
{
  sl_create(,,,,,,,foo, sl_sharg(int, sx));
  
  sl_seta(sx, sl_getp(y));

  sl_sync();

  sl_setp(y, sl_geta(sx));
}
sl_enddef

sl_def(t_main, void)
{
  sl_create(,,,,,,, bar, sl_sharg(int, x));
  sl_seta(x, 0);
  sl_sync();
}
sl_enddef
