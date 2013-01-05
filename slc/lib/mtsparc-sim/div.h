#include <svp/compiler.h>
#include <svp/abort.h>

// algorithm courtesy of Michael Hicks

#define DIVCODEU(Type, Dividend, Divisor)				\
  Type divisor = (Divisor);						\
  Type dividend = (Dividend);						\
  Type answer = 0;							\
  unsigned long shiftcount = 1;						\
									\
  if (unlikely(!divisor)) svp_abort();					\
  while (likely((divisor & (1ULL << ((sizeof(Type) << 3) - 1))) == 0) ) { \
    divisor <<= 1;							\
    ++shiftcount;							\
  }									\
									\
  while (likely(shiftcount > 0)){					\
    answer <<= 1;							\
    if (divisor <= dividend){						\
      dividend -= divisor;						\
      ++answer;								\
    }									\
    divisor >>= 1;							\
    --shiftcount;							\
  }									\
									\
  (Divisor) = answer;							\
  (Dividend) = dividend;

#define DIVCODES(Type, Dividend, Divisor)				\
  Type divisor = (Divisor);						\
  Type dividend = (Dividend);						\
  Type answer = 0;							\
  unsigned long shiftcount = 1;						\
  long  dendsign = 1, invsign = -1;					\
  if (unlikely(!divisor)) svp_abort();					\
  if (divisor > 0) { invsign = -invsign;				\
    divisor = -divisor; }						\
  if (dividend > 0) { dendsign = -dendsign;				\
    invsign = -invsign; dividend = -dividend; }				\
  while ( likely((divisor & (1LL << ((sizeof(Type) << 3) - 2))) != 0) ){ \
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
  (Divisor) = answer * invsign;						\
  (Dividend) = dividend * dendsign;
