
// XFAIL: C (extra storage class specifier)
sl_decl(foo, void, sl_shparm(register int, a));

sl_def(bar, void, sl_shparm(register int, a)) { } sl_enddef



