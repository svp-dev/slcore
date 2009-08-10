//----------------------------------------
//      LIVERMORE KERNEL 2
// Incomplete Cholesky Conjugate Gradient
//----------------------------------------

//---------------------------------
// ii = n;
// ipntp = 0;
// do
//  {
//   ipnt = ipntp;
//   ipntp += ii;
//   ii /= 2;
//   i = ipntp;
//   for ( k=ipnt+1 ; k<ipntp ; k=k+2 )
//     {
//       i++;
//       x[i] = x[k] - v[k]*x[k-1] - v[k+1]*x[k+1];
//     }
//  } while ( ii>0 );
//---------------------------------

// muTC/SL implementation courtesy of M.A.Hicks

sl_def(innerk2,void,
       sl_glparm(double*restrict, xl),
       sl_glparm(double*restrict, vl),
       sl_glparm(int, gi), 
       sl_glparm(int, gk))
{
  sl_index(i);
	
  // the following two variables have to
  // step from a fixed base value specified
  // as a global thread parameter.	
  // local k needs to step by 2, but index
  // only steps by one -- hence multiply
  // iteration by two (add it to itself)
  // (remember that integer divide is
  //  (as of 05/09) *very* slow
  int localk = sl_getp(gk) + (i << 1);//(i+i);
  int locali = sl_getp(gi) + i;

  //now the actual calculation
  sl_getp(xl)[locali] = 
    sl_getp(xl)[localk] - 
    sl_getp(vl)[localk] * sl_getp(xl)[localk-1] -
    sl_getp(vl)[localk+1] * sl_getp(xl)[localk+1];
}
sl_enddef

// LL_USE: X V
sl_def(kernel2, void)
{
  long loopcount = inner[KERNEL]; //initialise loop counter
  long lower, upper; //counters
	
  // upper must first be initialised to zero
  upper = 0;
	
  // N.B. Additional concurrency could be created by converting the
  // below do-while loop into a breaking family.
  // BUT there is no break yet (as of 05/09).

	// The code here is implemented in a standard sequential loop.
	// This is because the inner for loop is dependent on calculations
	// performed in  each epoch initiated by the precedent outer loop.
	do
	{
		lower = upper;
		upper += loopcount;
		loopcount = loopcount / 2;
					  
		//innerk2 simulates 1 and 2 stride by using inc of 1 
		// and multiplying by 2 where required. We must make
		// sure therefore that the range is halved:
		
		unsigned int range = ((upper - lower) / 2);
					    
		sl_create(,, 0, range, 1, blocksize[KERNEL],, innerk2,
			  sl_glarg(double*restrict, xxxl, X),
			  sl_glarg(double*restrict, vvl, V),
			  sl_glarg(int, ggi, upper),
			  sl_glarg(int, ggk, (lower+1)));
		sl_sync();
	} while (loopcount > 0);
}
sl_enddef
