#ifndef TLS_ARCH_MTSPARC_FPGA_H
#define TLS_ARCH_MTSPARC_FPGA_H

/*** Architecture-specific services ***/

#include <stdlib.h>
#include <stdint.h>
#include <svp/mgsim.h>
#include <svp/mtmalloc.h>

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


static forceinline
void *TLS_BOTTOM(void) {
    void *ret;
    // FIXME: get tls bottom address here
    asm("unimp 10 ! ldbp %0" : "=r"(ret));
    return ret;
}

static forceinline
void *TLS_TOP(void) {
    void *ret;
    // FIXME: get tls bottom address here
    asm("unimp 10 ! ldfp %0" : "=r"(ret));
    return ret;
}

/* TLS_BASE(): return base address of all TLS spaces */
static forceinline
void *TLS_BASE(void)
{
    // FIXME: get tls bottom address here
    return (void*)(1ULL << 63);
}

/* TLS_NEXT(P): return address of space after P, 0 if no more space */
static forceinline
void *TLS_NEXT(void *p)
{
    // FIXME: check this for FPGA
    uintptr_t tls_size = (uintptr_t)TLS_TOP() - (uintptr_t)TLS_BOTTOM();
    char *next = ((char*)p) + tls_size;
    if (!(((uintptr_t)next >= (uintptr_t)TLS_BASE()) &&
          ((uintptr_t)next <= ~(uintptr_t)0)))
        return 0;
    return next;
}


/* IS_TLS_PTR(P): true if P is managed by this heap allocator */
static forceinline
uintptr_t IS_TLS_PTR(void *ptr)
{
    // FIXME. compare with base and top pointers instead
    return (((uintptr_t)ptr) & (((uintptr_t)1) << (sizeof(void*)*CHAR_BIT-1)));
}

/* IS_TLS_LOCAL(P): true if P is in the current thread's TLS address space */
static forceinline
bool IS_TLS_LOCAL(void *ptr)
{
    // FIXME: on fpga these are two separate address spaces
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
    // FIXME: on fpga the full tls heap can be used
    uintptr_t med = top/2 + bottom/2;
    return (med - bottom)/SLOT_SIZE;
}


#endif
