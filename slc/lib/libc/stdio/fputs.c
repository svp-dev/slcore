#include "mtstdio.h"

int fputs(const char*restrict s, FILE*restrict f)
{
    /* fputs does not terminate with a newline,
       contrary to puts which does. */
    __writes(f, s);
    return 0;
}

