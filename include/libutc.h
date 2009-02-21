// libutc.h: this file is part of the utcc project.
//
// Copyright (C) 2008 The CSA group.
//
// $Id$
//
#ifndef UTC_LIBUTC_H
# define UTC_LIBUTC_H

#include "utcimpl.h"

// common:
#define kill(Fid) _UTI_kill(Fid)

#define family _UTI_family_t
#define place _UTI_place_t

#define PLACE_LOCAL _UTI_place_local
#define EXIT_NORMAL _UTI_enormal
#define EXIT_BROKEN _UTI_ebroken
#define EXIT_KILLED _UTI_ekilled

#endif // ! UTC_LIBUTC_H
