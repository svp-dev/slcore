#ifndef SL_SUPPORT_H
#define SL_SUPPORT_H

#define __sl_getshp(Name)						\
  ({									\
    __asm__ __volatile__("# MT: read shared " # Name " (%0)" : : "rf"(__sl_shparm_in_ ## Name)); \
    __sl_shparm_in_ # Name;						\
  })
#define __sl_setshp(Name, Value)					\
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

#define __sl_getsha(Name) __sl_sharg_ ## Name
#define __sl_setsha(Name, Val) do {				\
    __sl_sharg_ ## Name = (Val);				\
    __asm__ __volatile__("# MT: shared " # Name " ready (%0)"	\
			 : "=r"(__sl_sharg_ ## Name)		\
			 : "0"(__sl_sharg_ ## Name));		\
  } while(0)

#define __sl_declindex(Name) register const long long Name = __sl_index

#define __sl_prologue(Name, GI, SI, GF, SF, GPREG, IDXREG, IPVREG, ITLSREG, PVREG, TLSREG, LN) \
  extern void Name(void);						\
  void _x_ ## Name (void) {						\
    register long long __sl_index __asm__(IDXREG);			\
    __asm__ ("# MT: index starts in %0 (must be =" IDXREG ")" : "=r"(__sl_index)); \
    __asm__ __volatile__(".end _x_" #Name "\n"			\
			 "\n"						\
			 "\t.globl " #Name "\n"				\
			 "\t.ent " #Name "\n"				\
			 #Name ":\n"					\
			 "\t.registers " # GI " " # SI " 19 " # GF " " # SF " 19\n" \
			 "\tldah " GPREG ",0(" IPVREG ")\t!gpdisp!" # LN "\n" \
			 "\tlda " GPREG ",0(" GPREG ")\t!gpdisp!" # LN "\n" \
			 "$" # Name "..ng:\n"				\
			 "\tmov " IPVREG "," PVREG " # PV <- Gx (call convention)\n"	\
			 "\tmov " ITLSREG "," TLSREG " # SP <- L1 (thread init)\n" \
			 "\t.prologue 1\n");				\

#define __sl_epilogue(Name, ArgSpec)						\
  __asm__ __volatile__("# MT: end of thread " #Name "()\n"		\
		       "\tnop\n"					\
		       "\tend\n"					\
		       "# RENAME_END " # Name " " ArgSpec);		\
  }									

#define __sl_allocate(Tag, GI, SI, GF, SF)				\
  register long long __sl_fid_ ## Tag;					\
  __asm__ __volatile__ ("allocate %0, %1, %2, %3, %4\t# CREATE " #Tag \
			: "=r"(__sl__fid_ ## Tag)		      \
			: "I"(GI), "I"(SI), "I"(GF), "I"(SF))

#define __sl_setstart(Tag, Start) \
  __asm__ ("setstart %0, %2\t# CREATE " #Tag		\
	   : "=r"(__sl_fid_ ## Tag)			\
	   : "0"(__sl_fid_ ## Tag), "rI"(Start))

#define __sl_setlimit(Tag, Limit) \
  __asm__ ("setlimit %0, %2\t# CREATE " #Tag \
	   : "=r"(__sl_fid_ ## Tag) \
	   : "0"(__sl_fid_ ## Tag), "rI"(Limit))

#define __sl_setplace(Tag, Place) \
  __asm__ ("setplace %0, %2\t# CREATE " #Tag \
	   : "=r"(__sl_fid_ ## Tag) \
	   : "0"(__sl_fid_ ## Tag), "rI"(Place))

#define __sl_setstep(Tag, Step) \
  __asm__ ("setstep %0, %2\t# CREATE " #Tag \
	   : "=r"(__sl_fid_ ## Tag) \
	   : "0"(__sl_fid_ ## Tag), "rI"(Step))

#define __sl_setblock(Tag, Block)					\
  __asm__ ("setblock %0, %2\t# CREATE " #Tag \
	   : "=r"(__sl_fid_ ## Tag) \
	   : "0"(__sl_fid_ ## Tag), "rI"(Block))

#define __sl_dobreak(Val) do {			    \
    __asm__ __volatile__("break %0" : : "rI"(Val)); \
    while(1) __nop();                               \
  } while(0)

#define __sl_dobreakf(Val) do {			    \
    __asm__ __volatile__("breakf %0" : : "r"(Val)); \
    while(1) __nop();                               \
  } while(0)

#define __sl_dokill(Val) __asm__ __volatile__("kill %0" : : "r"(Val))

#define __sl_declshp(Type, Name, Reg1, Reg2) \
  register Type const __sl_shparm_in_ ## Name __asm__(Reg1); \
  register Type const __sl_shparm_out_ ## Name __asm__(Reg2)

#define __sl_declglp(Type, Name, Reg) \
  register Type const __sl_glparm_ ## Name __asm__(Reg)

#define __sl_declgla_noreg(Type, Name, Init)		\
  register Type const __sl_glarg_ ## Name = (Init)

#define __sl_declgla(Type, Name, Reg, Init)			\
  register Type const __sl_glarg_ ## Name __asm__(Reg) = (Init)

#define __sl_declsha_empty(Type, Name, Reg) \
  register Type __sl_sharg_ ## Name __asm__(Reg); \
  __asm__ ("setempty %0 # EMPTY " # Name : "=r"(__sl_sharg_ ## Name))

#define __sl_declsha(Type, Name, Reg, Init)		\
  register Type __sl_sharg_ ## Name __asm__(Reg) = (Init)

#define __sl_setbreak(Tag, BRI) \
  __asm__ ("setbreak %0, %1\t# CREATE " # Tag \
	   : "=r"(__sl_fid_ ## Tag) \
	   : "0"(__sl_fid_ ## Tag) : "I"(BRI))

#define __sl_setbreakf(Tag, BRI) \
  __asm__ ("setbreakf %0, %1\t# CREATE " # Tag \
	   : "=r"(__sl_fid_ ## Tag) \
	   : "0"(__sl_fid_ ## Tag) : "I"(BRI))

#define __sl_declbr(Tag, Type, Reg)		\
  register Type const __sl_br_ ## Tag __asm__(Reg)

typedef long long sl_family_t;
typedef long long sl_place_t;
typedef void (*__sl_fptr_t)(void);

#define SVP_LOCAL 0

#define SVP_ENOERR 0
#define SVP_EBROKEN 1
#define SVP_EKILLED 2

#endif
