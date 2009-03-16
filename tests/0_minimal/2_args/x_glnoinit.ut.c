// m4_include(proto.m4)

ut_def(foo, void, ut_glparm(int, a)) { } ut_enddef

// XFAIL: C (global not initialized)
ut_def(t_main, void)
{
  ut_create(,,,,,,, foo, ut_glarg(int, b));
  ut_sync();
}
ut_enddef
