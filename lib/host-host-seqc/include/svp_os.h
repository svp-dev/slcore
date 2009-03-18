//                                                             -*- C++ -*-
// svp_os.h: this file is part of the slc project.
//
// Copyright (C) 2009 The SL project.
// All rights reserved.
//
// $Id$
//
#ifndef SLC_SVP_OS_H
# define SLC_SVP_OS_H

#include <stdio.h>
#include <stdlib.h>

#define __write1(C) putchar(C)
#define __abort() abort()
#define __nop() __asm__ __volatile__("/* NO OP */")

#endif // ! SLC_SVP_OS_H
