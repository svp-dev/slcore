
#define sl_proccall(Fun, ...) do { sl_create(,,,,,,sl__forceseq,Fun, ## __VA_ARGS__); sl_sync(); } while(0)

#ifdef __slc_os_host_hrt__
#include <sl_hrt.h>
#endif

#ifdef __slc_soft_float__
#define sl_glfparm sl_glparm
#define sl_glfarg sl_glfarg
#define sl_shfparm sl_shparm
#define sl_shfarg sl_shfarg
#endif
