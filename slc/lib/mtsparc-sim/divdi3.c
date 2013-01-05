#include "div.h"

long long __divdi3(long long x, long long y)
{
    DIVCODES(long long, x, y);
    return x;
}
