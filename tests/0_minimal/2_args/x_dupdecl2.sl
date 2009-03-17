
sl_def(foo, void, sl_shparm(int, y), sl_glparm(int, x)) {} sl_enddef

// XFAIL: C (multiple declarations of x)
sl_def(t_main, void)
{
  sl_create(,,,,,,, foo, sl_sharg(int, x, 10), sl_glarg(int, x, 20));
  sl_sync();
}
sl_enddef
