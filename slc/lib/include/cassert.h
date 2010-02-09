#ifndef SLC_CASSERT_H
#define SLC_CASSERT_H


#ifdef __mt_freestanding__

#include <svp/compiler.h>

#undef assert

#ifdef NDEBUG
#define assert(e)       ((void)0)
#else

// forward decl, we avoid including stdio/stdlib
void abort(void);
int  printf(const char * restrict, ...);

#define assert(e)  \
    ((void) (likely(e) ? 0 : __assert (#e, __FILE__, __LINE__)))
#define __assert(e, file, line) \
    ((void)printf ("%s:%u: failed assertion `%s'\n", file, line, e), abort())

#endif

#else

#ifdef __cplusplus
#include <cassert>
#else
#include <assert.h>
#endif

#endif

#endif
