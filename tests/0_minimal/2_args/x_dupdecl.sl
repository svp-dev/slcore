
sl_def(foo, void, sl_glparm(int, x)) {} sl_enddef

// XFAIL: C (multiple declarations of x)
sl_def(t_main, void)
{
  sl_create(,,,,,,, foo, sl_glarg(int, x, 10));
  sl_sync();

  sl_create(,,,,,,, foo, sl_glarg(int, x, 10));
  sl_sync();
}
sl_enddef
