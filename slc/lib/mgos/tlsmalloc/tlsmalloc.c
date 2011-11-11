#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "tlsmalloc.h"
#include "tlsconfig.h"
#include "tricks.h"
#include ARCH_TLS_SERVICES

#ifdef DEBUG_MGSIM
#include <svp/mgsim.h>
#include <svp/testoutput.h>
#endif

/*** Data structures ***/

struct superblock;

struct control_block {
#ifdef FOOTERS
    unsigned long  cb_magic;
#define CB_MAGIC 0 /* must be 0 to match uninitialized space on mgsim */
#endif
    size_t nr_allocated_sbs;
    struct superblock *bins[NR_OF_BINS];
    struct superblock *first_available;
    size_t nr_busy_sbs;

    BITMAP_LIMB_T bitmap[];
};

#ifdef FOOTERS
#define check_cb(X)  ({ struct control_block *__cb = (X); assert(__cb->cb_magic == CB_MAGIC); __cb; })
#else
#define check_cb(X) (X)
#endif

static forceinline
size_t bitmap_search_0(const BITMAP_LIMB_T *bitmap)
{
    BITMAP_LIMB_T t;
    size_t ret;
    for (ret = 0; 
         ((t = *bitmap++) == ~(BITMAP_LIMB_T)0); 
         ret += sizeof(BITMAP_LIMB_T)*CHAR_BIT)
        ;
    if (!t) return ret;
    else return ret + CTZ(~t);
}

static forceinline
BITMAP_LIMB_T bitmap_get(const BITMAP_LIMB_T *bitmap, size_t n)
{
    BITMAP_LIMB_T mask = (BITMAP_LIMB_T)(1UL << (n % BITS_PER_LIMB));
    return bitmap[n / BITS_PER_LIMB] & mask;
}

static forceinline
void bitmap_set_0(BITMAP_LIMB_T *bitmap, size_t n)
{
    BITMAP_LIMB_T mask = ~(BITMAP_LIMB_T)(1UL << (n % BITS_PER_LIMB));
    bitmap[n / BITS_PER_LIMB] &= mask;
}

static forceinline
void bitmap_set_1(BITMAP_LIMB_T *bitmap, size_t n)
{
    BITMAP_LIMB_T mask = (BITMAP_LIMB_T)(1UL << (n % BITS_PER_LIMB));
    bitmap[n / BITS_PER_LIMB] |= mask;
}

struct block;

struct superblock {
    struct superblock_header {
#ifdef FOOTERS
        unsigned long  sb_magic;
#define SB_MAGIC 0xCAFE69
#endif
        struct superblock *next; /* next in bin, next in avail */
        struct superblock *prev; /* prev in bin */
        struct block *first_free;
        size_t nr_allocated_blocks;
        size_t bin; /* index in bins array in control_block */
        size_t wilderness_size;
        struct block *gc_hint; /* where to resume garbage collection */
    } header;
#define WILDERNESS_SIZE (SLOT_SIZE - sizeof(struct superblock_header))
    char wilderness[WILDERNESS_SIZE];
};

#ifdef FOOTERS
#define check_sb(X)  ({ struct superblock *__sb = (X); assert(__sb->header.sb_magic == SB_MAGIC); __sb; })
#else
#define check_sb(X) (X)
#endif


struct block {
    BLOCK_TAG_T tag;
    struct {
        struct block *next_free;
        char data[];
    } payload;   
};

#ifdef FOOTERS
#define check_free_block(X) ({ struct block *__b = (X); if (likely(__b != 0)) assert(__b->tag == TAG_FREE); __b; })
#else
#define check_free_block(X) (X)
#endif


#ifdef FOOTERS
#define TAG_FREE 0xCAFE51
#define TAG_PENDING 0xCAFE37
#define TAG_BUSY 0xCAFE42
#else
#define TAG_FREE 0  
#define TAG_PENDING 1
#define TAG_BUSY 0
#endif

/*** Helpers ***/

static inline
struct block* block_of_ptr(void *ptr)
{
    char *payload = (void*)ptr;
    return (struct block*)(void*)(payload - offsetof(struct block, payload));
}

static inline
struct superblock* sb_of_block(struct block* ptr)
{
    uintptr_t mask = ~(uintptr_t)(SLOT_SIZE-1);
    uintptr_t sb_base = ((uintptr_t)ptr) & mask;
    return (struct superblock*)sb_base;
}

static inline
void free_block(struct superblock *sb, struct block *b)
{
    assert(sb->header.nr_allocated_blocks > 0);
    b->payload.next_free = check_free_block(sb->header.first_free);
    sb->header.first_free = b;
    --(sb->header.nr_allocated_blocks);
}

static inline
size_t sb_is_empty(struct superblock *sb)
{
    size_t isempty = !sb->header.nr_allocated_blocks;
#ifdef DEBUG_MALLOC
    fprintf(stderr, "sb %p is %s (alloc %zu)\n", 
            sb, isempty ? "empty" : "nonempty", sb->header.nr_allocated_blocks);
#endif
    assert(!isempty || (0 == sb->header.nr_allocated_blocks));
    return isempty;
}

static inline
bool sb_is_full(struct superblock *sb, size_t blocksize)
{
    bool isfull = likely(sb->header.first_free == 0) 
        && likely(sb->header.wilderness_size < blocksize);
#ifdef DEBUG_MALLOC
    fprintf(stderr, "sb %p is %s (alloc %zu, ff %p, w %zu, bs %u)\n", 
            sb, isfull ? "full" : "nonfull", sb->header.nr_allocated_blocks,
            sb->header.first_free, sb->header.wilderness_size, (unsigned)blocksize);
#endif
    assert(!isfull || (0 < sb->header.nr_allocated_blocks));
    return isfull;
}

static inline
void move_to_bin_head(struct control_block *cb, struct superblock *sb,
                      size_t bin)
{
    cb->bins[bin] = sb;
}

static inline
void free_superblock(struct control_block *cb, struct superblock *sb)
{
    assert(sb->header.nr_allocated_blocks == 0);

    struct superblock *prev = sb->header.prev;
    if (prev == sb) 
    {
        /* only sb in bin, keep it */
        assert(sb->header.next == sb);
        assert(cb->bins[sb->header.bin] == sb);
#ifdef DEBUG_MALLOC
        fprintf(stderr, "sb %p -> last, keep\n", sb);
#endif
        return ;
    }

    size_t bin = sb->header.bin;
    struct superblock *next = sb->header.next;

    /* remove from circular list */
    assert(prev != 0);
    assert(next != 0);
    prev = check_sb(prev);
    next = check_sb(next);
    next->header.prev = prev;
    prev->header.next = next;
    cb->bins[bin] = next;

#ifdef DEBUG_MALLOC
    fprintf(stderr, "sb %p -> free, new candidate %p\n", sb, next);
#endif

    /* make available */
    sb->header.next = cb->first_available;
    cb->first_available = sb;
    --(cb->nr_busy_sbs);

#ifdef RELEASE_STORAGE
    /* threshold: half the count of allocated superblocks */
    size_t half_alloc = cb->nr_allocated_sbs / 2;
    size_t busy = cb->nr_busy_sbs;
#ifdef DEBUG_MALLOC_STORAGE
    fprintf(stderr, "%zu busy sbs, %zu alloc, half %zu\n",
            cb->nr_busy_sbs, cb->nr_allocated_sbs, half_alloc);
#endif
    if (half_alloc && (busy < half_alloc))
    {
        struct superblock *asb;
        struct superblock *next;
        for (asb = cb->first_available, next = asb->header.next; 
             busy < half_alloc; 
             --half_alloc, asb = next, next = asb->header.next)
        {
#ifdef DEBUG_MALLOC_STORAGE
            fprintf(stderr, "unmap sb %p\n", asb);
#endif
            asb = check_sb(asb);
            UNMAP_STORAGE(asb, SLOT_SIZE);

            /* inform */
            size_t slot_nr = (((char*)(void*)asb) - ((char*)(void*)cb))/SLOT_SIZE-1;
            bitmap_set_0(cb->bitmap, slot_nr);
            --(cb->nr_allocated_sbs);
        }
        cb->first_available = asb;
    }
#endif
}


/*** Heap services ***/

static inline
struct superblock* init_superblock(struct control_block *cb,
                                   struct superblock *sb,
                                   size_t bin)
{
    /* insert into bin */
    struct superblock *current_first = cb->bins[bin];
    if (current_first)
    {
        current_first = check_sb(current_first);

        /* insert into list */
        struct superblock *current_next = check_sb(current_first->header.next);
        sb->header.next = current_next;
        sb->header.prev = current_first;
        current_first->header.next = sb;
        current_next->header.prev = sb;
    }
    else
    {
        /* no list yet, become our own list */
        sb->header.prev = sb->header.next = sb;
    }

    sb->header.bin = bin;

    /* move to bin head */
    cb->bins[bin] = sb;

    /* initialize free list */
    sb->header.first_free = 0;
    sb->header.nr_allocated_blocks = 0;

    /* initialize wilderness */
    sb->header.wilderness_size = WILDERNESS_SIZE;
    sb->header.gc_hint = 0;

#ifdef FOOTERS
    /* initialize magic */
    sb->header.sb_magic = SB_MAGIC;
#endif

    /* inform business */
    ++(cb->nr_busy_sbs);

    return sb;
}

static inline
struct superblock* get_new_superblock(struct control_block *cb,  size_t bin)
{
    struct superblock *new_sb;
    if (likely((new_sb = cb->first_available) != 0))
    {
        new_sb = check_sb(new_sb);

        /* free list not empty, use it */
        cb->first_available = new_sb->header.next;
        return init_superblock(cb, new_sb, bin);
    }
    else
    {
        /* free list empty, need a new superblock */

        size_t nr_alloc = cb->nr_allocated_sbs;
        if (unlikely(nr_alloc == 0))
        {
            /* the controlblock itself is not yet allocated, do this first of hand */
            if (MAP_STORAGE(cb, SLOT_SIZE) == MAP_STORAGE_FAIL)
                return 0;
            nr_alloc = 1;
        }

        /* are there available slots? */
        if (unlikely((NR_SLOTS_IN_TLS() - nr_alloc) == 0))
            /* no */ return 0;

        /* yes */
        size_t slot_nr = bitmap_search_0(cb->bitmap);
        uintptr_t base = ((uintptr_t)cb) + (slot_nr + 1) * SLOT_SIZE;

#ifdef DEBUG_MALLOC_STORAGE
        fprintf(stderr, "map sb %p\n", (struct superblock*)base);
#endif
        if (MAP_STORAGE((void*)base, SLOT_SIZE) == MAP_STORAGE_FAIL)
            /* no storage, abandon */
            return 0;

        /* inform */
        bitmap_set_1(cb->bitmap, slot_nr);
        cb->nr_allocated_sbs = nr_alloc + 1;

        /* init */
        return init_superblock(cb, (struct superblock*)base, bin);
    }
}

static inline
struct block *carve_new_block(struct superblock *sb,
                              size_t blocksize)
{
    /* here the superblock needs a new block, and
       the wilderness has enough room for one */
    uintptr_t base = (uintptr_t)&sb->wilderness;
    base += (sb->header.wilderness_size -= blocksize);
    return (struct block*) base;
}

static inline
struct block* register_allocated_block(struct superblock *sb,
                                       struct block *bl)
{
    ++(sb->header.nr_allocated_blocks);
    return bl;
}


static inline
struct block *alloc_block_from_nonempty_sb(struct control_block *cb,
                                           struct superblock *sb,
                                           size_t blocksize,
                                           size_t bin)
{
    struct block *bl;

    /* here sb points to a valid, non-full superblock */
    
    /* is the free list non-empty? */
    if ((bl = check_free_block(sb->header.first_free)) != 0) 
    {
        /* remove from free list */
        sb->header.first_free = check_free_block(bl->payload.next_free);
    } 
    else
    {
        /* wilderness not empty */
        bl = carve_new_block(sb, blocksize);
    }

    bl = register_allocated_block(sb, bl);

    /* has the sb become full? */
    if (sb_is_full(sb, blocksize))
        /* yes: heuristic: move next to head of bin */
        move_to_bin_head(cb, sb->header.next, bin);

    return bl;
}

static inline
struct block* garbage_collect(struct control_block *cb,
                              struct superblock *sb_start,
                              size_t blocksize,
                              size_t bin)
{
    /* scan the superblocks for this bin and attempt to find
       some free blocks. Return 0 if no free block was found,
       otherwise stop at the first free block encountered */

    struct superblock *sb = sb_start;
    do
    {
#ifdef DEBUG_MALLOC
        fprintf(stderr, "gc sb %p\n", sb);
#endif
        uintptr_t bottom = (uintptr_t)(sb->wilderness) + sb->header.wilderness_size;
        uintptr_t top = (uintptr_t)(sb) + sizeof(*sb);

        uintptr_t hint = (uintptr_t)(sb->header.gc_hint);
        if (hint == 0) hint = top - blocksize;
        
        uintptr_t p;
        /* scan from hint to bottom */
        for (p = hint; p >= bottom; p -= blocksize)
        {
            struct block *bl = (struct block*)p;
            if (unlikely(bl->tag == TAG_PENDING))
            {
                /* found a free block, remember for next time */
                sb->header.gc_hint = bl-1;
                return bl;
            }
        }
        /* scan from top to hint */
        for (p = top - blocksize; p > hint; p -= blocksize)
        {
            struct block *bl = (struct block*)p;
            if (unlikely(bl->tag == TAG_PENDING))
            {
                /* found a free block, remember for next time */
                sb->header.gc_hint = bl-1;
                return bl;
            }
        }

        sb = check_sb(sb->header.next);

        /* if the next superblock has some free blocks, just use that directly */
        if (!sb_is_full(sb, blocksize))
        {
            move_to_bin_head(cb, sb, bin);
            return alloc_block_from_nonempty_sb(cb, sb, blocksize, bin);
        }
         
    } while(sb != sb_start);

    return 0;
}

static inline
void* tag_and_get_payload(struct block *bl)
{
    bl->tag = TAG_BUSY;
    return &(bl->payload);
}

static inline
void *alloc_from_empty_sb(struct superblock *sb,
                          size_t blocksize)
{
    struct block *bl;
    bl = carve_new_block(sb, blocksize);
    bl = register_allocated_block(sb, bl);
    return tag_and_get_payload(bl);
}

static inline
void *alloc_from_nonfull_sb(struct control_block *cb,
                            struct superblock *sb,
                            size_t blocksize,
                            size_t bin)
{
    struct block *bl;
    bl = alloc_block_from_nonempty_sb(cb, sb, blocksize, bin);
    return tag_and_get_payload(bl);
}

static forceinline
size_t find_bin(size_t sz)
{
    sz += sizeof(BLOCK_TAG_T);
    sz -= 1;
    sz |= FIRST_SIZE_CLASS/2;  
    size_t l2 = __builtin_clzl(sz);
    l2 = sizeof(size_t)*CHAR_BIT - l2 - __builtin_ctzl(FIRST_SIZE_CLASS);
    return l2;
}

static forceinline
size_t get_blocksize(size_t bin)
{
    return ((size_t)FIRST_SIZE_CLASS) << bin;
}

void* tls_malloc(size_t sz)
{
#ifdef DEBUG_MGSIM
//    mgsim_control(0x6d616c6c6f6300ULL, MGSCTL_TYPE_STATACTION, MGSCTL_SA_CONTINUE, MGSCTL_CHAN_DEBUG);
    output_uint(sz, 0);
#endif
    size_t ubin = find_bin(sz);
    if (unlikely(ubin >= NR_OF_BINS))
        goto fallback;

    void *ret;
    struct control_block *cb = CURRENT_CB();

    cb = check_cb(cb);

    struct superblock **bin;
    size_t blocksize;
    bin = cb->bins + ubin;
    struct superblock *sb;
    struct block *bl;

    blocksize = get_blocksize(ubin);

    if (unlikely(*bin == 0)) {
        /* bin empty */
        goto new_needed;
    }

    /* here *bin is valid, may be full ? */
    sb = check_sb(*bin);
    
    if (unlikely(sb_is_full(sb, blocksize)))
    {
#ifndef DISABLE_GC
        bl = garbage_collect(cb, sb, blocksize, ubin);
#else
        /* WARNING: when GC is disabled, all non-local calls to free() will cause a memory leak! */
        bl = 0;
#endif
        
        if (bl == 0)
            /* GC failed */
            goto new_needed;
        else
        {
            ret = tag_and_get_payload(bl);
#ifdef DEBUG_MALLOC
            fprintf(stderr, "malloc gc sb (%p): %p-%p (sz %zu -> %zu)\n", 
                    sb, ret, ret + blocksize - offsetof(struct block, payload) - 1,
                    sz, blocksize - offsetof(struct block, payload));
#endif
            return ret;
        }
    } 
    ret = alloc_from_nonfull_sb(cb, sb, blocksize, ubin);

#ifdef DEBUG_MALLOC
    fprintf(stderr, "malloc nonfull sb (%p): %p-%p (sz %zu -> %zu)\n", 
            sb, ret, ret + blocksize - offsetof(struct block, payload) - 1,
            sz, blocksize - offsetof(struct block, payload));
#endif
#ifdef DEBUG_MGSIM
    output_hex(ret, 0);
#endif
    return ret;

new_needed:
    sb = get_new_superblock(cb, ubin);

    if (unlikely(sb == 0)) /* no more superblock left, fall back */
        goto fallback;
    
    *bin = sb;
    ret = alloc_from_empty_sb(sb, blocksize);
#ifdef DEBUG_MALLOC
    fprintf(stderr, "malloc empty sb (%p): %p-%p (sz %zu -> %zu)\n", 
            sb, ret, ret + blocksize - offsetof(struct block, payload) - 1,
            sz, blocksize - offsetof(struct block, payload));
#endif
#ifdef DEBUG_MGSIM
    output_hex(ret, 0);
#endif
    return ret;

fallback:
    ret = EXT_MALLOC(sz);
#ifdef DEBUG_MALLOC
    fprintf(stderr, "malloc fallback %p (%zu)\n", ret, sz);
#endif
#ifdef DEBUG_MGSIM
    output_hex(ret, 0);
#endif
    return ret;
    
}

void *tls_calloc(size_t cnt, size_t sz)
{
    void *p = tls_malloc(cnt*sz);
    if (unlikely(!p)) return p;
    memset(p, 0, sz);
    return p;
}

void tls_free(void *ptr)
{
#ifdef DEBUG_MGSIM
    output_hex(ptr, 0);
#endif
#ifdef DEBUG_MALLOC
    fprintf(stderr, "free %p\n", ptr);
#endif    
    if (!IS_TLS_PTR(ptr))
        EXT_FREE(ptr);
    else if (!IS_TLS_LOCAL(ptr)) {
        /* FIXME: memory consistency: delegate to home place */
#ifdef DEBUG_MALLOC
        uintptr_t tls_base = (uintptr_t)TLS_BASE();
        uintptr_t tls_top = (uintptr_t)TLS_TOP();
        uintptr_t tls_size = tls_top - tls_base;
        uintptr_t ptr_where = (uintptr_t)ptr & (tls_size - 1);
        if (ptr_where >= tls_size/2)
        {
            fprintf(stderr, "warning: maybe free of stack pointer %p\n", ptr);
        }
#endif
        struct block *block = block_of_ptr(ptr);
#ifdef FOOTERS
        assert(block->tag == TAG_BUSY);
#endif
        block->tag = TAG_PENDING;
    }
    else {
        /*** this thread owns the superblock ***/

        /* 1. */
        struct block* block = block_of_ptr(ptr);
#ifdef FOOTERS
        assert(block->tag == TAG_BUSY);
#endif
        block->tag = TAG_FREE;

        /* 2. */
        struct superblock* sb = check_sb(sb_of_block(block));

#ifdef DEBUG_MALLOC
        fprintf(stderr, "free %p in sb %p (alloc %zu)\n", 
                ptr,  sb, sb->header.nr_allocated_blocks);
#endif

        free_block(sb, block);


        /* 3. */
        struct control_block *cb = check_cb(CURRENT_CB());
        if (!sb_is_empty(sb)) 
        {
#ifdef DEBUG_MALLOC
            fprintf(stderr, "sb %p -> head\n", sb);
#endif
            move_to_bin_head(cb, sb, sb->header.bin);
        }
        else 
        {
#ifdef DEBUG_MALLOC
            fprintf(stderr, "sb %p -> free\n", sb);
#endif
            /* 4. */
            free_superblock(cb, sb);
        }
    }        
}

void *tls_realloc(void *ptr, size_t ns)
{
    // FIXME: optimize this.
#ifdef DEBUG_MGSIM
    output_hex(ptr, 0);
    output_uint(ns, 0);    
#endif
#ifdef DEBUG_MALLOC
    fprintf(stderr, "realloc: %p %zu\n", ptr, ns);
#endif

    if (unlikely(!IS_TLS_PTR(ptr)))
        return EXT_REALLOC(ptr, ns);

    struct block* bl = block_of_ptr(ptr);
    struct superblock *sb = check_sb(sb_of_block(bl));
    
    size_t bin = sb->header.bin;
    
    assert(bin < NR_OF_BINS);
    size_t osz = get_blocksize(bin) - offsetof(struct block, payload);

    if (unlikely(osz >= ns)) {
#ifdef DEBUG_MALLOC
        fprintf(stderr, "realloc reuse %p (%zu -> %zu)\n", 
                ptr, osz, ns);
#endif
#ifdef DEBUG_MGSIM
        mgsim_control(0x726575736500ULL, MGSCTL_TYPE_STATACTION, MGSCTL_SA_CONTINUE, MGSCTL_CHAN_DEBUG);
#endif        
        // nothing to do
        return ptr;
    }

    void * p = tls_malloc(ns);
#ifdef DEBUG_MGSIM
    output_hex(p, 0);
#endif
    if (unlikely(!p)) return p;

#ifdef DEBUG_MALLOC
    fprintf(stderr, "realloc: %p -> %p (%zu) -> %p (%zu)\n", 
            ptr, ptr+osz, osz, p, ns);
#endif

    memcpy(p, ptr, osz);
    tls_free(ptr);
    return p;
}

static
void tls_sb_stats(struct superblock *sb, unsigned bin, size_t blocksize)
{
    printf("  sb %p: "
           "link (%p,%p), "
           "freelist %p, "
           "alloc %zu (%.2f%%), "
           "bin %zu, "
           "wild %zu, "
           "hint %p\n",
           sb, 
           sb->header.prev, sb->header.next,
           sb->header.first_free,
           sb->header.nr_allocated_blocks,
           (float)(sb->header.nr_allocated_blocks)*100. / (float)(WILDERNESS_SIZE/blocksize),
           sb->header.bin,
           sb->header.wilderness_size,
           sb->header.gc_hint);
    assert(sb->header.bin == bin);
}

void tls_local_stats(void *p)
{
    unsigned bin;
    struct superblock *sb;
    struct superblock *sb_first;
    struct control_block *cb = (struct control_block*)p;
    size_t sz;

    printf("stats for cb %p:\n"
           "  storage: %zu KB alloc (%zu sbs), "
           "%zu KB used (%zu sbs, %.2f%%)\n",
           cb, 
           (cb->nr_allocated_sbs) * SLOT_SIZE / 1024, 
           (cb->nr_allocated_sbs),
           (cb->nr_busy_sbs+1) * SLOT_SIZE / 1024,
           (cb->nr_busy_sbs+1),
           ((float)(cb->nr_busy_sbs+1)*100./(float)(cb->nr_allocated_sbs)));
    
    for (bin = 0; bin < NR_OF_BINS; ++bin)
    {
        printf("  bin %u: ", bin);
        sb_first = cb->bins[bin]; 
        if (sb_first)
        {
            sb = sb_first;
            do
            {
                printf("%p -> ", sb);
                sb = sb->header.next;
            } while(sb != sb_first);
        }                      
        printf("end\n");
    }

    printf("  freelist: ");
    for (sb = cb->first_available; sb != 0; sb = sb->header.next)
        printf("%p -> ", sb);
    printf("end\n");

    for (bin = 0; bin < NR_OF_BINS; ++bin)
    {
        sz = get_blocksize(bin);
        sb_first = cb->bins[bin]; 
        if (sb_first)
        {
            sb = sb_first;
            do
            {
                tls_sb_stats(sb, bin, sz);
                sb = sb->header.next;
            } while(sb != sb_first);
        }                      
    }

}

void tls_malloc_stats(void)
{
    struct control_block *cb;

    for (cb = TLS_BASE(); cb != 0; cb = TLS_NEXT(cb))
    {
        if (cb->nr_allocated_sbs)
            tls_local_stats(cb);
    }
}


