#ifndef TLS_ARCH_PTHREADS_H
#define TLS_ARCH_PTHREADS_H

#include <stdlib.h>

/* EXT_FREE(P): delegate to the external memory allocator */
#define EXT_FREE free
/* EXT_MALLOC(P): delegate to the external memory allocator */
#define EXT_MALLOC malloc

/* MAP_STORAGE(P, Sz): map storage, return != 0 if successful */
#define MAP_STORAGE(P, Sz)  1  /* always succeed here */

extern __thread void* __th_bottom;

static forceinline
void *TLS_BOTTOM(void) {
    return __th_bottom;
}

extern __thread void* __th_top;

static forceinline
void *TLS_TOP(void) {
    return __th_top;
}

/* IS_TLS_PTR(P): true if P is managed by this heap allocator */
static forceinline
uintptr_t IS_TLS_PTR(void *ptr)
{
    // FIXME: figure something out here.
    return 1;
}

/* IS_TLS_LOCAL(P): true if P is in the current thread's TLS address space */
static forceinline
bool IS_TLS_LOCAL(void *ptr)
{
    uintptr_t uptr = (uintptr_t)ptr;
    uintptr_t bottom = (uintptr_t)TLS_BOTTOM();
    uintptr_t top = (uintptr_t)TLS_TOP();
    return ((uptr > bottom) && (uptr < (top/2 + bottom/2))); // (t+b)/2 will overflow!
}

/* CURRENT_CB(): address of the control block for the current thread */
static forceinline
struct control_block* CURRENT_CB(void)
{
    return (struct control_block*)TLS_BOTTOM();
}

/* NR_SLOTS_IN_TLS(): maximum number of slots in the current thread's TLS */
static forceinline
size_t NR_SLOTS_IN_TLS(void)
{
    uintptr_t bottom = (uintptr_t)TLS_BOTTOM();
    uintptr_t top = (uintptr_t)TLS_TOP();
    uintptr_t med = top/2 + bottom/2;
    return (med - bottom)/SLOT_SIZE;
}


#endif
