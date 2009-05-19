// XFAIL: *:A
sl_def(t_main, void)
{
  __asm__ volatile("invalid instruction;");
}
sl_enddef

