#include <stdio.h>
#include <errno.h>

void perror(const char*s)
{
    FILE *f = stderr;
    if (s && s[0])
        fprintf(f, "%s: ", s);
    if (errno >= sys_nerr)
        fprintf(f, "Unknown error: %d\n", errno);
    else
        fprintf(f, "%s\n", sys_errlist[errno]);
}


