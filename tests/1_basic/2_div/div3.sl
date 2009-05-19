m4_include(svp/div.slh)

sl_def(t_main, void)
{
  unsigned long xqu = 42, yqu = 5;
  divmodu(xqu, yqu);

  signed long xqs = 42, yqs = 5;
  divmods(xqs, yqs);
}
sl_enddef

