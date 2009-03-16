// m4_include(proto.m4)

ut_def(foo, void, ut_shparm(int, a)) { } ut_enddef

// XFAIL: C (invalid initializer)
ut_def(t_main, void)
{
  ut_create(,,,,,,, foo, ut_sharg(int, b, "hello"));
  ut_sync();
}
ut_enddef
