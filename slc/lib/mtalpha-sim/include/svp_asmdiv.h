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

#define __ASM_DIVCALL(T, Ptr, X, Y)					\
  do {									\
      register T __arg0 = (X);                                          \
    register T __arg1 = (Y);                                            \
    register long __fid; \
    __asm__ ("allocate 8, %2\n"                                       \
	     "\tcrei %2, 0(%5)\n"					\
             "\tputs %3, %2, 0\n"                                       \
             "\tputs %4, %2, 1\n"                                       \
             "\tsync %2, %0\n"                                          \
             "\tmov %0, $31\n"                                          \
             "\tgets %2, 0, %0\n"                                       \
             "\tgets %2, 1, %1\n"                                       \
	     "\trelease %2"                                             \
	     : "=r"(__arg0), "=r"(__arg1), "=r"(__fid)                  \
	     : "r"(__arg0), "r"(__arg1), "r"(Ptr));                     \
    (X) = __arg0; (Y) = __arg1;						\
  } while(0)

#define __asm_divmodu(X, Y) __ASM_DIVCALL(unsigned long long, __divmodqu, X, Y)
#define __asm_divmods(X, Y) __ASM_DIVCALL(signed long long, __divmodqs, X, Y)

#endif // ! SLC_SVP_ASMDIV_H
