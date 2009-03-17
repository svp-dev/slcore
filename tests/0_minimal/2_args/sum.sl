// This is a tiny test, stolen
// from the core compiler test suite.

sl_def(sum_int, void, sl_shparm(int, sum))
{
  sl_setp(sum, sl_getp(sum) + 1);
}
sl_enddef

sl_def(t_main, void)
{
  sl_create(,,0,9,1,,, sum_int, sl_sharg(int, s_in, 0));
  sl_sync();
}
sl_enddef


 
  
