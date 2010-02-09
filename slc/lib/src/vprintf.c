#include <cstdio.h>

int vprintf(const char* restrict fmt, va_list args)
{
    return vfprintf(stdout, fmt, args);
}
