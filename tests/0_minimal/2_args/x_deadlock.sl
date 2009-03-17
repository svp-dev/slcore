
sl_def(foo, void, sl_shparm(int, a))
{
  sl_setp(a, sl_getp(a) + 1);
}
sl_enddef

// XTIMEOUT (shared never set: deadlock)
sl_def(t_main, void)
{
  sl_create(,,,,,,,
	    foo, sl_sharg(int, a));
  sl_sync();
}
sl_enddef
