// m4_include(proto.m4)
#include <libutc.h>

int e;

ut_def(foo, void) {} ut_enddef

ut_def(t_main, void)
{
    if (e == 0) {
        ut_create(f,,,,,,, foo);
        ut_sync(f);
    }
}
ut_enddef
