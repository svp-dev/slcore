
m4_include(svp/assert.slh)

sl_def(foo, void, sl_shparm(int, a)) 
{
  sl_setp(a, sl_getp(a) + 1);
}
sl_enddef

sl_def(t_main, void)
{
  sl_create(,,,,,,, foo, sl_sharg(int, a));
  sl_seta(a, 10);
  sl_sync();
  svp_assert(sl_geta(a) == 11);
  {
    sl_create(,,,,,,, foo, sl_sharg(int, a));
    sl_seta(a, 20);
    sl_sync();
    svp_assert(sl_geta(a) == 21);
  }
  svp_assert(sl_geta(a) == 11);   
}
sl_enddef

