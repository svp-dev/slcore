m4_include(svp/lib.slh)

sl_def(nopfunc, void)
{
  svp_nop();
}
sl_enddef

sl_def(t_main, void)
{
  sl_create(,,,,,,, nopfunc);
  sl_sync();
}
sl_enddef


 
  
