// XFAIL: L
extern int __do_not_exist;

sl_def(t_main, void)
{
  __do_not_exist = 1;
}
sl_enddef

