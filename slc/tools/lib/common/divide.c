#define DIVIDE_IMPL
#include <svp/divide.h>

int __divsi3(int x, int y)
{
    return __divs_int32_t(x, y);
}

int __modsi3(int x, int y)
{
    return __mods_int32_t(x, y);
}

unsigned int __udivsi3(unsigned int x, unsigned int y)
{
    return __divu_uint32_t(x, y);
}

unsigned int __umodsi3(unsigned int x, unsigned int y)
{
    return __modu_uint32_t(x, y);
}
