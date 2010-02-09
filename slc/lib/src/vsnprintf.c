#include "mtstdio.h"
#include <cstdint.h>

int vsnprintf(char*restrict buf, size_t sz, const char*restrict fmt, va_list args)
{
    int ret;
    FILE fake;
    fake.kind = FK_STRING;
    fake.out.str.cur = fake.out.str.buf = buf;
    fake.out.str.end = buf + sz - 1;
    ret = vfprintf(&fake, fmt, args);
    *fake.out.str.cur = '\0';
}

int vsprintf(char*restrict str, const char*restrict fmt, va_list args)
{
    return vsnprintf(str, SIZE_MAX, fmt, args);
}



