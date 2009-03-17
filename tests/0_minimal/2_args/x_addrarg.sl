sl_def(foo, void, sl_shparm(int, x)) {} sl_enddef

// XFAIL: C (shared has no address)
sl_def(t_main, void)
{
  int *a;
  sl_create(f1,,,,,,,foo, sl_sharg(int, b, 0));
  a = &(sl_geta(b));
  sl_sync(f1);
}
sl_enddef
