#include "mtstdio.h"

int fputs(const char*restrict s, FILE*restrict f)
{
    __writes(f, s);
    __writec(f, '\n');
    return 0;
}

