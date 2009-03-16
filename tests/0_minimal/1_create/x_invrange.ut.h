#include <libutc.h>

ut_def(foo, void) { } ut_enddef

ut_def(t_main, void)
{
  ut_create(,, test_index_params,, foo);
  ut_sync();
}
ut_enddef
