#include <svp/testoutput.h>
#include <svp/compiler.h>
#include <stddef.h>

noinline
size_t bar(const char* s)
{
    size_t l;
    for (l = 0; *s; ++l, ++s) ;
    return l;
}

sl_def(foo, void, sl_shparm(int, token))
{
    int token = sl_getp(token);
    (void)bar("");
    sl_index(i);
    output_int(i, 1); output_char('\n', 1);
    sl_setp(token, token);
}
sl_enddef

sl_def(t_main, void)
{
    sl_create(,,0,4,,,, foo,
        sl_sharg(int, token, 0)
    );
    sl_sync();
}
sl_enddef
