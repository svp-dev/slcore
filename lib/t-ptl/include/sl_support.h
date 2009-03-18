//                                                             -*- C++ -*-
// sl_support.h: this file is part of the slc project.
//
// Copyright (C) 2009 The SL project.
// All rights reserved.
//
// $Id$
//
#ifndef SLC_SL_SUPPORT_H
# define SLC_SL_SUPPORT_H

#ifdef USE_EXTERNAL_PTL
#include <uTC.h>
#else
#include "ptl_svp.h"
#endif

typedef uTC::family sl_family_t;
typedef uTC::place sl_place_t;

#define PLACE_DEFAULT uTC::PLACE_GROUP
#define PLACE_LOCAL   uTC::PLACE_LOCAL

#define SVP_ENOERR uTC::EXIT_NORMAL
#define SVP_EBROKEN uTC::EXIT_BREAK
#define SVP_EKILLED uTC::EXIT_KILL

#endif // ! SLC_SL_SUPPORT_H
