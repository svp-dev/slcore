#ifndef MT_CONF_H
#define MT_CONF_H

#include <stdint.h>

typedef uint32_t confword_t;

extern const confword_t * mgconf_core_freq;
extern const confword_t * mgconf_ftes_per_core;
extern const confword_t * mgconf_ttes_per_core;
extern void *mgconf_layout_data;

void sys_conf_init(void *init_parameters);

#endif
