// debugimpl.h: this file is part of the utcc project.
//
// Copyright (C) 2008 The CSA group.
//
// $Id$
//
#ifndef UTC_OLDCORE_SIM_DEBUGIMPL_H
# define UTC_OLDCORE_SIM_DEBUGIMPL_H

#define d_putchar(X) __asm__ volatile("print %0,1"::"r"(X))
#define d_abort() while (1) __asm__ volatile(".int 0x74000000")
#define d_nop() __asm__ volatile("nop")

extern void __divmodqu(void);
extern void __divmodlu(void);
extern void __divmodls(void);
extern void __divmodqs(void);

#define RL0 "$0"
#define RL1 "$1"
#define RL2 "$2"
#define PHYS_ZERO "$31"

#ifndef DIRECT_DIV_FUNCTIONS

#define DIVFUNC(T, Ptr, X, Y)						\
  do {									\
    register T __arg0 __asm__(RL0) = (X);			\
    register T __arg1 __asm__(RL1) = (Y);			\
    __asm__ ("allocate " RL2 ", 0, 0, 0, 0\n"					\
	    "\t# SETREGS not necessary when (%0=" RL0") and (%1=" RL1 ")\n" \
	    "\tcrei " RL2 ",0(%4)\n"				\
	    "\tbis " PHYS_ZERO ", " RL2 ", " PHYS_ZERO "\n\tswch"	\
	    : "=r"(__arg0), "=r"(__arg1)				\
	    : "0"(__arg0), "1"(__arg1), "r"(Ptr) : RL2);		\
    (X) = __arg0; (Y) = __arg1;						\
  } while(0)

#else

#define DIVFUNC(T, Ptr, X, Y)						\
  do {									\
    register T __arg0 __asm__(RL0) = (X);			\
    register T __arg1 __asm__(RL1) = (Y);			\
    __asm__ ("allocate " RL2 ", 0, 0, 0, 0\n"					\
	    "\t# SETREGS not necessary when (%0=" RL0") and (%1=" RL1 ")\n" \
	    "\tcred " RL2 "," # Ptr "\n"			\
	    "\tbis " PHYS_ZERO ", " RL2 ", " PHYS_ZERO "\n\tswch"	\
	    : "=r"(__arg0), "=r"(__arg1)				\
	    : "0"(__arg0), "1"(__arg1) : RL2);				\
    (X) = __arg0; (Y) = __arg1;						\
  } while(0)
#endif

#define d_divmodu64(X, Y) DIVFUNC(unsigned long long, __divmodqu, X, Y)
#define d_divmodu32(X, Y) DIVFUNC(unsigned int, __divmodlu, X, Y)
#define d_divmods64(X, Y) DIVFUNC(signed long long, __divmodqs, X, Y)
#define d_divmods32(X, Y) DIVFUNC(signed int, __divmodls, X, Y)

#endif // ! UTC_OLDCORE_SIM_DEBUGIMPL_H
