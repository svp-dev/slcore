//---------------------------------
//      LIVERMORE KERNEL 5
//        tri-diagonal
//   elimination, below diagonal
//---------------------------------

//---------------------------------
// for ( i=1 ; i<n ; i++ )
// {
//  x[i] = z[i]*( y[i] - x[i-1] );
// }
//---------------------------------

// muTC/SL implementation courtesy of M.A.Hicks

sl_def(innerk5,void,
       sl_shfparm(double, lastx),
       sl_glparm(double*restrict, xl),
       sl_glparm(double*restrict, yl),
       sl_glparm(double*restrict, zl))
{
  sl_index(iteration);
	
  sl_getp(xl)[iteration] = sl_getp(zl)[iteration]*( sl_getp(yl)[iteration] - sl_getp(lastx) );
  
  sl_setp(lastx, sl_getp(xl)[iteration]);
}
sl_enddef

// LL_USE: X Y Z
sl_def(kernel5,void)
{
  sl_create(,, 1, inner[KERNEL], 1, SHARED_BLOCK,, innerk5,
	    sl_shfarg(double, llastx, X[0]),
	    sl_glarg(double*restrict, xxl, X),
	    sl_glarg(double*restrict, yyl, Y),
	    sl_glarg(double*restrict, zzl, Z));
  sl_sync();
}
sl_enddef
