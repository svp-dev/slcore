#include "mtstdio.h"
#include <svp/divide.h>

size_t fwrite(const void*restrict b, size_t sz, size_t nitems, FILE*restrict f)
{
    size_t e = __write(f, b, sz * nitems);
    return __divu_size_t(e, sz);
}
