
#define sl_proccall(Fun, ...) do { sl_create(,,,,,,sl__forceseq,Fun, ## __VA_ARGS__); sl_sync(); } while(0)

#if defined(__slc_os_host_ptl__)

/*
  For using the embedded utc-ptl, use no flags at all. (warning: may not work any more.)

  For plain utc-ptl, using an external utc-ptl distribution, use the following flags:
  -DUSE_EXTERNAL_PTL -DUSE_PLAIN_PTL -I/path/to/ptl/include -L/path/to/ptl/lib -lptl-lib-name

  For utc-ptl + hlsim, using external utc-ptl and hlsim, use the following:
  -DUSE_EXTERNAL_PTL -I/path/to/ptl/include -L/path/to/ptl/lib -lptl-lib-name \
        -I/path/to/hlsim/include -L/path/to/hlsim/lib -lhlsim-lib-name

 */
#  ifdef USE_EXTERNAL_PTL
#    include <uTC.h>
#    ifndef USE_PLAIN_PTL
#      include <simulator.h>
#    endif
#  else
#    include "ptl_svp.h"
#  endif

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
