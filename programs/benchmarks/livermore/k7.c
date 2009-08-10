//---------------------------------
//      LIVERMORE KERNEL 7
//      equation of state
//          fragment
//---------------------------------

//---------------------------------
// for ( k=0 ; k<n ; k++ )
// {
//   x[k] = u[k] + r*( z[k] + r*y[k] ) +
//    t*( u[k+3] + r*( u[k+2] + r*u[k+1] ) +
//    t*( u[k+6] + q*( u[k+5] + q*u[k+4] ) ) );
//  }
//---------------------------------

// muTC/SL implementation courtesy of M.A.Hicks

// LL_USE: U X Y Z R T Q

//independent loop
sl_def(innerk7, void,
       sl_glparm(double*restrict, xl),
       sl_glparm(double*restrict, ul),
       sl_glparm(double*restrict, zl),
       sl_glparm(double*restrict, yl),
       sl_glparm(double, rl),
       sl_glparm(double, tl),
       sl_glparm(double, ql))
{
  sl_index(i);
  
  sl_getp(xl)[i] = 
    sl_getp(ul)[i] +	              
    sl_getp(rl) * ( sl_getp(zl)[i] + 
		    sl_getp(rl) * sl_getp(yl)[i] ) + 
    sl_getp(tl) * ( sl_getp(ul)[i+3] + 
		    sl_getp(rl) * ( sl_getp(ul)[i+2] + 
				    sl_getp(rl) * sl_getp(ul)[i+1] ) +
		    sl_getp(tl) * ( sl_getp(ul)[i+6] + 
				    sl_getp(ql) * ( sl_getp(ul)[i+5] + 
						    sl_getp(ql) * sl_getp(ul)[i+4] ) ) );
}
sl_enddef

sl_def(kernel7,void)
{
  sl_create(,, 0, inner[KERNEL], 1, blocksize[KERNEL],, innerk7,
	    sl_glarg(double*restrict, xx, X),
	    sl_glarg(double*restrict, uu, U),
	    sl_glarg(double*restrict, zz, Z),
	    sl_glarg(double*restrict, yy, Y),
	    sl_glarg(double,rrr, *R),
	    sl_glarg(double,tt, *T),
	    sl_glarg(double,qq, *Q));
  sl_sync();
}
sl_enddef
