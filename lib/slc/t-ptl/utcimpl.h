// utcimpl.h: this file is part of the utcc project.
//
// Copyright (C) 2008 The CSA group.
//
// $Id$
//
#ifndef UTC_PTL_CPP_UTCIMPL_H
# define UTC_PTL_CPP_UTCIMPL_H

#include "ptl_svp.h"
#define _UTI_kill(Fid) uTC::kill(Fid)
#define _UTI_family_t uTC::family
#define _UTI_place_t uTC::place
#define _UTI_place_local uTC::PLACE_LOCAL
#define _UTI_enormal uTC::EXIT_NORMAL
#define _UTI_ebroken uTC::EXIT_BREAK
#define _UTI_ekilled uTC::EXIT_KILL

#endif // ! UTC_PTL_CPP_UTCIMPL_H
