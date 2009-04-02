m4_include(sgr.slh)
m4_include(svp/iomacros.slh)

sgr_decl(sgr_var(a, int), sgr_var(b, int));

// SLT_RUN: a=10 b=-10

sl_def(t_main, void)
{
  long long a = sgr_get(a)[0];
  long long b = sgr_get(b)[0];
  printf("%d + %d = %d\n", a, b, a+b);
}
sl_enddef
