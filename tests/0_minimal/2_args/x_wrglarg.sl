
sl_def(foo, void, sl_glparm(int, a))
{ }
sl_enddef

// XFAIL: C (global is immutable)
sl_def(t_main, void)
{
  sl_create(,,,,,,, foo, sl_glarg(int, a, 10));
  sl_seta(a, 10);
  sl_sync();
}
sl_enddef 
  
