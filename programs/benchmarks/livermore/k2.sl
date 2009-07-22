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
	sl_glparm(double*,xl),sl_glparm(double*,vl),
	sl_glparm(int,gi), sl_glparm(int,gk))
{
	sl_index(iteration);
	
	// the following two variables have to
	// step from a fixed base value specified
	// as a global thread parameter.	
	// local k needs to step by 2, but index
	// only steps by one -- hence multiply
	// iteration by two (add it to itself)
	// (remember that integer divide is
	//  (as of 05/09) *very* slow
	int localk = sl_getp(gk) + (iteration << 1);//(iteration+iteration);
	int locali = sl_getp(gi) + iteration;

	//now the actual calculation
	sl_getp(xl)[locali] = sl_getp(xl)[localk] - sl_getp(vl)[localk]*sl_getp(xl)[localk-1] -
			sl_getp(vl)[localk+1]*sl_getp(xl)[localk+1];
}
sl_enddef

sl_def(kernel2,void)
{
	long int loopcount = inner[KERNEL]; //initialise loop counter
	long int lower,upper; //counters
	
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
		loopcount = loopcount >> 1; //divide by two (expressed here as
					    // a right shift to avoid
					    // calling the integer divide routine
					  
		//innerk2 simulates 1 and 2 stride by using inc of 1 
		// and multiplying by 2 where required. We must make
		// sure therefore that the range is halved:
		
		unsigned int range = ((upper - lower) >> 1);
					    
		sl_create(,, 0,range,1,blocksize[KERNEL],,innerk2,
   			sl_glarg(double*,xxxl,x),
			sl_glarg(double*,vvl,v),
			sl_glarg(int,ggi,upper),
			sl_glarg(int,ggk,(lower+1)));
		sl_sync();
	} while (loopcount > 0);
}
sl_enddef
