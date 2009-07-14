m4_define(KERNEL, [[9]])
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
//      LIVERMORE KERNEL 9
//     Integrate Predictors
//---------------------------------

//---------------------------------
// for ( i=0 ; i<n ; i++ )
// {
//   px[i][0] = dm28*px[i][12] + dm27*px[i][11] + dm26*px[i][10] +
//              dm25*px[i][ 9] + dm24*px[i][ 8] + dm23*px[i][ 7] +
//              dm22*px[i][ 6] + c0*( px[i][ 4] + px[i][ 5])
//              + px[i][ 2];
// }
//---------------------------------

sl_def(innerk9, void,
      sl_glparm(array2d, lpx),
      sl_glfparm(double, ldm28),
      sl_glfparm(double, ldm27),
      sl_glfparm(double, ldm26),
      sl_glfparm(double, ldm25),
      sl_glfparm(double, ldm24),
      sl_glfparm(double, ldm23),
      sl_glfparm(double, ldm22),
      sl_glfparm(double, lc0))
{
	sl_index(iteration);
	
	sl_getp(lpx)[iteration][0] = sl_getp(ldm28)*sl_getp(lpx)[iteration][12] + sl_getp(ldm27)*sl_getp(lpx)[iteration][11] + sl_getp(ldm26)*sl_getp(lpx)[iteration][10] +
		sl_getp(ldm25)*sl_getp(lpx)[iteration][ 9] + sl_getp(ldm24)*sl_getp(lpx)[iteration][ 8] + sl_getp(ldm23)*sl_getp(lpx)[iteration][ 7] +
		sl_getp(ldm22)*sl_getp(lpx)[iteration][ 6] + sl_getp(lc0)*( sl_getp(lpx)[iteration][ 4] + sl_getp(lpx)[iteration][ 5])
		+ sl_getp(lpx)[iteration][ 2];
}
sl_enddef

sl_def(kernel, void)
{
	sl_create(,, 0,inner[KERNEL],1,blocksize[KERNEL],,innerk9,
		sl_glarg(array2d, llpx, px),
		sl_glfarg(double, lldm28, dm28),
		sl_glfarg(double, lldm27, dm27),
		sl_glfarg(double, lldm26, dm26),
		sl_glfarg(double, lldm25, dm25),
		sl_glfarg(double, lldm24, dm24),
		sl_glfarg(double, lldm23, dm23),
		sl_glfarg(double, lldm22, dm22),
		sl_glfarg(double, llc0, c0))
	sl_sync();
}
sl_enddef
