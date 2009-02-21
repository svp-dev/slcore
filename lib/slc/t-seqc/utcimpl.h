// utcimpl.h: this file is part of the utcc project.
//
// Copyright (C) 2008 The CSA group.
//
// $Id$
//
#ifndef UTC_SEQ_C_UTCIMPL_H
# define UTC_SEQ_C_UTCIMPL_H

struct ut_famdata {
  int ix;
  int be;
  int li;
  int st;
  struct ut_famdata *ch;
  int ex;
  void (*f)(struct ut_famdata*);
  void *a;
};

#define _UTI_kill(Fid) do {					\
    struct ut_famdata *__p;					\
    for (__p = (Fid); __p; __p = __p->ch)			\
      __p->ex = EXIT_KILLED;					\
    if (__fam->ex == EXIT_KILLED) return;			\
  } while(0)

typedef struct ut_famdata *_UTI_family_t;
#define _UTI_place_t int
#define _UTI_place_local 0
#define _UTI_enormal 0
#define _UTI_ebroken 1
#define _UTI_ekilled 2

#endif // ! UTC_SEQ_C_UTCIMPL_H
