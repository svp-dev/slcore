int result[10];

sl_def(t_main, void)
{
  int i;
  i = 1;
  if (i) 
    result[0] = 2;
  else
    result[0] = 3;
}
sl_enddef

