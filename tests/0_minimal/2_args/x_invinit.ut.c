// m4_include(proto.m4)

ut_def(foo, void, ut_glparm(int, a)) { } ut_enddef

// XFAIL: C (invalid initializer)
ut_def(t_main, void)
{
  ut_create(,,,,,,, foo, ut_glarg(int, b, "hello"));
  ut_sync();
}
ut_enddef
