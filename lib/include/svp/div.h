//
// div.h: this file is part of the SL toolchain.
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

#ifndef SLC_SVP_DIV_H
# define SLC_SVP_DIV_H

#include <cstdint.h>

#ifdef USE_C_DIVIDE

#define divu(X, Y)					\
  ((__typeof__((X) / (Y)))				\
   ((uint64_t)(X) / (uint64_t)(Y)))

#define modu(X, Y)					\
  ((__typeof__((X) % (Y)))				\
   ((uint64_t)(X) % (uint64_t)(Y)))

#define divs(X, Y)					\
  ((__typeof__((X) / (Y)))				\
   ((int64_t)(X) / (int64_t)(Y)))

#define mods(X, Y)					\
  ((__typeof__((X) % (Y)))				\
   ((int64_t)(X) % (int64_t)(Y)))

#define divmodu(X, Y)						\
  do {								\
    uint64_t __divmod_dividend = (X), __divmod_divisor = (Y);	\
    uint64_t __divmod_mod = __divmod_dividend % __divmod_divisor;	\
    uint64_t __divmod_res = __divmod_dividend / __divmod_divisor;	\
    (X) = (__typeof__(X)) __divmod_mod;					\
    (Y) = (__typeof__(Y)) __divmod_res;					\
  } while(0)

#define divmods(X, Y)				\
  do {							     \
    int64_t __divmod_dividend = (X), __divmod_divisor = (Y); \
    int64_t __divmod_mod = __divmod_dividend % __divmod_divisor;	\
    int64_t __divmod_res = __divmod_dividend / __divmod_divisor;	\
    (X) = (__typeof__(X)) __divmod_mod;					\
    (Y) = (__typeof__(Y)) __divmod_res;					\
  } while(0)

#else

sl_decl(__divmodqu, void, sl_shparm(uint64_t, x), sl_shparm(uint64_t, y));
sl_decl(__divmodqs, void, sl_shparm(int64_t, x), sl_shparm(int64_t, y));

#define divmodu(X, Y)				  \
  do {						  \
    uint64_t __dmu_x = (X);			  \
    uint64_t __dmu_y = (Y);			  \
    sl_create(,,,,,,, __divmodqu,		  \
	      sl_sharg(uint64_t, __dmu_sx, __dmu_x),	\
	      sl_sharg(uint64_t, __dmu_sy, __dmu_y));	\
    sl_sync();						\
    (X) = sl_geta(__dmu_sx);				\
    (Y) = sl_geta(__dmu_sy);				\
  } while(0)

#define divmods(X, Y)				  \
  do {						  \
    int64_t __dmu_x = (X);			  \
    int64_t __dmu_y = (Y);			  \
    sl_create(,,,,,,, __divmodqs,		  \
	      sl_sharg(int64_t, __dmu_sx, __dmu_x),	\
	      sl_sharg(int64_t, __dmu_sy, __dmu_y));	\
    sl_sync();						\
    (X) = sl_geta(__dmu_sx);				\
    (Y) = sl_geta(__dmu_sy);				\
  } while(0)

#define divu(X, Y) ({				\
      uint64_t __divu_x = (X), __divu_y = (Y);	\
      divmodu(__divu_x, __divu_y);		\
      (__typeof__((X) / (Y)))__divu_y;		\
    })

#define divs(X, Y) ({				\
    int64_t __divs_x = (X), __divs_y = (Y);	\
    divmods(__divs_x, __divs_y);		\
    (__typeof__((X) / (Y)))__divs_y;		\
  })

#define modu(X, Y) ({				\
    uint64_t __modu_x = (X), __modu_y = (Y);	\
    divmodu(__modu_x, __modu_y);		\
    (__typeof__((X) % (Y)))__modu_x;		\
  })

#define mods(X, Y) ({				\
    int64_t __mods_x = (X), __mods_y = (Y);	\
    divmods(__mods_x, __mods_y);		\
    (__typeof__((X) % (Y)))__mods_x;		\
  })

#endif

#endif // ! SLC_SVP_DIV_H
