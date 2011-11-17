#ifndef MTINIT_H
#define MTINIT_H

#include <stdbool.h>

void sys_environ_init(char *initenv);
void sys_check_ncores(void);
void sys_places_init(void);
void sys_sep_init(void);
void sys_fibre_init(void*, bool);
void sys_vars_init(void*, bool);
void sys_argv_init(void);

#endif
