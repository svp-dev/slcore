m4_include(slr.slh)
m4_include(svp/iomacros.slh)

slr_decl(slr_var(int, a), slr_var(int, b));

// SLT_RUN:  a=10  b=-10

sl_def(t_main, void)
{
  int a = slr_get(a)[0];
  int b = slr_get(b)[0];
  printf("%d + %d = %d\n", a, b, a+b);
}
sl_enddef
