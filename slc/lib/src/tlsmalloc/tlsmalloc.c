#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

#include "tlsmalloc.h"
#include "tlsconfig.h"
#include "tricks.h"
#include ARCH_TLS_SERVICES

/*** Data structures ***/

struct superblock;

struct control_block {
    struct superblock *bins[NR_OF_BINS];

    struct superblock *first_available;
    size_t nr_allocated_sbs;
    size_t nr_busy_sbs;

    BITMAP_LIMB_T bitmap[];
};

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

struct block {
    BLOCK_TAG_T tag;
    struct {
        struct block *next_free;
        char data[];
    } payload;   
};

#define TAG_NONPENDING 0
#define TAG_PENDING 1

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
    b->payload.next_free = sb->header.first_free;
    sb->header.first_free = b;
    --(sb->header.nr_allocated_blocks);
}

static inline
size_t sb_is_empty(struct superblock *sb)
{
    return sb->header.nr_allocated_blocks;
}

static inline
bool sb_is_full(struct superblock *sb, UWORD blocksize)
{
    return (sb->header.first_free == 0) 
        && (sb->header.wilderness_size < blocksize);
}

static inline
void move_to_bin_head(struct control_block *cb, struct superblock *sb,
                      UWORD bin)
{
    cb->bins[bin] = sb;
}

static inline
void free_superblock(struct control_block *cb, struct superblock *sb)
{
    struct superblock *prev = sb->header.prev;
    if (prev == sb) 
    {
        /* only sb in bin, keep it */
        assert(sb->header.next == sb);
        assert(cb->bins[sb->header.bin] == sb);
        return ;
    }

    UWORD bin = sb->header.bin;
    struct superblock *next = sb->header.next;

    /* remove from circular list */
    assert(prev != 0);
    assert(next != 0);
    next->header.prev = prev;
    prev->header.next = next;
    cb->bins[bin] = next;

    /* make available */
    sb->header.next = cb->first_available;
    cb->first_available = sb;
    --(cb->nr_busy_sbs);

#ifdef RELEASE_STORAGE
    /* threshold: half the count of allocated superblocks */
    if (cb->nr_busy_sbs < (cb->nr_allocated_sbs / 2))
    {
#error TODO: "release some available superblocks"
    }
#endif
}


/*** Heap services ***/

static inline
struct superblock* init_superblock(struct control_block *cb,
                                   struct superblock *sb,
                                   UWORD bin)
{
    /* insert into bin */
    struct superblock *current_first = cb->bins[bin];
    if (current_first)
    {
        /* insert into list */
        struct superblock *current_next = current_first->header.next;
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

    /* inform business */
    ++(cb->nr_busy_sbs);
}

static inline
struct superblock* get_new_superblock(struct control_block *cb,  UWORD bin)
{
    struct superblock *new_sb;
    if (likely((new_sb = cb->first_available) != 0))
    {
        /* free list not empty, use it */
        cb->first_available = new_sb->header.next;
        return init_superblock(cb, new_sb, bin);
    }
    else
    {
        /* free list empty, need a new superblock */
        
        /* are there available slots? */
        if (unlikely((NR_SLOTS_IN_TLS() - cb->nr_allocated_sbs) == 0))
            /* no */ return 0;

        /* yes */
        size_t slot_nr = bitmap_search_0(cb->bitmap);
        uintptr_t base = ((uintptr_t)cb) + (slot_nr + 1) * SLOT_SIZE;

        if (!MAP_STORAGE(base, SLOT_SIZE))
            /* no storage, abandon */
            return 0;

        /* inform */
        bitmap_set_0(cb->bitmap, slot_nr);
        ++(cb->nr_allocated_sbs);

        /* init */
        return init_superblock(cb, (struct superblock*)base, bin);
    }
}

static inline
struct block *carve_new_block(struct superblock *sb,
                              UWORD blocksize)
{
    /* here the superblock needs a new block, and
       the wilderness has enough room for one */
    uintptr_t base = (uintptr_t)&sb->wilderness;
    base += (sb->header.wilderness_size -= blocksize);
    return (struct block*) base;
}

static inline
struct block *alloc_block_from_nonempty_sb(struct control_block *cb,
                                           struct superblock *sb,
                                           UWORD blocksize,
                                           UWORD bin)
{
    struct block *bl;

    /* here sb points to a valid, non-full superblock */
    
    /* is the free list non-empty? */
    if ((bl = sb->header.first_free) != 0) 
    {
        /* remove from free list */
        sb->header.first_free = bl->payload.next_free;
    } 
    else
    {
        /* wilderness not empty */
        bl = carve_new_block(sb, blocksize);
    }

    /* has the sb become full? */
    if (sb_is_full(sb, blocksize))
        /* yes: heuristic: move next to head of bin */
        move_to_bin_head(cb, sb->header.next, bin);

    return bl;
}

static inline
struct block* garbage_collect(struct control_block *cb,
                              struct superblock *sb_start,
                              UWORD blocksize,
                              UWORD bin)
{
    /* scan the superblocks for this bin and attempt to find
       some free blocks. Return 0 if no free block was found,
       otherwise stop at the first free block encountered */

    struct superblock *sb = sb_start;
    do
    {
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

        sb = sb->header.next;

        /* if the next superblock has some free blocks, just use that directly */
        if (!sb_is_full(sb, blocksize))
            return alloc_block_from_nonempty_sb(cb, sb, blocksize, bin);
         
    } while(sb != sb_start);
}

static inline
void* tag_and_get_payload(struct block *bl)
{
    bl->tag = TAG_NONPENDING;
    return &(bl->payload);
}

static inline
struct block* register_allocated_block(struct superblock *sb,
                                       struct block *bl)
{
    ++(sb->header.nr_allocated_blocks);
    return bl;
}

static inline
void *alloc_from_empty_sb(struct superblock *sb,
                          UWORD blocksize)
{
    struct block *bl;
    bl = carve_new_block(sb, blocksize);
    bl = register_allocated_block(sb, bl);
    return tag_and_get_payload(bl);
}

static inline
void *alloc_from_nonfull_sb(struct control_block *cb,
                            struct superblock *sb,
                            UWORD blocksize,
                            UWORD bin)
{
    struct block *bl;
    bl = alloc_block_from_nonempty_sb(cb, sb, blocksize, bin);
    bl = register_allocated_block(sb, bl);
    return tag_and_get_payload(bl);
}


void* tls_malloc(size_t sz)
{
    if (unlikely(sz > MAXBINSIZE)) goto fallback;

    struct control_block *cb = CURRENT_CB();
    struct superblock **bin;
    UWORD blocksize;
    if (sz <= SZ_CLASS_0) { bin = cb->bins + 0; blocksize = SZ_CLASS_0+sizeof(BLOCK_TAG_T); }
#if NR_OF_BINS > 1
    else if (sz <= SZ_CLASS_1) { bin = cb->bins + 1; blocksize = SZ_CLASS_1+sizeof(BLOCK_TAG_T); }
#endif
#if NR_OF_BINS > 2
    else if (sz <= SZ_CLASS_2) { bin = cb->bins + 2; blocksize = SZ_CLASS_2+sizeof(BLOCK_TAG_T); }
#endif
#if NR_OF_BINS > 3
    else if (sz <= SZ_CLASS_3) { bin = cb->bins + 3; blocksize = SZ_CLASS_3+sizeof(BLOCK_TAG_T); }
#endif
#if NR_OF_BINS > 4
    else if (sz <= SZ_CLASS_4) { bin = cb->bins + 4; blocksize = SZ_CLASS_4+sizeof(BLOCK_TAG_T); }
#endif
#if NR_OF_BINS > 5
    else if (sz <= SZ_CLASS_5) { bin = cb->bins + 5; blocksize = SZ_CLASS_5+sizeof(BLOCK_TAG_T); }
#endif
#if NR_OF_BINS > 6
    else if (sz <= SZ_CLASS_6) { bin = cb->bins + 6; blocksize = SZ_CLASS_6+sizeof(BLOCK_TAG_T); }
#endif
/* (extend as needed) */
    
    struct superblock *sb;
    struct block *bl;

    if (unlikely(*bin == 0)) {
        /* bin empty */
        goto new_needed;
    }

    /* here *bin is valid, may be full ? */
    sb = *bin;
    if (unlikely(sb_is_full(sb, blocksize)))
    {
        bl = garbage_collect(cb, sb, blocksize, bin - cb->bins);
        
        if (bl == 0)
            /* GC failed */
            goto new_needed;
        else
            return tag_and_get_payload(bl);
    } 
    return alloc_from_nonfull_sb(cb, sb, blocksize, bin - cb->bins);


new_needed:
    sb = get_new_superblock(cb, bin - cb->bins);

    if (unlikely(sb == 0)) /* no more superblock left, fall back */
        goto fallback;
    
    *bin = sb;
    return alloc_from_empty_sb(sb, blocksize);

fallback:
    return EXT_MALLOC(sz);
    
}


void tls_free(void *ptr)
{
    if (!IS_TLS_PTR(ptr))
        EXT_FREE(ptr);
    else if (!IS_TLS_LOCAL(ptr))
        /* FIXME: memory consistency: delegate to home place */
        block_of_ptr(ptr)->tag = TAG_PENDING;
    else {
        /*** this thread owns the superblock ***/

        /* 1. */
        struct block* block = block_of_ptr(ptr);
        block->tag = TAG_NONPENDING;

        /* 2. */
        struct superblock* sb = sb_of_block(block);
        free_block(sb, block);

        /* 3. */
        struct control_block *cb = CURRENT_CB();
        if (!sb_is_empty(sb)) 
            move_to_bin_head(cb, sb, sb->header.bin);
        else 
            /* 4. */
            free_superblock(cb, sb);
    }        
}



