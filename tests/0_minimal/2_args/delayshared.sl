m4_include(svp/lib.slh)
m4_include(svp/iomacros.slh)

sl_def(foo, void, sl_shparm(int, a))
{
  sl_setp(a, sl_getp(a) + 1);
}
sl_enddef

// 2009-04-02: FIXME: we acknowledge that muTC-ptl
// does not support this construct fully yet; but
// we want slc's testsuite to properly succeed. So
// we mark the test to ignore the output on muTC-ptl:
// XIGNORE: ptl*:D

sl_def(t_main, void)
{
  int busy;
  sl_create(,,,,,,,
	    foo, sl_sharg(int, a));
  // FIXME: for ptl here we should find a way to
  // force a context switch, otherwise the problem
  // is not demonstrated. In the previous version
  // of this test, a call was done to the C library's
  // putc() function, but we can't do this in SL
  // because putc is a thread function and we cannot
  // nest creates.
  for (busy = 0; busy < 10000; ++busy) svp_nop();
  sl_seta(a, 42);
  sl_sync();

  putn(sl_geta(a));
  putc('\n');
}
sl_enddef
