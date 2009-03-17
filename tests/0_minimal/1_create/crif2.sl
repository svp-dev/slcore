sl_def(foo, void) {} sl_enddef

sl_def(t_main, void)
{
  if (1) {
    sl_create(,,1,10,2,3,, foo);
    sl_sync();
  }
}
sl_enddef
