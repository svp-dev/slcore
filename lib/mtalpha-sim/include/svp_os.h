//                                                             -*- C++ -*-
//
// svp_os.h: this file is part of the slc project.
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
// $Id$
//

#ifndef SLC_SVP_OS_H
# define SLC_SVP_OS_H

#define __write1(C) __asm__ __volatile__("print %0, 1" : : "r"(C))
#define __abort() while(1) __asm__ __volatile__("nop\n\tnop\n\tpal1d 0")
#define __nop() __asm__ __volatile__("nop\n\tswch")

#endif // ! SLC_SVP_OS_H
