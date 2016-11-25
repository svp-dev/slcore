#include <svp/testoutput.h>

# define MAX_REC 5

int e;

sl_def(foo,void,sl_glparm(int, x))
{
    if (sl_getp(x) > 0)
    {
        sl_spawndecl(f);
        sl_spawn(f,,,,,,,foo,sl_glarg(int,,sl_getp(x)-1));
        sl_spawnsync(f);
    } else {
	e = 1;
    }
}
sl_enddef

int main(void)
{
    sl_create(,,,,,,,foo,sl_glarg(int,,5));
    sl_sync();
    return e - 1;
}
