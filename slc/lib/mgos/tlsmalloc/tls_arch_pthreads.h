#ifndef TLS_ARCH_PTHREADS_H
#define TLS_ARCH_PTHREADS_H

#include <stdlib.h>
#include <sys/mman.h>

/* EXT_FREE(P): delegate to the external memory allocator */
#define EXT_FREE free
/* EXT_MALLOC(P): delegate to the external memory allocator */
#define EXT_MALLOC malloc
/* EXT_REALLOC(P): delegate to the external memory allocator */
#define EXT_REALLOC realloc

/* MAP_STORAGE(P, Sz): map storage, return != 0 if successful */
#define MAP_STORAGE(P, Sz)  mprotect((void*)(P), Sz, PROT_READ|PROT_WRITE)
#define MAP_STORAGE_FAIL (-1)

#define RELEASE_STORAGE 1
#define UNMAP_STORAGE(P, Sz) mprotect((void*)(P), Sz, PROT_NONE)

extern THREADSPEC char* __th_bottom;

static forceinline
void *TLS_BOTTOM(void) {
    return __th_bottom;
}

extern THREADSPEC char* __th_top;

static forceinline
void *TLS_TOP(void) {
    return __th_top;
}

extern char* __alltls_base;
extern size_t __alltls_total_size;
extern size_t __tls_size;

/* TLS_BASE(): return base address of all TLS spaces */
static forceinline
void *TLS_BASE(void)
{
    return __alltls_base;
}

/* TLS_NEXT(P): return address of space after P, 0 if no more space */
static forceinline
void *TLS_NEXT(void *p)
{
    char *next = ((char*)p) + __tls_size;
    if (!(((uintptr_t)next >= (uintptr_t)__alltls_base) &&
          ((uintptr_t)next < ((uintptr_t)__alltls_base + __alltls_total_size))))
        return 0;
    return next;
}

/* IS_TLS_PTR(P): true if P is managed by this heap allocator */
static forceinline
uintptr_t IS_TLS_PTR(void *ptr)
{
    uintptr_t p = (uintptr_t)ptr;
    uintptr_t base = (uintptr_t)__alltls_base;
    return (p >= base) && (p < (base + __alltls_total_size));
}

/* IS_TLS_LOCAL(P): true if P is in the current thread's TLS address space */
static forceinline
bool IS_TLS_LOCAL(void *ptr)
{
    uintptr_t uptr = (uintptr_t)ptr;
    uintptr_t bottom = (uintptr_t)TLS_BOTTOM();
    uintptr_t top = (uintptr_t)TLS_TOP();
    return ((uptr >= bottom) && (uptr < top)); 
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
    return (top - bottom)/SLOT_SIZE;
}


#endif
