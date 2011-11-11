#include "mtstdio.h"

int putc(int c, FILE* f)
{
    __writec(f, c);
    return c;
}
