//---------------------------------
//      LIVERMORE KERNEL 12
//       First Difference
//---------------------------------

//---------------------------------
// for ( k=0 ; k<n ; k++ )
// {
//   x[k] = y[k+1] - y[k];
// }
//---------------------------------

// muTC/SL implementation courtesy of M.A.Hicks

sl_def(innerk12,void,
	sl_glparm(double*,xl),
	sl_glparm(double*,yl))
{
	sl_index(iteration);
	sl_getp(xl)[iteration] = sl_getp(yl)[iteration+1] - sl_getp(yl)[iteration];
}
sl_enddef

sl_def(kernel12,void)
{
	sl_create(,, 0,inner[KERNEL],1,blocksize[KERNEL],,innerk12,
     		sl_glarg(double*,xxl,x),
		sl_glarg(double*,yyl,y));
	sl_sync();
}
sl_enddef
