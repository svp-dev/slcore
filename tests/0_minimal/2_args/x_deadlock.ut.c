// m4_include(proto.m4)
#include <libutc.h>
#include <utcdebug.h>

ut_def(foo, void, ut_shparm(int, a))
{
  ut_setp(a, ut_getp(a) + 1);
}
ut_enddef

// XTIMEOUT: R (shared never set: deadlock)
ut_def(t_main, void)
{
  ut_create(,,,,,,,
	    foo, ut_sharg(int, a));
  ut_sync();
}
ut_enddef
