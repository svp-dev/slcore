m4_define(KERNEL, [[5]])
m4_include(livermore.slh)
//---------------------------------
// Livemore Loops -- SLC (uTC)
// M.A.Hicks, CSA Group, UvA
// Implementation based of various
// reference implementations
// including the original FORTRAN
// but mostly from
// Roy Longbottom, 1996.
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

sl_def(kernel,void)
{

	sl_create(,, 1,inner[KERNEL],1,SHARED_BLOCK,,innerk5,
		sl_shfarg(double,llastx,x[0]),
		sl_glarg(double*,xxl,x),
		sl_glarg(double*,yyl,y),
		sl_glarg(double*,zzl,z));
	sl_sync();
}
sl_enddef
