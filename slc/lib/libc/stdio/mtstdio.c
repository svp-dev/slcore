#include "mtstdio.h"

static FILE __std_fds[] = { 
    { FK_OUT, { 0 } }, 
    { FK_OUT, { 1 } }, 
    { FK_OUT, { 2 } } 
};
FILE *stddebug = &__std_fds[0];
FILE *stdout = &__std_fds[1];
FILE *stderr = &__std_fds[2];

