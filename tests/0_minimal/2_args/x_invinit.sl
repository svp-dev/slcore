sl_def(foo, void, sl_glfparm(float, a)) { } sl_enddef

// XFAIL: *:C (invalid initializer)
sl_def(t_main, void)
{
  sl_create(,,,,,,, foo, sl_glfarg(float, b, "hello"));
  sl_sync();
}
sl_enddef
