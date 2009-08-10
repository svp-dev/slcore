sl_def(foo, void)
{
  while(1)
    sl_end_thread;
}
sl_enddef

sl_def(t_main, void)
{
  sl_proccall(foo);
}
sl_enddef
