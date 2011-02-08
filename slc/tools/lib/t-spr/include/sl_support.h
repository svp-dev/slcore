
#define sl_proccall(Fun, ...) do { sl_create(,,,,,,sl__forceseq,Fun, ## __VA_ARGS__); sl_sync(); } while(0)

#ifdef __slc_os_host_hrt__
#include <sl_hrt.h>
#endif
