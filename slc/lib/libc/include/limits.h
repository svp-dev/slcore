//
// limits.h: this file is part of the SL toolchain.
//
// Copyright (C) 2009,2010 The SL project.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//

#ifndef SLC_MTA_LIMITS_H
# define SLC_MTA_LIMITS_H

#if defined(__alpha__)||defined(__mtalpha__)||defined(__mtsparc__)||defined(__mips__)

#define MB_LEN_MAX      16

/* Number of bits in a `char'.  */
#  define CHAR_BIT      8

/* Minimum and maximum values a `signed char' can hold.  */
#  define SCHAR_MIN     (-128)
#  define SCHAR_MAX     127

/* Maximum value an `unsigned char' can hold.  (Minimum is 0.)  */
#  define UCHAR_MAX     255

/* Minimum and maximum values a `char' can hold.  */
#  ifdef __CHAR_UNSIGNED__
#   define CHAR_MIN     0
#   define CHAR_MAX     UCHAR_MAX
#  else
#   define CHAR_MIN     SCHAR_MIN
#   define CHAR_MAX     SCHAR_MAX
#  endif

/* Minimum and maximum values a `signed short int' can hold.  */
#  define SHRT_MIN      (-32768)
#  define SHRT_MAX      32767

/* Maximum value an `unsigned short int' can hold.  (Minimum is 0.)  */
#  define USHRT_MAX     65535

/* Minimum and maximum values a `signed int' can hold.  */
#  define INT_MIN       (-INT_MAX - 1)
#  define INT_MAX       2147483647

/* Maximum value an `unsigned int' can hold.  (Minimum is 0.)  */
#  define UINT_MAX      4294967295U
/* Minimum and maximum values a `signed long int' can hold.  */
#  if __WORDSIZE == 64
#   define LONG_MAX     9223372036854775807L
#  else
#   define LONG_MAX     2147483647L
#  endif
#  define LONG_MIN      (-LONG_MAX - 1L)

/* Maximum value an `unsigned long int' can hold.  (Minimum is 0.)  */
#  if __WORDSIZE == 64
#   define ULONG_MAX    18446744073709551615UL
#  else
#   define ULONG_MAX    4294967295UL
#  endif

/* Minimum and maximum values a `signed long long int' can hold.  */
#   define LLONG_MAX    9223372036854775807LL
#   define LLONG_MIN    (-LLONG_MAX - 1LL)

/* Maximum value an `unsigned long long int' can hold.  (Minimum is 0.)  */
#   define ULLONG_MAX   18446744073709551615ULL

#else
#warning No definitions for limits.h for this target.
#endif

#endif // ! SLC_MTA_LIMITS_H
