[[]]
//---------------------------------
// Livemore Loops -- SLC (uTC)
// M.A.Hicks, CSA Group, UvA
// Implementation based on various
// reference implementations
// including the original FORTRAN
// but mostly from
// Roy Longbottom, 1996.
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


sl_def(innerk2,void,
       sl_glparm(double*restrict, X),
       sl_glparm(const double*restrict, V),
       sl_glparm(long, gi), 
       sl_glparm(long, gk))
{
    sl_index(i);
	
    // the following two variables have to
    // step from a fixed base value specified
    // as a global thread parameter.	
    // local k needs to step by 2, but index
    // only steps by one -- hence multiply
    // iteration by two
    long k = sl_getp(gk) + 2*i;
    long li = sl_getp(gi) + i;

    //now the actual calculation
    sl_getp(X)[li] = sl_getp(X)[k] 
        - sl_getp(V)[k]   * sl_getp(X)[k-1] 
        - sl_getp(V)[k+1] * sl_getp(X)[k+1];
}
sl_enddef

sl_def(kernel2, void,
       sl_glparm(size_t, ncores),
       sl_glparm(size_t, n),
       sl_glparm(const double*restrict, V),
       sl_glparm(size_t, V_dim),
       sl_glparm(double*restrict, X),
       sl_glparm(size_t, X_dim))
{
    long  loopcount = sl_getp(n); //initialise loop counter
    long  lower, upper; //counters
	
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
        loopcount /= 2;
					  
        //innerk2 simulates 1 and 2 stride by using inc of 1 
        // and multiplying by 2 where required. We must make
        // sure therefore that the range is halved:
		
        long range = (upper - lower) / 2;
					    
        sl_create(,, , range, , ,, innerk2,
                  sl_glarg(double*, , sl_getp(X)),
                  sl_glarg(const double*, , sl_getp(V)),
                  sl_glarg(long, , upper),
                  sl_glarg(long, , (lower+1)));
        sl_sync();
    } while (loopcount > 0);
}
sl_enddef
