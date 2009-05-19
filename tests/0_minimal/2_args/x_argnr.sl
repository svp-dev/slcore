sl_def(foo, void, sl_shparm(int, a)) { } sl_enddef

// XFAIL: *:C (incompatible number of args)
sl_def(t_main, void)
{
  sl_create(,,,,,,, foo);
  sl_sync();
}
sl_enddef
