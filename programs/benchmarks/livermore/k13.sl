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

// muTC/SL implementation courtesy of M.A.Hicks


sl_def(innerk1, void,
       sl_glparm(double*restrict, el), sl_glparm(double*restrict, fl),
       sl_glparm(double*restrict, yl), sl_glparm(double*restrict, zl),
       sl_glparm(array2d restrict, bl), sl_glparm(array2d restrict, cl),
       sl_glparm(array2d restrict, hl), sl_glparm(array2d restrict, pl),
       sl_glfparm(double, one))
{
	sl_index(ip);
	long i1,j1,i2,j2;

	i1 = sl_getp(pl)[ip][0];
	j1 = sl_getp(pl)[ip][1];
	i1 &= 64-1;
	j1 &= 64-1;
	sl_getp(pl)[ip][2] += sl_getp(bl)[j1][i1];
	sl_getp(pl)[ip][3] += sl_getp(cl)[j1][i1];
	sl_getp(pl)[ip][0] += sl_getp(pl)[ip][2];
	sl_getp(pl)[ip][1] += sl_getp(pl)[ip][3];
	i2 = sl_getp(pl)[ip][0];
	j2 = sl_getp(pl)[ip][1];
	i2 = ( i2 & 64-1 ) - 1 ;
	j2 = ( j2 & 64-1 ) - 1 ;
	sl_getp(pl)[ip][0] += sl_getp(yl)[i2+32];
	sl_getp(pl)[ip][1] += sl_getp(zl)[j2+32];
	i2 += sl_getp(el)[i2+32];
	j2 += sl_getp(fl)[j2+32];
	sl_getp(hl)[j2][i2] += sl_getp(one);
}
sl_enddef

// LL_USE: Ex Y B H V Z Cx P
sl_def(kernel13, void)
{
  // create the family of the appropriate size
  // specified in the 'inner' array
  sl_create(,, 0, inner[KERNEL], 1, blocksize[KERNEL],, innerk1,
	    sl_glarg(double*restrict, ell, Ex), sl_glarg(double*restrict, fll, V),
	    sl_glarg(double*restrict, yll, Y), sl_glarg(double*restrict, zll, Z),
	    sl_glarg(array2d restrict, bll, B), sl_glarg(array2d restrict, cll, Cx),
	    sl_glarg(array2d restrict, hll, H), sl_glarg(array2d restrict, pll, P),
	    sl_glfarg(double, one, 1.0));
  sl_sync();
}
sl_enddef
