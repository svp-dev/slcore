#include "mtstdio.h"

int putchar(int c)
{
    __writec(stdout, c);
    return c;
}
