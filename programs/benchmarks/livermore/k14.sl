//---------------------------------
//      LIVERMORE KERNEL 14
//  1-D PIC (Particle In Cell)
//---------------------------------

//---------------------------------
// 'Original' C:
//         for ( k=0 ; k<n ; k++ )
//         {
//             vx[k] = 0.0;
//             xx[k] = 0.0;
//             ix[k] = (long) grd[k];
//             xi[k] = (double) ix[k];
//             ex1[k] = ex[ ix[k] - 1 ];
//             dex1[k] = dex[ ix[k] - 1 ];
//         }
//         for ( k=0 ; k<n ; k++ )
//         {
//             vx[k] = vx[k] + ex1[k] + ( xx[k] - xi[k] )*dex1[k];
//             xx[k] = xx[k] + vx[k]  + flx;
//             ir[k] = xx[k];
//             rx[k] = xx[k] - ir[k];
//             ir[k] = ( ir[k] & 2048-1 ) + 1;
//             xx[k] = rx[k] + ir[k];
//         }
//         for ( k=0 ; k<n ; k++ )
//         {
//             rh[ ir[k]-1 ] += 1.0 - rx[k];
//             rh[ ir[k]   ] += rx[k];
//         }
//---------------------------------

// muTC/SL implementation courtesy of M.A.Hicks


sl_def(innerphase1, void,
       sl_glparm(double*restrict, vxl),
       sl_glparm(double*restrict, xxl),
       sl_glparm(long*restrict, ixl),
       sl_glparm(double*restrict, xil),
       sl_glparm(double*restrict, ex1l),
       sl_glparm(double*restrict, exl),
       sl_glparm(double*restrict, dexl),
       sl_glparm(double*restrict, dex1l),
       sl_glparm(double*restrict, grdl))
{
  sl_index(k);
  
  sl_getp(vxl)[k] = 0.0;
  sl_getp(xxl)[k] = 0.0;
  sl_getp(ixl)[k] = (long) sl_getp(grdl)[k];
  sl_getp(xil)[k] = (double) sl_getp(ixl)[k];
  sl_getp(ex1l)[k] = sl_getp(exl)[ sl_getp(ixl)[k] - 1 ];
  sl_getp(dex1l)[k] = sl_getp(dexl)[ sl_getp(ixl)[k] - 1 ];
}
sl_enddef

sl_def(innerphase2, void,
	sl_glparm(double*restrict, vxl),
	sl_glparm(double*restrict, ex1l),
	sl_glparm(double*restrict, xxl),
	sl_glparm(double*restrict, xil),
	sl_glparm(double*restrict, dex1l),
	sl_glfparm(double, flxl),
	sl_glparm(long*restrict, irl),
	sl_glparm(double*restrict, rxl))
{
	sl_index(k);

	sl_getp(vxl)[k] = sl_getp(vxl)[k] + sl_getp(ex1l)[k] + 
	  ( sl_getp(xxl)[k] - sl_getp(xil)[k] ) * sl_getp(dex1l)[k];
	sl_getp(xxl)[k] = sl_getp(xxl)[k] + sl_getp(vxl)[k]  + sl_getp(flxl);
	sl_getp(irl)[k] = sl_getp(xxl)[k];
	sl_getp(rxl)[k] = sl_getp(xxl)[k] - sl_getp(irl)[k];
	sl_getp(irl)[k] = ( sl_getp(irl)[k] & 2048-1 ) + 1;
	sl_getp(xxl)[k] = sl_getp(rxl)[k] + sl_getp(irl)[k];
}
sl_enddef

sl_def(innerphase3, void,
       sl_glparm(double*restrict, rxl),
       sl_glparm(long*restrict, irl),
       sl_glparm(double*restrict, rhl))
{
  sl_index(k);
  
  sl_getp(rhl)[ sl_getp(irl)[k]-1 ] += 1.0 - sl_getp(rxl)[k];
  sl_getp(rhl)[ sl_getp(irl)[k]   ] += sl_getp(rxl)[k];
}
sl_enddef

// LL_USE: V X Ix Z Y Ex Dex Dex1 Grd Flx Rx Ir Rh
sl_def(kernel14, void)
{
  //create the family of the appropriate size
  //specified in the 'inner' array
  sl_create(,, 0, inner[KERNEL], 1, blocksize[KERNEL],, innerphase1,
	    sl_glarg(double*restrict, vxll, V),
	    sl_glarg(double*restrict, xxll, X),
	    sl_glarg(long*restrict, ixll, Ix),
	    sl_glarg(double*restrict, xill, Z),
	    sl_glarg(double*restrict, ex1ll, Y),
	    sl_glarg(double*restrict, exll, Ex),
	    sl_glarg(double*restrict, dexll, Dex),
	    sl_glarg(double*restrict, dex1ll, Dex1),
	    sl_glarg(double*restrict, grdll, Grd));
  sl_sync();
  
  sl_create(,, 0, inner[KERNEL], 1, blocksize[KERNEL],, innerphase2,
	    sl_glarg(double*restrict, vxlll, V),
	    sl_glarg(double*restrict, ex1lll, Y),
	    sl_glarg(double*restrict, xxlll, X),
	    sl_glarg(double*restrict, xilll, Z),
	    sl_glarg(double*restrict, dex1lll, Dex1),
	    sl_glfarg(double, flxll, *Flx),
	    sl_glarg(long*restrict, irll, Ir),
	    sl_glarg(double*restrict, rxll, Rx));
  sl_sync();
  
  sl_create(,, 0, inner[KERNEL], 1, blocksize[KERNEL],, innerphase3,
	    sl_glarg(double*restrict, rxlll, Rx),
	    sl_glarg(long*, irlll, Ir),
	    sl_glarg(double*restrict, rhll, Rh));
  sl_sync();
}
sl_enddef
