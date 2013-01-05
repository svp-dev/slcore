#include "div.h"

long long __moddi3(long long x, long long y)
{
    DIVCODES(long long, x, y);
    return y;
}
