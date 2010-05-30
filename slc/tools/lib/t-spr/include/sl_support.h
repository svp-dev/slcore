
#define sl_proccall(Fun, ...) do { sl_create(,,,,,,,Fun, ## __VA_ARGS__); sl_sync(); } while(0)
