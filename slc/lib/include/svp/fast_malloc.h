#ifndef SVP_FAST_MALLOC_H
#define SVP_FAST_MALLOC_H

#include <stdlib.h>

#ifndef fast_malloc
#define fast_malloc malloc
#endif

#ifndef fast_free
#define fast_free free
#endif

#ifndef fast_realloc
#define fast_realloc realloc
#endif

#ifndef fast_calloc
#define fast_calloc calloc
#endif

#endif
