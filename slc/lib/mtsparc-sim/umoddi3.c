#include "div.h"

unsigned long long __umoddi3(unsigned long long x, unsigned long long y)
{
    DIVCODEU(unsigned long long, x, y);
    return y;
}
