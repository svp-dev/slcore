m4_include(svp/div.slh)
m4_include(svp/iomacros.slh)

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

m4_define([[do_tests]],[[{
    puts("----\n");
    sl_proccall(testu, sl_glarg(unsigned long long, x, [[$1]]),
		sl_glarg(unsigned long long, y, [[$2]]));
    sl_proccall(tests, sl_glarg(signed long long, x, [[$1]]),
		sl_glarg(signed long long, y, [[$2]]));
  } 
]])

sl_def(t_main, void)
{
  do_tests(42, 5); 
  do_tests(0, 1);
  do_tests(0, -1);
  do_tests(5, 42);
  do_tests(301, -10);
  do_tests(-301, -10);
  do_tests(-301, 10);
  do_tests(301, 10);
  do_tests(0xffffffffUL, 0x7ffffffeUL);
  do_tests(0xffffffffffffffffULL, 0x7ffffffffffffffeULL);
}
sl_enddef


