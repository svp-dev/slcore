#include "mtstdio.h"

int puts(const char*s)
{
    FILE *f = stdout;
    __writes(f, s);
    __writec(f, '\n');
    return 0;
}

