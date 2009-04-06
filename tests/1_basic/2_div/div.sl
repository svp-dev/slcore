m4_include(svp/div.slh)
m4_include(svp/iomacros.slh)
m4_include(slr.slh)

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

slr_decl(slr_var(ua, unsigned long long),
         slr_var(ub, unsigned long long),
         slr_var(sa, signed long long),
         slr_var(sb, signed long long));

// SLT_RUN: -dua=42 -dsa=42 -dub=5 -dsb=5
// SLT_RUN: -dua=5 -dsa=5 -dub=42 -dsb=42

// SLT_RUN: -dua=0 -dsa=0 -dub=1 -dsb=1
// SLT_RUN: -dua=0 -dsa=0 -dub=-1 -dsb=-1

// SLT_RUN: -dua=301 -dsa=301 -dub=-10 -dsb=-10
// SLT_RUN: -dua=-301 -dsa=-301 -dub=-10 -dsb=-10
// SLT_RUN: -dua=-301 -dsa=-301 -dub=10 -dsb=10
// SLT_RUN: -dua=301 -dsa=301 -dub=10 -dsb=10

// SLT_RUN: -dua=0xffffffffUL -dsa=0xffffffffUL -dub=0x7ffffffeUL -dsb=0x7ffffffeUL
// SLT_RUN: -dua=0xffffffffffffffffULL -dsa=0xffffffffffffffffULL -dub=0x7ffffffffffffffeULL -dsb=0x7ffffffffffffffeULL

sl_def(t_main, void)
{
  puts("----\n");
  sl_proccall(testu, 
	      sl_glarg(unsigned long long, x, slr_get(ua)[0]),
	      sl_glarg(unsigned long long, y, slr_get(ub)[0]));
  sl_proccall(tests, 
	      sl_glarg(signed long long, x, slr_get(sa)[0]),
	      sl_glarg(signed long long, y, slr_get(sb)[0]));
}
sl_enddef


