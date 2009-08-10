//
// div.c: this file is part of the slc project.
//
// Copyright (C) 2009 The SL project.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//
// $Id$
//

#include <svp/compiler.h>
#include <svp/abort.h>
#include <svp/div.h>

// algorithm courtesy of Michael Hicks

#define DIVCODEU(Type, Dividend, Divisor)				\
  Type divisor = (Divisor);						\
  Type dividend = (Dividend);						\
  Type answer = 0;							\
  unsigned long shiftcount = 1;						\
									\
  if (unlikely(!divisor)) svp_abort();					\
  while (likely((divisor & (1ULL << ((sizeof(Type) << 3) - 1))) == 0) ) { \
    divisor <<= 1;							\
    ++shiftcount;							\
  }									\
									\
  while (likely(shiftcount > 0)){					\
    answer <<= 1;							\
    if (divisor <= dividend){						\
      dividend -= divisor;						\
      ++answer;								\
    }									\
    divisor >>= 1;							\
    --shiftcount;							\
  }									\
									\
  (Divisor) = answer;							\
  (Dividend) = dividend;

#define DIVCODES(Type, Dividend, Divisor)				\
  Type divisor = (Divisor);						\
  Type dividend = (Dividend);						\
  Type answer = 0;							\
  unsigned long shiftcount = 1;						\
  long  dendsign = 1, invsign = -1;					\
  if (unlikely(!divisor)) svp_abort();					\
  if (divisor > 0) { invsign = -invsign;				\
    divisor = -divisor; }						\
  if (dividend > 0) { dendsign = -dendsign;				\
    invsign = -invsign; dividend = -dividend; }				\
  while ( likely((divisor & (1LL << ((sizeof(Type) << 3) - 2))) != 0) ){ \
    divisor <<= 1;							\
    ++shiftcount;							\
  }									\
  while (likely(shiftcount > 0)){					\
    answer <<= 1;							\
    if (divisor >= dividend){						\
      dividend -= divisor;						\
      --answer;								\
    }									\
    divisor >>= 1;							\
    --shiftcount;							\
  }									\
  (Divisor) = answer * invsign;						\
  (Dividend) = dividend * dendsign;

#define DIVFUNCU(Name, Type)					\
  sl_def(Name, void, sl_shparm(Type, x), sl_shparm(Type, y))	\
  {								\
    Type x1 = sl_getp(x), y1 = sl_getp(y);			\
    DIVCODEU(Type, x1, y1);					\
    sl_setp(x, x1);						\
    sl_setp(y, y1);						\
  }								\
  sl_enddef

#define DIVFUNCS(Name, Type)					\
  sl_def(Name, void, sl_shparm(Type, x),sl_shparm(Type, y))	\
  {								\
    Type x1 = sl_getp(x), y1 = sl_getp(y);			\
    DIVCODES(Type, x1, y1);					\
    sl_setp(x, x1);						\
    sl_setp(y, y1);						\
  }								\
  sl_enddef

DIVFUNCU(__divmodqu, uint64_t)
DIVFUNCU(__divmodlu, uint32_t)
DIVFUNCS(__divmodqs, int64_t)
DIVFUNCS(__divmodls, int32_t)
