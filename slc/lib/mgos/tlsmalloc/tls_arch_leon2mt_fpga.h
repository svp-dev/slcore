#ifndef TLS_ARCH_MTSPARC_FPGA_H
#define TLS_ARCH_MTSPARC_FPGA_H

/*** Architecture-specific services ***/

#include <stdlib.h>
#include <stdint.h>
#include <svp/compiler.h>

/* EXT_FREE(P): delegate to the external memory allocator */
#define EXT_FREE(P)  ((void)0) // FIXME: excl_dlfree
/* EXT_MALLOC(P): delegate to the external memory allocator */
#define EXT_MALLOC(N) ((void*)0) // FIXME: excl_dlmalloc

/* EXT_REALLOC(P): delegate to the external memory allocator */
#define EXT_REALLOC(P,N) ((void*)0) // FIXME: excl_dlrealloc

/* MAP_STORAGE(P, Sz): map storage, return != 0 if successful */
static forceinline
int MAP_STORAGE(void *p, size_t sz)
{
    return 1;
}
#define MAP_STORAGE_FAIL 0

#undef RELEASE_STORAGE

extern char __tls_base;
extern char __first_tls_top;

alwaysinline unused
void *TLS_BOTTOM(void) {
    unsigned tid;
    __asm__ ("t_get_tid %0" : "=r"(tid));
    return (void*)(&__tls_base + (1024 * tid));
}

alwaysinline unused
void *TLS_TOP(void) {
    unsigned tid;
    __asm__ ("t_get_tid %0" : "=r"(tid));
    return (void*)(&__first_tls_top + (1024 * tid));
}

/* TLS_BASE(): return base address of all TLS spaces */
alwaysinline unused
void *TLS_BASE(void)
{
    return &__tls_base;
}


/* IS_TLS_PTR(P): true if P is managed by this heap allocator */
alwaysinline unused
uintptr_t IS_TLS_PTR(void *ptr)
{
    // FIXME. this assumes 256KB TLS
    return ((uintptr_t)ptr >= (uintptr_t)TLS_BASE()) &&
	((uintptr_t)ptr < (uintptr_t)(&__tls_base + 256*1024));
}

/* TLS_NEXT(P): return address of space after P, 0 if no more space */
alwaysinline unused
void *TLS_NEXT(void *p)
{
    uintptr_t tls_size = 1024;
    char *next = ((char*)p) + tls_size;
    if (!IS_TLS_PTR(next))
        return 0;
    return next;
}


/* IS_TLS_LOCAL(P): true if P is in the current thread's TLS address space */
alwaysinline unused
bool IS_TLS_LOCAL(void *ptr)
{
    // FIXME: on fpga these are two separate address spaces
    uintptr_t uptr = (uintptr_t)ptr;
    uintptr_t bottom = (uintptr_t)TLS_BOTTOM();
    uintptr_t top = (uintptr_t)TLS_TOP();
    return ((uptr > bottom) && (uptr < (top/2 + bottom/2))); // (t+b)/2 will overflow!
}

/* CURRENT_CB(): address of the control block for the current thread */
alwaysinline unused
struct control_block* CURRENT_CB(void)
{
    return (struct control_block*)TLS_BOTTOM();
}

/* NR_SLOTS_IN_TLS(): maximum number of slots in the current thread's TLS */
alwaysinline unused
size_t NR_SLOTS_IN_TLS(void)
{
    uintptr_t bottom = (uintptr_t)TLS_BOTTOM();
    uintptr_t top = (uintptr_t)TLS_TOP();
    // FIXME: on fpga the full tls heap can be used
    uintptr_t med = top/2 + bottom/2;
    return (med - bottom)/SLOT_SIZE;
}


#endif
