
// XFAIL: T (sync without create should fail during transform)

sl_def(t_main, void)
{
  sl_sync();
}
sl_enddef
