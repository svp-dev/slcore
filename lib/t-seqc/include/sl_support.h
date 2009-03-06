#ifndef SL_SUPPORT_H
#define SL_SUPPORT_H

struct sl_famdata {
  int ix;
  int be;
  int li;
  int st;
  struct sl_famdata *ch;
  int ex;
  void (*f)(struct sl_famdata*);
  void *a;
};

typedef struct sl_famdata *sl_family_t;
typedef int sl_place_t;

#define SVP_LOCAL 0

#define SVP_ENOERR 0
#define SVP_EBROKEN 1
#define SVP_EKILLED 2

#endif
