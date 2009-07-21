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
      sl_glparm(double*,xl),
      sl_glparm(double*,yl),
      sl_glparm(double*,zl))
{
	sl_index(iteration);
	
	sl_getp(xl)[iteration] = sl_getp(zl)[iteration]*( sl_getp(yl)[iteration] - sl_getp(lastx) );
	
	sl_setp(lastx, sl_getp(xl)[iteration]);
}
sl_enddef

sl_def(kernel5,void)
{

	sl_create(,, 1,inner[KERNEL],1,SHARED_BLOCK,,innerk5,
		sl_shfarg(double,llastx,x[0]),
		sl_glarg(double*,xxl,x),
		sl_glarg(double*,yyl,y),
		sl_glarg(double*,zzl,z));
	sl_sync();
}
sl_enddef
