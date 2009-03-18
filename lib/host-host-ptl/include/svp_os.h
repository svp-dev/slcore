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

#include <iostream>
#include <cstdlib>

#define __write1(C) do { std::cout.put(C); std::cout.flush(); } while(0)
#define __abort() std::abort()
#define __nop() __asm__ __volatile__("/* NO OP */")

#endif // ! SLC_SVP_OS_H
