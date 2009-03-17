sl_decl(ext, void);

// XFAIL: L (symbol not defined)

sl_def(t_main, void)
{
  sl_create(,,0,10,1,0,, ext);
  sl_sync();
}
sl_enddef

