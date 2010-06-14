#ifndef TLSMALLOC_H
#define TLSMALLOC_H

#include <stddef.h>

void *tls_realloc(void*, size_t);
void *tls_malloc(size_t);
void *tls_calloc(size_t, size_t);
void tls_free(void*);

void tls_local_stats(void*);
void tls_malloc_stats(void);

#endif
