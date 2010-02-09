#include "mtstdio.h"
#include <cstring.h>
#include <cerrno.h>

void perror(const char* s)
{
    FILE *f = stderr;
    if (s && *s) {
        __writes(f, s);
        __writec(f, ':');
        __writec(f, ' ');
    }
    __writes(f, strerror(errno));
    __writec(f, '\n');
}
