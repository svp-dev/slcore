#include "div.h"

unsigned long long __udivdi3(unsigned long long x, unsigned long long y)
{
    DIVCODEU(unsigned long long, x, y);
    return x;
}
