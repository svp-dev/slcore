#ifndef TLSCONFIG_H
#define TLSCONFIG_H

#include <limits.h>

/*** Configuration ***/

#define SLOT_SIZE 8192


#define BITMAP_LIMB_T unsigned long
#define CTZ __builtin_ctzl
#define BITS_PER_LIMB (sizeof(BITMAP_LIMB_T)*CHAR_BIT)

#define BLOCK_TAG_T unsigned long /* tag at start of block */
#define UWORD       unsigned long /* word sized unsigned int */

#ifdef DEBUG_MALLOC_STORAGE

#define NR_OF_BINS 1
#define SZ_CLASS_0 (256-sizeof(BLOCK_TAG_T))
#define MAXBINSIZE SZ_CLASS_0
#define DELEGATION_THRESHOLD 256

#else

#define NR_OF_BINS 6
#define SZ_CLASS_0 (16-sizeof(BLOCK_TAG_T))
#define SZ_CLASS_1 (32-sizeof(BLOCK_TAG_T))
#define SZ_CLASS_2 (64-sizeof(BLOCK_TAG_T))
#define SZ_CLASS_3 (128-sizeof(BLOCK_TAG_T))
#define SZ_CLASS_4 (256-sizeof(BLOCK_TAG_T))
#define SZ_CLASS_5 (512-sizeof(BLOCK_TAG_T))
#define MAXBINSIZE SZ_CLASS_5
#define DELEGATION_THRESHOLD 512

#endif

#endif
