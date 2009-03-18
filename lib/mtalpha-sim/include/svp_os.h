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

#define __write1(C) __asm__ __volatile__("print %0, 1" : : "r"(C))
#define __abort() while(1) __asm__ __volatile__("nop\n\tnop\n\tpal1d 0")
#define __nop() __asm__ __volatile__("nop\n\tswch")

#endif // ! SLC_SVP_OS_H
