#ifndef __SVP_ABORT_H__
# define __SVP_ABORT_H__

#ifdef __mt_freestanding__

#ifndef __GNUC__
# warn __GNUC__ is not defined. Some macros here may not work.
#endif

# if defined(__alpha__) || defined(__mtalpha__)
#   define svp_abort() __asm__ __volatile__("nop\n\tnop\n\tpal1d 0" : : : "memory")
# elif defined(__sparc__) || defined(__mtsparc__)
#   define svp_abort() __asm__ __volatile__("nop\n\tnop\n\tunimp" : : : "memory")
# else
#   warn svp_abort() is not defined on this platform.
# endif

#else
# include <stdlib.h>
# define svp_abort() abort()
#endif


#endif
