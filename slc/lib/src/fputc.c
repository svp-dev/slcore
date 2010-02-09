#include "mtstdio.h"

int fputc(int c, FILE* f)
{
    __writec(f, c);
    return c;
}
