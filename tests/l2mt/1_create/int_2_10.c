#include <svp/testoutput.h>

sl_def(foo_2_10, void, sl_glparm(int*, output) ,
	sl_glparm(int,garg0),
	sl_glparm(int,garg1),
	sl_glparm(int,garg2),
	sl_glparm(int,garg3),
	sl_glparm(int,garg4),
	sl_glparm(int,garg5),
	sl_glparm(int,garg6),
	sl_glparm(int,garg7),
	sl_glparm(int,garg8),
	sl_glparm(int,garg9) )
{
  *sl_getp(output) = (sl_getp(garg0)
	+sl_getp(garg1)
	+sl_getp(garg2)
	+sl_getp(garg3)
	+sl_getp(garg4)
	+sl_getp(garg5)
	+sl_getp(garg6)
	+sl_getp(garg7)
	+sl_getp(garg8)
	+sl_getp(garg9)
  );
}
sl_enddef

int iresult = 0;

sl_def(do_call, void) {
  int result; 
  sl_proccall(foo_2_10, sl_glarg(int*, output, &result) ,
	sl_glarg(int,garg0,0),
	sl_glarg(int,garg1,1),
	sl_glarg(int,garg2,2),
	sl_glarg(int,garg3,3),
	sl_glarg(int,garg4,4),
	sl_glarg(int,garg5,5),
	sl_glarg(int,garg6,6),
	sl_glarg(int,garg7,7),
	sl_glarg(int,garg8,8),
	sl_glarg(int,garg9,9)   );
   iresult = result;
}
sl_enddef

int main(void) {
    sl_proccall(do_call);
    return iresult - 45;
}
