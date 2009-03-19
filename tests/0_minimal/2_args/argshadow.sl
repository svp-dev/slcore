
sl_def(foo, void, sl_glparm(int, a)) {} sl_enddef

struct blah { int x; } a;
sl_def(t_main, void)
{
  if (1) {
    sl_family_t f;
    sl_create(f,,,,,,,foo, sl_glarg(int, a, 10)); // valid: different scope
    sl_sync();
    int z = sl_geta(a); // a still visible
  }
  a.x = 10; // previous a now visible
}
sl_enddef
