#ifndef TLSCONFIG_H
#define TLSCONFIG_H

#include <limits.h>

/*** Configuration ***/

#define SLOT_SIZE 8192


#define BITMAP_LIMB_T unsigned long
#define CTZ __builtin_ctzl
#define BITS_PER_LIMB (sizeof(BITMAP_LIMB_T)*CHAR_BIT)

#define BLOCK_TAG_T unsigned long /* tag at start of block */

#ifdef DEBUG_MALLOC_STORAGE

#define NR_OF_BINS 1
#define FIRST_SIZE_CLASS 256

#else

#define NR_OF_BINS 6
#define FIRST_SIZE_CLASS 16

#endif

#endif
