struct A {} a;

// XFAIL: C (not thread type)
sl_def(t_main, void)
{
  sl_family_t f;
  sl_create(f,,1,10,1,1,, a);
  sl_sync(f);
}
sl_enddef
