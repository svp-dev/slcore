#ifndef TRICKS_H
#define TRICKS_H

#ifdef __GNUC__
#ifndef likely
#define likely(E) __builtin_expect(!!(E), 1)
#endif
#ifndef unlikely
#define unlikely(E) __builtin_expect((E), 0)
#endif

#ifndef forceinline
#define forceinline __attribute__((__always_inline__))
#endif

#else /* ! __GNUC__ */

#ifndef forceinline
#define forceinline inline
#endif

#ifndef likely
#define likely(E) (E)
#endif
#ifndef unlikely
#define unlikely(E) (E)
#endif

#endif

#endif
