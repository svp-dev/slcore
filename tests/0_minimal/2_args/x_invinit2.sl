
sl_def(foo, void, sl_shfparm(float, a)) { } sl_enddef

// XFAIL: C (invalid initializer)
sl_def(t_main, void)
{
  sl_create(,,,,,,, foo, sl_shfarg(float, b, "hello"));
  sl_sync();
}
sl_enddef
