m4_define(KERNEL, [[14]])
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


sl_def(innerphase1, void,
	sl_glparm(double*,vxl),
	sl_glparm(double*,xxl),
	sl_glparm(long*,ixl),
	sl_glparm(double*,xil),
	sl_glparm(double*,ex1l),
	sl_glparm(double*,exl),
	sl_glparm(double*,dexl),
	sl_glparm(double*,dex1l),
	sl_glparm(double*,grdl))
{
	sl_index(iteration);

	sl_getp(vxl)[iteration] = 0.0;
	sl_getp(xxl)[iteration] = 0.0;
	sl_getp(ixl)[iteration] = (long) sl_getp(grdl)[iteration];
	sl_getp(xil)[iteration] = (double) sl_getp(ixl)[iteration];
	sl_getp(ex1l)[iteration] = sl_getp(exl)[ sl_getp(ixl)[iteration] - 1 ];
	sl_getp(dex1l)[iteration] = sl_getp(dexl)[ sl_getp(ixl)[iteration] - 1 ];
}
sl_enddef

sl_def(innerphase2, void,
	sl_glparm(double*,vxl),
	sl_glparm(double*,ex1l),
	sl_glparm(double*,xxl),
	sl_glparm(double*,xil),
	sl_glparm(double*,dex1l),
	sl_glfparm(double, flxl),
	sl_glparm(long*,irl),
	sl_glparm(double*,rxl))
{
	sl_index(iteration);

	sl_getp(vxl)[iteration] = sl_getp(vxl)[iteration] + sl_getp(ex1l)[iteration] + ( sl_getp(xxl)[iteration] - sl_getp(xil)[iteration] )*sl_getp(dex1l)[iteration];
	sl_getp(xxl)[iteration] = sl_getp(xxl)[iteration] + sl_getp(vxl)[iteration]  + sl_getp(flxl);
	sl_getp(irl)[iteration] = sl_getp(xxl)[iteration];
	sl_getp(rxl)[iteration] = sl_getp(xxl)[iteration] - sl_getp(irl)[iteration];
	sl_getp(irl)[iteration] = ( sl_getp(irl)[iteration] & 2048-1 ) + 1;
	sl_getp(xxl)[iteration] = sl_getp(rxl)[iteration] + sl_getp(irl)[iteration];
}
sl_enddef

sl_def(innerphase3, void,
	sl_glparm(double*,rxl),
	sl_glparm(long*,irl),
	sl_glparm(double*,rhl))
{
	sl_index(iteration);

	  sl_getp(rhl)[ sl_getp(irl)[iteration]-1 ] += 1.0 - sl_getp(rxl)[iteration];
	  sl_getp(rhl)[ sl_getp(irl)[iteration]   ] += sl_getp(rxl)[iteration];

}
sl_enddef

sl_def(kernel, void)
{

	//create the family of the appropriate size
	//specified in the 'inner' array
	sl_create(,, 0,inner[KERNEL],1,blocksize[KERNEL],,innerphase1,
		sl_glarg(double*,vxll,vx),
		sl_glarg(double*,xxll,xxx),
		sl_glarg(long*,ixll,ix),
		sl_glarg(double*,xill,xi),
		sl_glarg(double*,ex1ll,ex1),
		sl_glarg(double*,exll,ex),
		sl_glarg(double*,dexll,dex),
		sl_glarg(double*,dex1ll,dex1),
		sl_glarg(double*,grdll,grd));
	sl_sync();

	sl_create(,, 0,inner[KERNEL],1,blocksize[KERNEL],,innerphase2,
		sl_glarg(double*,vxlll,vx),
		sl_glarg(double*,ex1lll,ex1),
		sl_glarg(double*,xxlll,xxx),
		sl_glarg(double*,xilll,xi),
		sl_glarg(double*,dex1lll,dex1),
		sl_glfarg(double,flxll,flx),
		sl_glarg(long*, irll, ir),
		sl_glarg(double*,rxll,rx));
	sl_sync();

	sl_create(,, 0,inner[KERNEL],1,blocksize[KERNEL],,innerphase3,
		sl_glarg(double*,rxlll,rx),
		sl_glarg(long*, irlll, ir),
		sl_glarg(double*,rhll,rh));
	sl_sync();

}
sl_enddef
