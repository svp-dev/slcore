int bla;

sl_def(a, void, sl_shparm(int, x), sl_shparm(int, v))
{
  bla = sl_getp(x);
  sl_setp(v, sl_getp(v) + bla);
}
sl_enddef

sl_def(t_main, void)
{
  sl_family_t fid;
  sl_create(fid,,,,,,, a, sl_sharg(int, x, 0), sl_sharg(int, v, 0));
  sl_sync();
}
sl_enddef
