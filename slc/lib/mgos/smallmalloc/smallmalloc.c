#include <stdlib.h>
#include <string.h>

extern char __end;
extern char _RAM_END;
char *HEAP = &__end;
#define HEAP_SIZE (&_RAM_END - &__end)
size_t HEAP_ALLOCATED = 0;

void *small_malloc(size_t s) {
    if (s < 4) s = 4;
    s = (s + 3) & ~3; // ensure size is multiple of 4

    size_t cur_alloc = HEAP_ALLOCATED;
    if (cur_alloc >= HEAP_SIZE - s) {
	return 0;
    }

    void *ret = HEAP + cur_alloc;
    HEAP_ALLOCATED = cur_alloc + s;
    return ret;
}

void small_free(void* p) { }

void* small_calloc(size_t s, size_t c) {
    s *= c;
    void *p = malloc(s);
    if (p) {
	memset(p, 0, s);
    }
    return p;
}

void* small_realloc(void*p, size_t s) {
    void *n = malloc(s);
    if (p && n) {
	// <black magic ahead> This is a simple malloc! We may copy
	// more bytes here than necessary. But that's ok because the
	// source will be all lower in memory (see malloc above) so
	// there is no dangerous overlap.
	memcpy(n, p, s);
    }
    return n;
}
