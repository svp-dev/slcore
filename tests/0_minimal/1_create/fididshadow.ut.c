// m4_include(proto.m4)
#include <libutc.h>

ut_def(foo, void) {} ut_enddef

struct { int a; } f;
ut_def(t_main, void)
{
  ut_create(f,,,,,,,foo); // valid: different scope
  ut_sync(f);
  f.a = 10; // valid: f visible again
}
ut_enddef
