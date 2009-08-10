//---------------------------------
//      LIVERMORE KERNEL 4
//     Banded Linear Equations
//---------------------------------

//---------------------------------
//     int m = (1001 - 7) / 2;
//     for ( k=6 ; k<1001 ; k=k+m )
//      {
//         lw = k - 6;
//         temp = x[k-1];
//         for ( j=4 ; j<n ; j=j+5 )
//          {
//             temp -= x[lw]*y[j];
//             lw++;
//          }
//         x[k-1] = y[4]*temp;
//      }
//---------------------------------

// muTC/SL implementation courtesy of M.A.Hicks

//this variable is defined slightly strangely
//in the reference implementation
//m4_define(MSTEP, [[((inner[KERNEL] -7) >> 1)]])
m4_define(MSTEP, [[((1001 -7) >> 1)]])

sl_def(innerk4, void,
       sl_shfparm(double, total),
       sl_glparm(double*restrict, xl),
       sl_glparm(double*restrict, yl),
       sl_glparm(unsigned int, lw))
{
  sl_index(i);
  //j -> 5 step with a starting value of 4
  sl_setp(total, sl_getp(total) - (sl_getp(xl)[sl_getp(lw)+i] * sl_getp(yl)[(i*5) + 4]));
}
sl_enddef

sl_def(outerk4, void,
       sl_glparm(double*restrict, xl),
       sl_glparm(double*restrict, yl))
{
  sl_index(i);
  unsigned int counter = i - 6;
  
	
  //check that previous loop iteration has finished
  //because innerk4 reads previous result from xl[]
  //	int temp = sl_getp(writelock);

  //loop uses an internal stride of 5 by multiplying a 1 stride counter
  //this means the range of the loop must be divided by 5
	
  sl_create(,, 0, inner[KERNEL] / 5, 1, SHARED_BLOCK,, innerk4,
	    sl_shfarg(double, ttotal, sl_getp(xl)[i-1]),
	    sl_glarg(double*restrict, xxl, sl_getp(xl)),
	    sl_glarg(double*restrict, yyl, sl_getp(yl)),
	    sl_glarg(unsigned int, llw, counter));
  sl_sync();

  sl_getp(xl)[i-1] = sl_getp(yl)[4] * sl_geta(ttotal);
  //now set this lock as free by writing to shared, since xl[]
  // has been written safely.
  //	sl_setp(writelock, temp + 1);
}
sl_enddef

// LL_USE: X Y
sl_def(kernel4, void)
{
  sl_create(,, 6, 1001, MSTEP, blocksize[KERNEL],, outerk4,
	    sl_glarg(double*restrict, xxl, X),
	    sl_glarg(double*restrict, yyl, Y));
  sl_sync();
}
sl_enddef

/*
sl_def(kernel4, void)
{
	// outer loop as sequential loop
	unsigned int counter;
	
	for(counter=6; counter<inner[KERNEL];counter+=MSTEP){
		
		sl_create(,, 0,inner[KERNEL],1,blocksize[KERNEL],,innerk4,
			sl_shfarg(double, ttotal, X[counter-1]),
			sl_glarg(double*restrict,xxl, X),
			sl_glarg(double*restrict, yyl, Y),
			sl_glarg(unsigned int, llw,(counter-6)));
		sl_sync();
		
		X[counter-1] = Y[4] * sl_geta(ttotal);
	}
}
sl_enddef
*/
