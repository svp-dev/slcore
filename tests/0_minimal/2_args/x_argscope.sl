
sl_def(foo, void, sl_glparm(int, a)) {} sl_enddef

// XFAIL: *:C (use of arg out of scope)
sl_def(t_main, void)
{
  {
    sl_create(f,,,,,,, foo, sl_glarg(int, a, 10));
    sl_sync(f);
  }
  int z = sl_geta(a);
}
sl_enddef
