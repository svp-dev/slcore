
sl_def(foo, void)
{ }
sl_enddef

sl_decl((*foo1), void) = &foo;

sl_def(t_main, void)
{
  sl_decl((*foo2), void) = &foo;

  sl_create(,,,,,,, *foo1);
  sl_sync();

  sl_create(,,,,,,, *foo2);
  sl_sync();
}
sl_enddef

