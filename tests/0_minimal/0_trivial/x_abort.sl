m4_include(svp/lib.slh)

// XFAIL: R
sl_def(t_main, void)
{
  svp_abort();
}
sl_enddef
