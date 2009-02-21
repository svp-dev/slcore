#include "utc2.h"

static const char *__basechars = "0123456789abcdef";
#define d_putchar(C) __asm__ volatile("print %0,1"::"r"(C))

#define idivq(X, Y)				\
  ({						\
    unsigned long long __dividendmod = (X);			\
    unsigned long long __divisorres = (Y);			\
    divmodqu(__dividendmod, __divisorres); \
    __divisorres; \
  })

#define imodq(X, Y)				\
  ({						\
    unsigned long long __dividendmod = (X);			\
    unsigned long long __divisorres = (Y);			\
    divmodqu(__dividendmod, __divisorres); \
    __dividendmod; \
  })

#define idiv(X, Y)				\
  ({						\
    unsigned __dividendmod = (X);			\
    unsigned __divisorres = (Y);			\
    divmodlu(__dividendmod, __divisorres); \
    __divisorres; \
  })

#define imod(X, Y)				\
  ({						\
    unsigned __dividendmod = (X);			\
    unsigned __divisorres = (Y);			\
    divmodlu(__dividendmod, __divisorres); \
    __dividendmod; \
  })

#define d_n(X) do { \
    unsigned __blah = (X); \
    while (__blah--) d_putchar('.'); \
    d_putchar('\n'); \
  } while(0)

#define d_putqu(X, T) do {					\
    unsigned T __x = (unsigned T)(X);				\
    if (__x < 10) d_putchar(__basechars[__x]);			\
    else {							\
      unsigned T __root = 1;					\
      while (idivq(__x, __root) >= 10) __root *= 10;		\
      while (__root) {						\
	unsigned __rel = idivq(__x, __root);			\
	d_putchar(__basechars[__rel]);			\
	__x = imodq(__x, __root);			\
	__root = idivq(__root, 10);			\
      }                                                 \
    }                                                   \
  } while(0)

#define d_putlu(X, T) do {					\
    unsigned T __x = (unsigned T)(X);				\
    if (__x < 10) d_putchar(__basechars[__x]);			\
    else {							\
      unsigned T __root = 1;					\
      while (idiv(__x, __root) >= 10) __root *= 10;		\
      while (__root) {						\
	unsigned __rel = idiv(__x, __root);			\
	d_putchar(__basechars[__rel]);			\
	__x = imod(__x, __root);			\
	__root = idiv(__root, 10);			\
      }                                                 \
    }                                                   \
  } while(0)


BEGIN_FUNC(t_main)
{
  d_putlu(~(unsigned int)0, int); d_putchar('\n');
  d_putlu(idiv(~(unsigned int)0, 1), int); d_putchar('\n');
  d_putqu(~(unsigned long long)0, long long); d_putchar('\n');
  d_putqu(idivq(~(unsigned long long)0, 1LL), long long); d_putchar('\n');
}
END_FUNC(t_main)
