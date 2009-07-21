//---------------------------------
//      LIVERMORE KERNEL 10
//     Difference Predictors
//---------------------------------

//---------------------------------
// for ( i=0 ; i<n ; i++ )
// {
//   ar        =      cx[i][ 4];
//   br        = ar - px[i][ 4];
//   px[i][ 4] = ar;
//   cr        = br - px[i][ 5];
//   px[i][ 5] = br;
//   ar        = cr - px[i][ 6];
//   px[i][ 6] = cr;
//   br        = ar - px[i][ 7];
//   px[i][ 7] = ar;
//   cr        = br - px[i][ 8];
//   px[i][ 8] = br;
//   ar        = cr - px[i][ 9];
//   px[i][ 9] = cr;
//   br        = ar - px[i][10];
//   px[i][10] = ar;
//   cr        = br - px[i][11];
//   px[i][11] = br;
//   px[i][13] = cr - px[i][12];
//   px[i][12] = cr;
// }
//---------------------------------

// muTC/SL implementation courtesy of M.A.Hicks

sl_def(innerk10, void,
      sl_glparm(array2d, lcx),
      sl_glparm(array2d, lpx))
{

	sl_index(iteration);
	double ar, br, cr;
	
	ar        =      sl_getp(lcx)[iteration][ 4];
	br        = ar - sl_getp(lpx)[iteration][ 4];
	sl_getp(lpx)[iteration][ 4] = ar;
	cr        = br - sl_getp(lpx)[iteration][ 5];
	sl_getp(lpx)[iteration][ 5] = br;
	ar        = cr - sl_getp(lpx)[iteration][ 6];
	sl_getp(lpx)[iteration][ 6] = cr;
	br        = ar - sl_getp(lpx)[iteration][ 7];
	sl_getp(lpx)[iteration][ 7] = ar;
	cr        = br - sl_getp(lpx)[iteration][ 8];
	sl_getp(lpx)[iteration][ 8] = br;
	ar        = cr - sl_getp(lpx)[iteration][ 9];
	sl_getp(lpx)[iteration][ 9] = cr;
	br        = ar - sl_getp(lpx)[iteration][10];
	sl_getp(lpx)[iteration][10] = ar;
	cr        = br - sl_getp(lpx)[iteration][11];
	sl_getp(lpx)[iteration][11] = br;
	sl_getp(lpx)[iteration][13] = cr - sl_getp(lpx)[iteration][12];
	sl_getp(lpx)[iteration][12] = cr;
}
sl_enddef

sl_def(kernel10,void)
{
	sl_create(,, 0,inner[KERNEL],1,blocksize[KERNEL],,innerk10,
		sl_glarg(array2d, llcx, cx),
		sl_glarg(array2d, llpx, px));
	sl_sync();	
}
sl_enddef
