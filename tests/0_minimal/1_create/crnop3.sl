m4_include(svp/lib.slh)

sl_def(nopfunc, void)
{
  svp_nop();
}
sl_enddef

sl_def(foo, void)
{
  sl_create(,,0,9,1,1,, nopfunc);
  sl_sync();
}
sl_enddef

sl_def(t_main, void)
{
  sl_create(,,0,9,1,1,, foo);
  sl_sync();
}
sl_enddef


 
  
