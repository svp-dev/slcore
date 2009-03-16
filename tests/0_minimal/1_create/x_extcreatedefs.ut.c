// m4_include(proto.m4)
#include <libutc.h>

ut_decl(ext, void);

// XFAIL: L (symbol not defined)

ut_def(t_main, void)
{
  ut_create(,,,,,,, ext);
  ut_sync();
}
ut_enddef

