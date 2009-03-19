sl_def(foo, void) {} sl_enddef

sl_def(t_main, void)
{
  sl_family_t f;

  sl_create(f,,1,10,2,3,, foo);
  sl_sync();

  sl_create(f,,1,10,2,3,, foo);
  sl_sync();

  sl_create(f,,1,10,2,3,, foo);
  sl_kill(f); 
  sl_sync();

}
sl_enddef
