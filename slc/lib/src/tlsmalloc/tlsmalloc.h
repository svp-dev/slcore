#ifndef TLSMALLOC_H
#define TLSMALLOC_H

#include <stddef.h>

void *tls_realloc(void*, size_t);
void *tls_malloc(size_t);
void *tls_calloc(size_t);
void tls_free(void*);

void tls_local_mallinfo(void*);
void tls_mallinfo(void);

#endif
