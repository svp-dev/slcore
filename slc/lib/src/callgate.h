//
// callgate.h: this file is part of the SL toolchain.
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

#ifndef SLC_CALLGATE_H
# define SLC_CALLGATE_H

#ifdef __mt_freestanding__
typedef union {
  unsigned long u;
  long s;
  void *p;
} mixedint;

struct callinfo {
  double farg5;
  mixedint arg5;
  double farg4;
  mixedint arg4;
  double farg3;
  mixedint arg3;
  double farg2;
  mixedint arg2;
  double farg1;
  mixedint arg1;
  double farg0;
  mixedint arg0;
  union { double retf; void *fptr; } head1;
  mixedint head0;
};

extern void __sl_callgate(void);

#define CALL_WITH_INFO(Info, Place)						\
  do {									\
    long __fid;								\
    __asm__ __volatile__("allocate %0, 0, 0, 0, 0\n"			\
			 "\tsetstart %0, %2\n"				\
			 "\tsetlimit %0, %3\n"				\
			 "\tcrei %0, 0(%4)\n"				\
			 "\tmov %0, $31 #SYNC\n"			\
			 : "=&r"(__fid)					\
			 : "0"(Place),					\
			   "rI"((&(Info))+1),				\
			   "rI"(((char*)(void*)((&(Info))+1))+1),	\
			   "r"(__sl_callgate)				\
			 : "memory");					\
  } while(0);
#endif

#endif // ! SLC_CALLGATE_H
