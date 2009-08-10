//
// svp_asmdiv.h: this file is part of the SL toolchain.
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

#ifndef SLC_SVP_ASMDIV_H
# define SLC_SVP_ASMDIV_H

extern void __divmodqu(void);
extern void __divmodqs(void);

#ifndef RL0
#define RL0 "$0"
#define RL1 "$1"
#define RL2 "$2"
#endif

#define __ASM_DIVCALL(T, Ptr, X, Y)					\
  do {									\
    register T __arg0 __asm__(RL0) = (X);				\
    register T __arg1 __asm__(RL1) = (Y);				\
    __asm__ ("allocate " RL2 ", 0, 0, 0, 0\n"				\
	     "\t# SETREGS not necessary when (%0=" RL0") and (%1=" RL1 ")\n" \
	     "\tcrei " RL2 ",0(%4)\n"					\
	     "\tbis $31, " RL2 ", $31\n\tswch"				\
	     : "=r"(__arg0), "=r"(__arg1)				\
	     : "0"(__arg0), "1"(__arg1), "r"(Ptr) : RL2);		\
    (X) = __arg0; (Y) = __arg1;						\
  } while(0)

#define __asm_divmodu(X, Y) __ASM_DIVCALL(unsigned long long, __divmodqu, X, Y)
#define __asm_divmods(X, Y) __ASM_DIVCALL(signed long long, __divmodqs, X, Y)

#endif // ! SLC_SVP_ASMDIV_H
