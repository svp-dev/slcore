int e;

sl_def(foo, void) {} sl_enddef

sl_def(t_main, void)
{
  sl_create(,,,,,,, foo);
  sl_sync();
  if (e == 0) {
      e = e + 1;
  }
}
sl_enddef
