

sl_def(foo, void, sl_glparm(int, a)) 
{ }
sl_enddef

sl_def(t_main, void)
{
  sl_create(,,,,,,, foo, sl_glarg(int, x, 10));
  sl_sync();
  int z = sl_geta(x);
}
sl_enddef
