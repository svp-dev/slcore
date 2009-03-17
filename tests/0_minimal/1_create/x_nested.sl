m4_include(svp/lib.slh)

sl_def(foo, void) {} sl_enddef

// XFAIL: T (nested creates)
sl_def(t_main, void)
{
  sl_family_t f1, f2;
  sl_create(f1,,,,,,, foo);
  sl_create(f2,,,,,,, foo);
  sl_sync(f2);
  sl_sync(f1);
}
sl_enddef
