#include <svp/testoutput.h>
#include <string.h>
#include "mtinit.h"
#include "malloc/mtmalloc.h"
#include "mgsim/mtconf.h"

void sys_environ_init(char *initenv)
{
    extern char** environ;

    /* 
       the environment is provided as concatenated C strings separated
       by an ASCII NUL (0). The last string is followed by a
       a couple of standalone NUL bytes.
    */

    // (using dlmalloc directly since we are still single-threaded at
    // this point, no exclusion required)

    /* first: count, and detect verbosity. */
    char *p = initenv;
    size_t nvars = 0;
    if (p)
        while (p[0] != 0)
        {
            ++nvars;
            size_t sz = strlen(p);
            if (strncmp("MGSYS_QUIET=", p, (sz > 12) ? 12 : sz) == 0)
                verbose_boot = 0;
            p += sz + 1;
        }

    size_t envdatasize = p - initenv;

    /* then: duplicate the data, since it may be in ROM */
    if (verbose_boot)
    {
        output_string("* input data: env at 0x", 2);
        output_hex(initenv, 2);
        output_string(", ", 2);
        output_uint(envdatasize, 2);
        output_string(" bytes.", 2);
        output_ts(2);
        output_char('\n', 2);
    }

    char *area = dlmalloc(envdatasize);
    memcpy(area, initenv, envdatasize);

    if (verbose_boot)
    {
        output_string("* strings at 0x", 2);
        output_hex(area, 2);
    }

    /* then: make array of pointers to each variable */
    environ = dlmalloc((nvars + 1) * sizeof(char*));

    size_t i;
    for (i = 0, p = area; i < nvars; ++i, p += strlen(p) + 1)
    {
        environ[i] = p;
    }
    environ[i] = 0;

    if (verbose_boot)
    {
        output_string(", environ[", 2);
        output_uint(nvars, 2);
        output_string("] at 0x", 2);
        output_hex(environ, 2);
        output_string(".", 2);
        output_ts(2);
        output_char('\n', 2);
    }
}
