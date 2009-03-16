// m4_include(proto.m4)
#include <libutc.h>

// XFAIL: C (thread has no break)
ut_def(foo, int)
{
	//no break here so fail
}
ut_enddef
