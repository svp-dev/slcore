
// XFAIL: *:[TPC] (global is immutable)
sl_def(foo, void, sl_glparm(int, a))
{
  sl_setp(a, sl_getp(a) + 1);
}
sl_enddef

 
  
