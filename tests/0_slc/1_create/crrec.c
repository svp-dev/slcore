#include <svp/testoutput.h>

// XIGNORE: mt*_*n:R

sl_def(foo,void,sl_glparm(int, x))
{
    if (sl_getp(x) > 0)
    {
        sl_create(,,,,,,,foo,sl_glarg(int,,sl_getp(x)-1));
        sl_sync();
    }
    output_char('.', 1);
}
sl_enddef

sl_def(t_main,void)
{
    sl_create(,,,,,,,foo,sl_glarg(int,,200));
    sl_sync();
    output_char('\n', 1);
}
sl_enddef