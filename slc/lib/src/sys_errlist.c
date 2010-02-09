#include <cstring.h>

const char * const sys_errlist[] = {
    /* 0 */       "Unknown error: 0",
    /* ENOMEM */  "Cannot allocate memory",
    /* EINVAL */  "Invalid argument",
    /* EFAULT */  "Bad address",
    /* EDOM */    "Numerical argument out of domain",
    /* ERANGE */  "Result too large",
    /* EILSEQ */  "Illegal byte sequence",
    0
};
const int sys_nerr = sizeof(sys_errlist) / sizeof(sys_errlist[0]);

