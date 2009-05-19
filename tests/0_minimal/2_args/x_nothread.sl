
void foo(int a, int b);

// XFAIL: *:C (not thread type)
sl_def(t_main, void)
{
  sl_create(f,,1,10,1,1,, foo, sl_glarg(int, a), sl_glarg(int, b));
  sl_sync(f);
}
sl_enddef
