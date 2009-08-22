#include <cmalloc.h>

sl_def(t_main, void)
{
 void *p0;
 void *p1;
 void *p2;
 void *p3;
 void *p4;
 void *p5;
 p0 = malloc(1000);
 p1 = malloc(100);
 p2 = malloc(1000);
 p3 = malloc(100);
 p4 = malloc(1000);
 p5 = malloc(100);
 free(p5);
 free(p4);
 free(p3);
 free(p2);
 free(p1);
 free(p0);
}
sl_enddef
