m4_include(svp/div.slh)
m4_include(svp/iomacros.slh)

m4_define([[do_test]],[[{
    [[$1]] x = ([[$1]])([[$4]]), y = ([[$1]])([[$5]]);
    $2(x); putc(':'); $2(y); putc('=');
    $3(x, y);
    $2(x); putc(':'); $2(y); putc('\n');
  } 
]])

m4_define([[do_tests]],[[{
    puts("----\n");
    puts("64u: "); do_test(unsigned long long, [[putu]], [[divmodu]], [[$1]], [[$2]]);
    puts("64s: "); do_test(signed long long, [[putn]], [[divmods]], [[$1]], [[$2]]);
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


