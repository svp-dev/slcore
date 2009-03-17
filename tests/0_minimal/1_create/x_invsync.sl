struct { int a[100]; } bogus;

sl_def(foo, void) {} sl_enddef

// XFAIL: C (invalid type for exit code)
sl_def(t_main, void)
{
  sl_create(,,,,,,, foo);
  sl_sync(bogus);
}
sl_enddef


 
