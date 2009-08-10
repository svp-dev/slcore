#ifndef __SVP_TESTIO_H__
# define __SVP_TESTIO_H__

#ifdef __mt_freestanding__

# if defined(__alpha__) || defined(__mtalpha__)
#  define output_float(F, Stream, Precision) __asm__ __volatile("printf %0, %1" : : "f"(F), \
								"r"(Stream | (Precision << 4)))

#  define output_uint(N, Stream) __asm__ __volatile__("print %0, %1" : : "r"(N), "rI"(Stream | (0 << 6)))
#  define output_hex(N, Stream) __asm__ __volatile__("print %0, %1" : : "r"(N), "rI"(Stream | (1 << 6)))
#  define output_int(N, Stream) __asm__ __volatile__("print %0, %1" : : "r"(N), "rI"(Stream | (2 << 6)))
#  define output_char(N, Stream) __asm__ __volatile__("print %0, %1" : : "r"(N), "rI"(Stream | (3 << 6)))

# else
#  warning "Don't know how to define debug_int/debug_float on this platform."
# endif

# define output_string(S, Stream)		 \
  do {						 \
    const char *__ptr = (S);			 \
    while(*__ptr) output_char(*__ptr++, Stream); \
  } while(0)

# define output_bytes(S, L, Stream)				\
  do {								\
    const char *__ptr = (S);					\
    unsigned long __i = 0, __max = (L);				\
    while(__i < __max) output_char(__ptr[__i++], Stream);	\
  } while(0)

#else 

# ifdef __cplusplus
#  include <iostream>
#  include <iomanip>
#  define output_float(F, Stream, Precision) ((((Stream) == 2)? std::cerr : std::cout) \
					      << std::setprecision(Precision) \
					      << std::scientific \
					      << (double)F)
#  define output_char(N, Stream) ((((Stream) == 2)? std::cerr : std::cout) << (char)(N))
#  define output_hex(N, Stream) ((((Stream) == 2)? std::cerr : std::cout) << std::hex << (unsigned long)(N))
#  define output_int(N, Stream) ((((Stream) == 2)? std::cerr : std::cout) << std::dec << (long)(N))
#  define output_uint(N, Stream) ((((Stream) == 2)? std::cerr : std::cout) << std::dec << (unsigned long)(N))
#  define output_string(S, Stream) ((((Stream) == 2)? std::cerr : std::cout) << (const char*)(S))
#  define output_bytes(S, L, Stream) ((((Stream) == 2)? std::cerr : std::cout).write((const char*)(S), (L)))
# else
#  include <stdio.h>
#  define output_float(F, Stream, Precision) fprintf((Stream) == 2 ? stderr : stdout, "%.*le", (Precision), (double)(F))
#  define output_char(N, Stream) fprintf((Stream) == 2 ? stderr : stdout, "%c", (char)(N))
#  define output_hex(N, Stream) fprintf((Stream) == 2 ? stderr : stdout, "%lx", (unsigned long)(N))
#  define output_int(N, Stream) fprintf((Stream) == 2 ? stderr : stdout, "%ld", (long)(N))
#  define output_uint(N, Stream) fprintf((Stream) == 2 ? stderr : stdout, "%lu", (unsigned long)(N))
#  define output_string(S, Stream) fprintf((Stream) == 2 ? stderr : stdout, "%s", (const char*)(S))
#  define output_bytes(S, L, Stream) fwrite((const char*)(S), (L), 1, (Stream) == 2 ? stderr : stdout)
# endif

#endif

#endif
