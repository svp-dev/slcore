
// XFAIL: C (shared has no address)
int *p;
sl_def(foo, void, sl_shparm(int, a))
{
  p = &(sl_getp(a));
  sl_setp(a, sl_getp(a));
}
sl_enddef
