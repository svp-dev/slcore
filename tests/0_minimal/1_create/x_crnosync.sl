m4_include(svp/iomacros.slh)

sl_def(func, void)
{
  puts("hello world\n");
}
sl_enddef

// XFAIL: *:T (create without sync should fail during transform)

sl_def(t_main, void)
{
  sl_family_t f;
  sl_create(f,,0,1,1,,, func);
}
sl_enddef
