//
// divide.h: this file is part of the SL toolchain.
//
// Copyright (C) 2016 The SL project.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// The complete GNU General Public Licence Notice can be found as the
// `COPYING' file in the root directory.
//
#ifndef SVP_DIVIDE_H
#define SVP_DIVIDE_H

#include <svp/compiler.h>
#include <svp/abort.h>
#include <stdint.h>
#include <stddef.h>

#if !defined(DIVIDE_IMPL)
#define EXTERNDIV extern
#else
#define EXTERNDIV /*nothing*/
#endif

#if (defined(__slc_arch_leon2mt__) || defined(__slc_arch_leon2__)) && defined(__slc_os_fpga__)

#define DIVCODEU(Type)							\
    __attribute__((gnu_inline,optimize(3)))				\
    EXTERNDIV inline void __divmodu_##Type(Type dividend, Type divisor, Type *result, Type*mod) \
    {									\
	Type answer = 0;						\
	unsigned long shiftcount = 1;					\
									\
	if (unlikely(!divisor)) svp_abort();				\
	while (likely((divisor & (1ULL << ((sizeof(Type) << 3) - 1))) == 0) ) { \
	    divisor <<= 1;						\
	    ++shiftcount;						\
	}								\
									\
	while (likely(shiftcount > 0)){					\
	    answer <<= 1;						\
	    if (divisor <= dividend){					\
		dividend -= divisor;					\
		++answer;						\
	    }								\
	    divisor >>= 1;						\
	    --shiftcount;						\
	}								\
									\
	if(result) *result = answer;					\
	if(mod) *mod = dividend;					\
    }									\
    __attribute__((gnu_inline,pure,optimize(3)))			\
    EXTERNDIV inline Type __divu_##Type(Type dividend, Type divisor)		\
    {									\
	Type result;							\
	__divmodu_##Type(dividend, divisor, &result, 0);		\
	return result;							\
    }									\
    __attribute__((gnu_inline,pure,optimize(3)))			\
    EXTERNDIV inline Type __modu_##Type(Type dividend, Type divisor)		\
    {									\
	Type mod;							\
	__divmodu_##Type(dividend, divisor, 0, &mod);			\
	return mod;							\
    } 

#define DIVCODES(Type)							\
    __attribute__((gnu_inline,optimize(3)))				\
    EXTERNDIV inline void __divmods_##Type(Type dividend, Type divisor, Type *result, Type *mod) \
    {									\
	Type answer = 0;						\
	unsigned long shiftcount = 1;					\
	long  dendsign = 1, invsign = -1;				\
	if (unlikely(!divisor)) svp_abort();				\
	if (divisor > 0) { invsign = -invsign;				\
	    divisor = -divisor; }					\
	if (dividend > 0) { dendsign = -dendsign;			\
	    invsign = -invsign; dividend = -dividend; }			\
	while ( likely((divisor & (1LL << ((sizeof(Type) << 3) - 2))) != 0) ){ \
	    divisor <<= 1;						\
	    ++shiftcount;						\
	}								\
	while (likely(shiftcount > 0)){					\
	    answer <<= 1;						\
	    if (divisor >= dividend){					\
		dividend -= divisor;					\
		--answer;						\
	    }								\
	    divisor >>= 1;						\
	    --shiftcount;						\
	}								\
	if(result) *result = answer * invsign;				\
	if(mod) *mod = dividend * dendsign;				\
    }									\
    __attribute__((gnu_inline,pure,optimize(3)))			\
    EXTERNDIV inline Type __divs_##Type(Type dividend, Type divisor)		\
    {									\
	Type result;							\
	__divmods_##Type(dividend, divisor, &result, 0);		\
	return result;							\
    }									\
    __attribute__((gnu_inline,pure,optimize(3)))			\
    EXTERNDIV inline Type __mods_##Type(Type dividend, Type divisor)		\
    {									\
	Type mod;							\
	__divmods_##Type(dividend, divisor, 0, &mod);			\
	return mod;							\
    } 
#else

#define __DIVCODE(L, Type)						\
    __attribute__((gnu_inline,optimize(3)))				\
    EXTERNDIV inline void __divmod##L##_##Type(Type dividend, Type divisor, Type *result, Type *mod) \
    {									\
	if(result) *result = dividend / divisor;			\
	if(mod) *mod = dividend % divisor;				\
    }									\
    __attribute__((gnu_inline,pure,optimize(3)))			\
    EXTERNDIV inline Type __div##L##_##Type(Type dividend, Type divisor)		\
    {									\
	return dividend / divisor;					\
    }									\
    __attribute__((gnu_inline,pure,optimize(3)))			\
    EXTERNDIV inline Type __mod##L##_##Type(Type dividend, Type divisor)		\
    {									\
	return dividend % divisor;					\
    }
#define DIVCODEU(Type) __DIVCODE(u, Type)
#define DIVCODES(Type) __DIVCODE(s, Type)
#endif

DIVCODEU(uint8_t)
DIVCODEU(uint16_t)
DIVCODEU(uint32_t)
DIVCODES(int8_t)
DIVCODES(int16_t)
DIVCODES(int32_t)
DIVCODEU(size_t)

#endif
