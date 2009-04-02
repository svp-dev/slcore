m4_include(svp/div.slh)
m4_include(svp/iomacros.slh)
m4_include(sgr.slh)

sl_def(testu, void, 
       sl_glparm(unsigned long long, x),
       sl_glparm(unsigned long long, y))
{
  unsigned long long lx = sl_getp(x), ly = sl_getp(y);
  printf("unsigned: %u : %u = ", lx, ly);
  divmodu(lx, ly);
  printf("%u : %u\n", lx, ly);
}
sl_enddef

sl_def(tests, void, 
       sl_glparm(signed long long, x),
       sl_glparm(signed long long, y))
{
  signed long long lx = sl_getp(x), ly = sl_getp(y);
  printf("signed: %d : %d = ", lx, ly);
  divmods(lx, ly);
  printf("%d : %d\n", lx, ly);
}
sl_enddef

sgr_decl(sgr_var(ua, unsigned long long),
         sgr_var(ub, unsigned long long),
         sgr_var(sa, signed long long),
         sgr_var(sb, signed long long));

// SLT_RUN: ua=42 sa=42 ub=5 sb=5
// SLT_RUN: ua=5 sa=5 ub=42 sb=42

// SLT_RUN: ua=0 sa=0 ub=1 sb=1
// SLT_RUN: ua=0 sa=0 ub=-1 sb=-1

// SLT_RUN: ua=301 sa=301 ub=-10 sb=-10
// SLT_RUN: ua=-301 sa=-301 ub=-10 sb=-10
// SLT_RUN: ua=-301 sa=-301 ub=10 sb=10
// SLT_RUN: ua=301 sa=301 ub=10 sb=10

// SLT_RUN: ua=0xffffffffUL sa=0xffffffffUL ub=0x7ffffffeUL sb=0x7ffffffeUL
// SLT_RUN: ua=0xffffffffffffffffULL sa=0xffffffffffffffffULL ub=0x7ffffffffffffffeULL sb=0x7ffffffffffffffeULL

sl_def(t_main, void)
{
  puts("----\n");
  sl_proccall(testu, 
	      sl_glarg(unsigned long long, x, sgr_get(ua)[0]),
	      sl_glarg(unsigned long long, y, sgr_get(ub)[0]));
  sl_proccall(tests, 
	      sl_glarg(signed long long, x, sgr_get(sa)[0]),
	      sl_glarg(signed long long, y, sgr_get(sb)[0]));
}
sl_enddef


