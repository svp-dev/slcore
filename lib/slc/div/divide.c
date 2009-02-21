#include "utc2.h"

// algorithm courtesy of Michael Hicks
#define likely(x)       __builtin_expect((x), 1)
#define unlikely(x)     __builtin_expect((x), 0)
#define halt while (1) __asm__ volatile(".int 0x74000000")

#define DIVCODEU(T, DividendMod, DivisorResult)			\
  T divisor = (DivisorResult);					\
  T dividend = (DividendMod);					\
  T answer = 0;							\
  unsigned long long shiftcount = 1;				\
  								\
  if (unlikely(!divisor)) halt;				\
  while ( likely((divisor & (1ULL << ((sizeof(T) << 3) - 1))) == 0) ){	\
    divisor <<= 1;						\
    ++shiftcount;						\
  }								\
  								\
  while (likely(shiftcount > 0)){				\
    answer <<= 1;						\
    if (divisor <= dividend){					\
      dividend -= divisor;					\
      ++answer;							\
    }								\
    divisor >>= 1;						\
    --shiftcount;						\
  }								\
  								\
  (DivisorResult) = answer;					\
  (DividendMod) = dividend; 


#define DIVCODES(T, DividendMod, DivisorResult)				\
  T divisor = (DivisorResult);						\
  T dividend = (DividendMod);						\
  T answer = 0;								\
  unsigned long long shiftcount = 1;					\
  long long dendsign = 1, invsign = -1;						\
  if (unlikely(!divisor)) halt;						\
  if (divisor > 0) { invsign = -invsign;				\
    divisor = -divisor; }						\
  if (dividend > 0) { dendsign = -dendsign;				\
    invsign = -invsign; dividend = -dividend; }				\
  while ( likely((divisor & (1LL << ((sizeof(T) << 3) - 2))) != 0) ){	\
    divisor <<= 1;							\
    ++shiftcount;							\
  }									\
  while (likely(shiftcount > 0)){					\
    answer <<= 1;							\
    if (divisor >= dividend){						\
      dividend -= divisor;						\
      --answer;								\
    }									\
    divisor >>= 1;							\
    --shiftcount;							\
  }									\
  (DivisorResult) = answer * invsign;					\
  (DividendMod) = dividend * dendsign;					




#define DIVFUNCU(Name, T)			\
  BEGIN_FUNC(Name)				\
  {						\
    SHARG0(T, x);				\
    SHARG1(T, y);				\
    T x1 = GETSH(x), y1 = GETSH(y);		\
    DIVCODEU(T, x1, y1);			\
    SETSH(x, x1);				\
    SETSH(y, y1);				\
  }						\
  END_FUNC(Name)

#define DIVFUNCS(Name, T)			\
  BEGIN_FUNC(Name)				\
  {						\
    SHARG0(T, x);				\
    SHARG1(T, y);				\
    T x1 = GETSH(x), y1 = GETSH(y);		\
    DIVCODES(T, x1, y1);			\
    SETSH(x, x1);				\
    SETSH(y, y1);				\
  }						\
  END_FUNC(Name)

DIVFUNCU(__divmodqu, unsigned long long)
DIVFUNCU(__divmodlu, unsigned int)
DIVFUNCS(__divmodqs, signed long long)
DIVFUNCS(__divmodls, signed int)
