m4_define(KERNEL, [[13]])
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
//      LIVERMORE KERNEL 13
//  2-D PIC (Particle In Cell)
//---------------------------------

//---------------------------------
// 'Original' C:
// for ( ip=0; ip<n; ip++)
//         {
//             i1 = p[ip][0];
//             j1 = p[ip][1];
//             i1 &= 64-1;
//             j1 &= 64-1;
//             p[ip][2] += b[j1][i1];
//             p[ip][3] += c[j1][i1];
//             p[ip][0] += p[ip][2];
//             p[ip][1] += p[ip][3];
//             i2 = p[ip][0];
//             j2 = p[ip][1];
//             i2 = ( i2 & 64-1 ) - 1 ;
//             j2 = ( j2 & 64-1 ) - 1 ;
//             p[ip][0] += y[i2+32];
//             p[ip][1] += z[j2+32];
//             i2 += e[i2+32];
//             j2 += f[j2+32];
//             h[j2][i2] += 1.0;
//         }
//---------------------------------


sl_def(innerk1, void,
       sl_glparm(double*,el),sl_glparm(double*,fl),
       sl_glparm(double*,yl),sl_glparm(double*,zl),
       sl_glparm(array2d,bl),sl_glparm(array2d,cl),
       sl_glparm(array2d,hl),sl_glparm(array2d,pl) )
{
	sl_index(iteration);
	long i1,j1,i2,j2;

	i1 = sl_getp(pl)[iteration][0];
	j1 = sl_getp(pl)[iteration][1];
	i1 &= 64-1;
	j1 &= 64-1;
	sl_getp(pl)[iteration][2] += sl_getp(bl)[j1][i1];
	sl_getp(pl)[iteration][3] += sl_getp(cl)[j1][i1];
	sl_getp(pl)[iteration][0] += sl_getp(pl)[iteration][2];
	sl_getp(pl)[iteration][1] += sl_getp(pl)[iteration][3];
	i2 = sl_getp(pl)[iteration][0];
	j2 = sl_getp(pl)[iteration][1];
	i2 = ( i2 & 64-1 ) - 1 ;
	j2 = ( j2 & 64-1 ) - 1 ;
	sl_getp(pl)[iteration][0] += sl_getp(yl)[i2+32];
	sl_getp(pl)[iteration][1] += sl_getp(zl)[j2+32];
	i2 += sl_getp(el)[i2+32];
	j2 += sl_getp(fl)[j2+32];
	sl_getp(hl)[j2][i2] += 1.0;
}
sl_enddef

sl_def(kernel, void)
{
	
	//create the family of the appropriate size
	//specified in the 'inner' array
	sl_create(,, 0,inner[KERNEL],1,blocksize[KERNEL],,innerk1,
	  sl_glarg(double*,ell,e),sl_glarg(double*,fll,ff),
	  sl_glarg(double*,yll,y),sl_glarg(double*,zll,z),
	  sl_glarg(array2d,bll,b),sl_glarg(array2d,cll,c),
	  sl_glarg(array2d,hll,h),sl_glarg(array2d,pll,p) );
	  sl_sync();

}
sl_enddef
