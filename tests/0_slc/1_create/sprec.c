#include <svp/testoutput.h>

// XIGNORE: mt*_*n:R

// spawn not supported on MT-SPARC yet
// XIGNORE: mts*:*

sl_def(foo,void,sl_glparm(int, x))
{
    if (sl_getp(x) > 0)
    {
        sl_spawndecl(f);
        sl_spawn(f,,,,,,,foo,sl_glarg(int,,sl_getp(x)-1));
        sl_spawnsync(f);
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
