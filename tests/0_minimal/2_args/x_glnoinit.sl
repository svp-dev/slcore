
sl_def(foo, void, sl_glparm(int, a)) { } sl_enddef

// XFAIL: *:C (global not initialized)
sl_def(t_main, void)
{
  sl_create(,,,,,,, foo, sl_glarg(int, b));
  sl_sync();
}
sl_enddef
