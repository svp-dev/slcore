// utcimpl.h: this file is part of the utcc project.
//
// Copyright (C) 2008 The CSA group.
//
// $Id$
//
#ifndef UTC_OLDCORE_SIM_UTCIMPL_H
# define UTC_OLDCORE_SIM_UTCIMPL_H

#define _UTI_kill(Fid) __asm__ volatile("kill %0" : : "r"(Fid))
typedef long long _UTI_family_t;
typedef unsigned int _UTI_place_t;
#define _UTI_place_local 0
#define _UTI_enormal 0
#define _UTI_ebroken 1
#define _UTI_ekilled 2

#endif // ! UTC_OLDCORE_SIM_UTCIMPL_H
