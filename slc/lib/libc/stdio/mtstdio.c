#include "mtstdio.h"

size_t dbgbuf_pos;
char dbgbuf_buf[STDIO_OUT_BUFFER_SIZE];

static FILE __std_fds[] = { 
    { FK_OUT, { 1 } },
    { FK_OUT, { 1 } }, 
    { FK_OUT, { 2 } },
    { FK_UART, { 0 } },
    { FK_BUFFER, { .buf = { 1, (char *)dbgbuf_buf, &dbgbuf_pos } } },
    { FK_BUFFER, { .buf = { 2, (char *)dbgbuf_buf, &dbgbuf_pos } } }
};
FILE *stddebug = &__std_fds[0];
FILE *stdout = &__std_fds[1];
FILE *stderr = &__std_fds[2];
FILE *stduart = &__std_fds[3];
FILE *dbgstdout = &__std_fds[4];
FILE *dbgstderr = &__std_fds[5];
