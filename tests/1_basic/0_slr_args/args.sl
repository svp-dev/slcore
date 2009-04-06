m4_include(slr.slh)
m4_include(svp/iomacros.slh)

slr_decl(slr_var(a, int), slr_var(b, int));

// SLT_RUN: -da=10 -db=-10

sl_def(t_main, void)
{
  long long a = slr_get(a)[0];
  long long b = slr_get(b)[0];
  printf("%d + %d = %d\n", a, b, a+b);
}
sl_enddef
