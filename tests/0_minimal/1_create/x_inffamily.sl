m4_include(svp/lib.slh)

sl_def(foo, void) 
{
  svp_nop();
}
sl_enddef

// XTIMEOUT: *
sl_def(t_main, void)
{
  sl_create(,,0,1,0,,, foo);
  sl_sync();
}
sl_enddef
