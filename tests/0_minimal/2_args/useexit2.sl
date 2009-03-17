m4_include(svp/lib.slh)

sl_def(foo, void) { } sl_enddef

sl_def(t_main, void)
{
  int r;
  sl_family_t f;
  sl_create(f,,,,,, void, foo);
  sl_kill(f);
  sl_sync(r);
  if (r == SVP_EKILLED) 
    svp_nop();
}
sl_enddef
