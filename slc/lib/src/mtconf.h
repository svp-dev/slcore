#ifndef MT_CONF_H
#define MT_CONF_H

#include <stdint.h>

typedef uint32_t confword_t;

extern confword_t mgconf_master_freq;
extern confword_t mgconf_core_freq;
extern confword_t mgconf_ftes_per_core;
extern confword_t mgconf_ttes_per_core;

void sys_detect_devs(void);
void sys_conf_init(void);

#endif
