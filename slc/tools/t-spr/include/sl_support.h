
#define sl_proccall(Fun, ...) do { sl_create(,,,,,,sl__forceseq,Fun, ## __VA_ARGS__); sl_sync(); } while(0)

#ifdef __cplusplus
/* we need to import std:: since math functions etc may reside there. */
using namespace std;
#endif

#if defined(__slc_os_host_ptl__) || defined(__slc_os_host_hlsim__) \
    || defined(__slc_os_host_ptld__) || defined(__slc_os_host_hlsimd__)

# include <uTC.h>

# if defined(__slc_os_host_hlsim__) || defined(__slc_os_host_hlsimd__)
#  include <simulator.h>
# endif

typedef uTC::family sl_family_t;

/* restricted pointers may not be supported by
   the underlying C++ compiler. (FIXME: this hacks
   should be removed eventually when restrict works properly) */
[[]]#ifdef res[[]]trict
[[]]#undef res[[]]trict
[[]]#endif
[[]]#ifdef __res[[]]trict__
[[]]#undef __res[[]]trict__
[[]]#endif
[[]]#define res[[]]trict /* nothing */
[[]]#define __res[[]]trict__ /* nothing */

/* utc-ptl defines "main", so any use of "main" by programs
   must be renamed to "Pmain" */
[[]]#define main Pmain

#ifdef UTC_CONFIG_DEBUG
[[]]#define DP[[]]RINT(s) ((void)(std::clog << "[" << pthread_self() << "] " << s << std::endl))
#else
[[]]#define DP[[]]RINT(s) ((void)0)
#endif

#else

/* utc-ptl / hlsim use signature-based profiling; this implies that
   code generators will use markers of the form "_HLSSig(a,b,c)"
   throughout program code. To preserve cross-target portability, this
   form must be elided for all non-ptl targets. */

#define _HLSSig(a,b,c) /* nothing */

#endif

#ifdef __slc_os_host_hrt__
#include <sl_hrt.h>
#endif

#ifdef __slc_soft_float__
#define sl_glfparm sl_glparm
#define sl_glfarg sl_glfarg
#define sl_shfparm sl_shparm
#define sl_shfarg sl_shfarg
#endif
