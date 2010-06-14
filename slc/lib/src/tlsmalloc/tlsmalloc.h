#ifndef TLSMALLOC_H
#define TLSMALLOC_H

#include <stddef.h>

void *tls_malloc(size_t);
void tls_free(void*);

#endif
