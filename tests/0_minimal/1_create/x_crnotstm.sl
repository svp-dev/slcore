sl_def(foo, void) {} sl_enddef

// XFAIL: *:C (create not statement)
sl_def(t_main, void)
{
  sl_family_t f;
  if (1) 
    sl_create(f,,1,10,2,3,, foo);
    sl_sync(f);
}
sl_enddef
