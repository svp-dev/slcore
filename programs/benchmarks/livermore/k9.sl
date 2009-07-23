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

// muTC/SL implementation courtesy of M.A.Hicks

sl_def(innerk9, void,
      sl_glparm(array2d restrict, lpx),
      sl_glfparm(double, ldm28),
      sl_glfparm(double, ldm27),
      sl_glfparm(double, ldm26),
      sl_glfparm(double, ldm25),
      sl_glfparm(double, ldm24),
      sl_glfparm(double, ldm23),
      sl_glfparm(double, ldm22),
      sl_glfparm(double, lc0))
{
	sl_index(i);
	
	sl_getp(lpx)[i][0] = 
	  sl_getp(ldm28) * sl_getp(lpx)[i][12] + 
	  sl_getp(ldm27) * sl_getp(lpx)[i][11] + 
	  sl_getp(ldm26) * sl_getp(lpx)[i][10] +
	  sl_getp(ldm25) * sl_getp(lpx)[i][ 9] + 
	  sl_getp(ldm24) * sl_getp(lpx)[i][ 8] + 
	  sl_getp(ldm23) * sl_getp(lpx)[i][ 7] +
	  sl_getp(ldm22) * sl_getp(lpx)[i][ 6] + 
	  sl_getp(lc0) * ( sl_getp(lpx)[i][ 4] + 
			   sl_getp(lpx)[i][ 5]) +
	  sl_getp(lpx)[i][ 2];
}
sl_enddef

// LL_USE: Px C0 Dm22 Dm23 Dm24 Dm25 Dm26 Dm27 Dm28

sl_def(kernel9, void)
{
  sl_create(,, 0, inner[KERNEL], 1, blocksize[KERNEL],, innerk9,
	    sl_glarg(array2d restrict, llpx, Px),
	    sl_glfarg(double, lldm28, *Dm28),
	    sl_glfarg(double, lldm27, *Dm27),
	    sl_glfarg(double, lldm26, *Dm26),
	    sl_glfarg(double, lldm25, *Dm25),
	    sl_glfarg(double, lldm24, *Dm24),
	    sl_glfarg(double, lldm23, *Dm23),
	    sl_glfarg(double, lldm22, *Dm22),
	    sl_glfarg(double, llc0, *C0));
    sl_sync();
}
sl_enddef
