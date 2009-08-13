sl_def(key_blit, void,
       sl_glparm(unsigned long*restrict, img),
       sl_glparm(unsigned long*restrict, sprite),
       sl_glparm(unsigned long, key))
{
  sl_index(i);
  if (sl_getp(sprite)[i] != sl_getp(key))
    sl_getp(img)[i] = sl_getp(sprite)[i];
}
sl_enddef

// SLT_RUN: -f TEST.d

#ifdef SIMPLE_MAIN
extern unsigned long img[];
extern unsigned long sprite[];

sl_def(t_main, void)
{
  sl_create(,,,10,,,, key_blit,
	    sl_glarg(unsigned long*restrict, _1, img),
	    sl_glarg(unsigned long*restrict, _2, sprite),
	    sl_sharg(unsigned long, _3, 3));
  sl_sync();
}

unsigned long img[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
unsigned long sprite[10] = {3, 11, 3, 22, 3, 3, 3, 33, 3, 3}; 
sl_enddef

#else //! SIMPLE_MAIN

#include <svp/iomacros.h>
#include <svp/fibre.h>
#include <svp/assert.h>

sl_def(printarray, void,
       sl_shparm(long, token),
       sl_glparm(unsigned long*restrict, img))
{
  sl_index(i);
  long x = sl_getp(token);
  putu(sl_getp(img)[i]); putc('\n');
  sl_setp(token, x);
}
sl_enddef


#ifdef min
#undef min
#endif
#define min(A, B) ((A) < (B) ? (A) : (B))


sl_def(t_main, void)
{
  svp_assert(fibre_tag(0) < 2 && fibre_rank(0) == 1);
  svp_assert(fibre_tag(1) < 2 && fibre_rank(1) == 1);
  svp_assert(fibre_tag(2) < 2 && fibre_rank(2) == 0);
  size_t len = min(fibre_shape(0)[0], fibre_shape(1)[0]);
  sl_create(,,,len,,,, key_blit,
	    sl_glarg(unsigned long*restrict, _1, (unsigned long*)fibre_data(0)),
	    sl_glarg(unsigned long*restrict, _2, (unsigned long*)fibre_data(1)),
	    sl_sharg(unsigned long, _3, *(unsigned long*)fibre_data(2)));
  sl_sync();
  sl_create(,,,len,,,, printarray,
	    sl_sharg(long, token, 0),
	    sl_glarg(unsigned long*restrict, _4, (unsigned long*)fibre_data(0)));
  sl_sync();
}
sl_enddef
       
#endif //! SIMPLE_MAIN
