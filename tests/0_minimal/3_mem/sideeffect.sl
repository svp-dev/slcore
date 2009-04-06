m4_include(svp/iomacros.slh)

int blah;

sl_def(foo, void)
{
  blah = 10;
}
sl_enddef

sl_def(t_main, void)
{
  blah = 42;
  sl_proccall(foo);
  putn(blah); putc('\n');
}
sl_enddef
