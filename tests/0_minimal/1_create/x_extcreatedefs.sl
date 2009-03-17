sl_decl(ext, void);

// XFAIL: L (symbol not defined)

sl_def(t_main, void)
{
  sl_create(,,,,,,, ext);
  sl_sync();
}
sl_enddef

