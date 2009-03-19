
// XFAIL: C (global has no address)
int *p;
sl_def(foo, void, sl_glparm(int, a))
{
  p = &(sl_getp(a));
}
sl_enddef
