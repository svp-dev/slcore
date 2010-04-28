//
// sl_support.h: this file is part of the SL toolchain.
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

#ifndef SLC_SL_SUPPORT_H
# define SLC_SL_SUPPORT_H

[[#]]ifndef SL_SUPPORT_DEFINED
[[#]]define SL_SUPPORT_DEFINED

[[#]]define __sl_getshp(Name)						\
  ({									\
    __asm__ __volatile__("# MT: read shared " # Name " (%0)"		\
			 : "=rf"(__sl_shparm_in_ ## Name)		\
			 : "0"(__sl_shparm_in_ ## Name));		\
    __sl_shparm_in_ ## Name;						\
  })


[[#]]ifndef MT_[[]]FORCE_TEMP_FOR_SHARED_WRITE

[[#]]define __sl_setshp(Name, Value)                                  \
({                                                                      \
    __asm__ __volatile__("# MT: clobber incoming " # Name " (%0)"       \
                         : "=rf"(__sl_shparm_in_ ## Name)               \
                         : "0"(__sl_shparm_in_ ## Name));               \
    __typeof__(__sl_shparm_out_ ## Name) __tmp_set_ ## Name = (Value);  \
    __asm__ __volatile__("# MT: start write shared " # Name " (%0)"     \
                         : "=rf"(__sl_shparm_out_ ## Name)              \
                         : "0"(__sl_shparm_out_ ## Name));              \
    __sl_shparm_out_ ## Name = __tmp_set_ ## Name;                      \
    __asm__ __volatile__("# MT: end write shared " # Name " (%0)"       \
                         : "=rf" (__sl_shparm_out_ ## Name),            \
                           "=rf" (__sl_shparm_in_ ## Name)              \
                         : "0"(__sl_shparm_out_ ## Name),               \
                           "1" (__sl_shparm_in_ ## Name));              \
})

[[#]]else

[[#]]define __sl_setshp(Name, Value)            \
    ({                                                                  \
        __asm__ __volatile__("# MT: clobber incoming " # Name " (%0)"   \
                             : "=rf"(__sl_shparm_in_ ## Name)           \
                             : "0"(__sl_shparm_in_ ## Name));           \
        __typeof__(__sl_shparm_out_ ## Name) __tmp_set_ ## Name = (Value); \
        __asm__ __volatile__("# MT: start write shared " # Name " (%0)" \
                             : "=rf"(__sl_shparm_out_ ## Name)          \
                             : "0"(__sl_shparm_out_ ## Name));          \
        __asm__ __volatile__("mov %4, %0 # MT: write shared " # Name " (%0)" \
                             : "=rf" (__sl_shparm_out_ ## Name),        \
                               "=rf" (__sl_shparm_in_ ## Name)          \
                             : "0"(__sl_shparm_out_ ## Name),           \
                               "1" (__sl_shparm_in_ ## Name),           \
                               "rf" (__tmp_set_ ## Name));              \
    })
    
[[#]]endif

#if 0 /* OLD TECHNIQUE, BUG WITH sl_setp(foo, a ? a : 0) */
[[#]]define __sl_setshp(Name, Value)					\
  ({									\
    __typeof__(Value) __tmp_setsh = (Value);				\
    __asm__ __volatile__("# MT: start write shared " # Name " (%1) <- " # Value " (and clobber incoming %0)" \
			 : /* ensure incoming shared is marked clobbered */ \
			   "=rf" (__sl_shparm_in_ ## Name)		\
			 : "rf"(__sl_shparm_out_ ## Name));		\
    __sl_shparm_out_ ## Name = __tmp_setsh;				\
    __asm__ __volatile__("# MT: end write shared " # Name " (%0)"	\
			 : "=rf" (__sl_shparm_out_ ## Name)		\
			 : "0"(__sl_shparm_out_ ## Name));		\
  })
#endif

[[#]]define __sl_getsha(Name) __sl_sharg_ ## Name
[[#]]define __sl_setsha(Name, Val) do {				\
  __typeof__(Val) __tmp_set_ ## Name = (Val);			\
  __asm__ ("puts %0, %1, %2 # MT: writing shared " # Name " (%0)"	\
		       : "=rf"(__sl_arg_ ## Name)		\
		       : "0"(__sl_arg_ ## Name));		\
  __sl_arg_ ## Name = __tmp_set_ ## Name;			\
  __asm__ __volatile__("# MT: shared " # Name " ready (%0)"	\
		       : "=rf"(__sl_arg_ ## Name)		\
		       : "0"(__sl_arg_ ## Name));		\
 } while(0)


[[#]]define __sl_declindex(Name) register const long Name = __sl_index

[[#]]define __sl_prologue(Name, GI, SI, GF, SF, IDXREG, LN) \
  extern void Name(void);						\
  void __slf_ ## Name (void) {						\
    register long __sl_index __asm__(IDXREG);			\
    __asm__ ("# MT: index starts in %0 (must be =" IDXREG ")" : "=r"(__sl_index)); \
    __asm__ __volatile__(".registers " # GI " " # SI " 19 " # GF " " # SF " 19\n");

[[#]]define __sl_epilogue(Name, ArgSpec)		\
  __sl_end_thread: (void)0; }

[[#]]define __sl_allocate(Tag, Place)			\
  register long __sl_fid_ ## Tag;					\
  __asm__ __volatile__ ("allocate %1, %0\t# MT: CREATE" #Tag	\
			: "=r"(__sl_fid_ ## Tag)			\
			: "rI"(Place))

[[#]]define __sl_setstart(Tag, Start) \
  __asm__ ("setstart %0, %2\t# MT: CREATE " #Tag		\
	   : "=r"(__sl_fid_ ## Tag)			\
	   : "0"(__sl_fid_ ## Tag), "rI"(Start))

[[#]]define __sl_setlimit(Tag, Limit) \
  __asm__ ("setlimit %0, %2\t# MT: CREATE " #Tag \
	   : "=r"(__sl_fid_ ## Tag) \
	   : "0"(__sl_fid_ ## Tag), "rI"(Limit))

[[#]]define __sl_setstep(Tag, Step) \
  __asm__ ("setstep %0, %2\t# MT: CREATE " #Tag \
	   : "=r"(__sl_fid_ ## Tag) \
	   : "0"(__sl_fid_ ## Tag), "rI"(Step))

[[#]]define __sl_setblock(Tag, Block)					\
  __asm__ ("setblock %0, %2\t# MT: CREATE " #Tag \
	   : "=r"(__sl_fid_ ## Tag) \
	   : "0"(__sl_fid_ ## Tag), "rI"(Block))

[[#]]define __sl_setshfa(Name, Tag, Val, Offset, Type)                     \
  __asm__ ("fputs %2, %0, " Offset "\t# MT: set shared " # Name : "=r"(__sl_fid_ ## Tag) : "0"(__sl_fid_ ## Tag), "f"((Type)(Val)))

[[#]]define __sl_setshia(Name, Tag, Val, Offset, Type)                     \
  __asm__ ("puts %2, %0, " Offset "\t# MT: set shared " # Name : "=r"(__sl_fid_ ## Tag) : "0"(__sl_fid_ ## Tag), "rI"((Type)(Val)))

[[#]]define __sl_setma(Tag, Offset) \
  __asm__ ("wmb; putg %2, %0, " Offset "\t# MT: set offset for memargs" : "=r"(__sl_fid_ ## Tag) : "0"(__sl_fid_ ## Tag), "r"(&__sl_ma_data_ ## Tag))

[[#]]define __sl_setglfa(Name, Tag, Val, Offset, Type)                     \
  __asm__ ("fputg %2, %0, " Offset "\t# MT: set global " # Name : "=r"(__sl_fid_ ## Tag) : "0"(__sl_fid_ ## Tag), "f"(__sl_after_ ## Name = (Val)))

[[#]]define __sl_setglia(Name, Tag, Val, Offset, Type)                     \
  __asm__ ("putg %2, %0, " Offset "\t# MT: set global " # Name : "=r"(__sl_fid_ ## Tag) : "0"(__sl_fid_ ## Tag), "rI"(__sl_after_ ## Name = (Val)))

[[#]]define __sl_getshfa(Name, Tag, Offset)                          \
  __asm__ ("fgets %0, " Offset ", %1; fmov %1, %1\t# MT: get shared " # Name : "=r"(__sl_fid_ ## Tag), "=f"(__sl_after_ ## Name) : "0"(__sl_fid_ ## Tag)); 

[[#]]define __sl_getshia(Name, Tag, Offset)                          \
  __asm__ ("gets %0, " Offset ", %1; mov %1, %1\t# MT: get shared " # Name : "=r"(__sl_fid_ ## Tag), "=r"(__sl_after_ ## Name) : "0"(__sl_fid_ ## Tag))

[[#]]define __sl_create(Tag)                                         \
  __asm__ __volatile__("wmb; crei %0, 0(%2)\t# MT: CREATE " # Tag : "=r"(__sl_fid_ ## Tag) : "0"(__sl_fid_ ## Tag), "r"(__sl_funcptr_ ## Tag) : "memory")

[[#]]define __sl_sync_nouse(Tag)                                       \
  __asm__("sync %0, %1; mov %1, $31\t# MT: SYNC " # Tag : "=r"(__sl_fid_ ## Tag), "=r"(__sl_syncvar_ ## Tag) : "0"(__sl_fid_ ## Tag) : "memory")

[[#]]define __sl_sync(Tag, Var)                                           \
  __asm__("sync %0, %1; mov %1, $31\t# MT: SYNC " # Tag : "=r"(__sl_fid_ ## Tag), "=r"(Var) : "0"(__sl_fid_ ## Tag) : "memory")

[[#]]define __sl_release(Tag) \
  __asm__ __volatile__("release %0" : : "r"(__sl_fid_ ## Tag));



[[#]]define __sl_dobreak(Val) do {			    \
    __asm__ __volatile__("break %0" : : "rI"(Val)); \
    while(1) __nop();                               \
  } while(0)

[[#]]define __sl_dobreakf(Val) do {			    \
    __asm__ __volatile__("breakf %0" : : "f"(Val)); \
    while(1) __nop();                               \
  } while(0)

[[#]]define __sl_dokill(Val) __asm__ __volatile__("kill %0" : : "r"(Val))

[[#]]define __sl_declshp(Type, Name, Reg1, Reg2) \
  register Type __sl_shparm_in_ ## Name __asm__(Reg1); \
  register Type __sl_shparm_out_ ## Name __asm__(Reg2)

[[#]]define __sl_declglp_r(Type, Name, Reg) \
  register Type const __sl_glparm_ ## Name __asm__(Reg)

[[#]]define __sl_declglp_w(Type, Name, Reg) \
  register Type const __sl_glparmr_ ## Name __asm__(Reg);	\
  register Type __sl_glparm_ ## Name = __sl_glparmr_ ## Name \

[[#]]define __sl_declgla_noreg(Type, Name, Init)		\
  register Type const __sl_arg_ ## Name = (Init)

[[#]]define __sl_declgla_noreg_undef(Type, Name)		\
  register Type __sl_arg_ ## Name

[[#]]define __sl_declgla(Type, Name, Reg, Init)			\
  register Type const __sl_arg_ ## Name __asm__(Reg) = (Init)

[[#]]define __sl_declsha_empty(Type, Name, Reg) \
  register Type __sl_arg_ ## Name __asm__(Reg); \
  __asm__ ("setempty %0 # EMPTY " # Name : "=rf"(__sl_arg_ ## Name))

// : "0"(__sl_sharg_ ## Name))

[[#]]define __sl_declsha(Type, Name, Reg, Init)		\
  register Type __sl_arg_ ## Name __asm__(Reg) = (Init)

[[#]]define __sl_setbreak(Tag, BRI) \
  __asm__ ("setbreak %0, %1\t# CREATE " # Tag \
	   : "=r"(__sl_fid_ ## Tag) \
	   : "0"(__sl_fid_ ## Tag) : "I"(BRI))

[[#]]define __sl_setbreakf(Tag, BRI) \
  __asm__ ("setbreakf %0, %1\t# CREATE " # Tag \
	   : "=r"(__sl_fid_ ## Tag) \
	   : "0"(__sl_fid_ ## Tag) : "I"(BRI))

[[#]]define __sl_declbr(Tag, Type, Reg)		\
  register Type const __sl_br_ ## Tag __asm__(Reg)

extern void _sl_callgate(void);

typedef long sl_family_t;
typedef long sl_place_t;
typedef void (*__sl_fptr_t)(void);

[[#]]define PLACE_LOCAL   0xA /* 4 = local, 8 = suspend */
[[#]]define PLACE_DEFAULT 0x8 /* 8 = suspend */

[[#]]define SVP_ENOERR 0
[[#]]define SVP_EBROKEN 1
[[#]]define SVP_EKILLED 2

[[#]]endif


#endif // ! SLC_SL_SUPPORT_H
