#ifndef MT_HEAP_H
#define MT_HEAP_H

void *brk(const void*);
void *sbrk(int incr);

void sys_heap_init(void);

#endif
