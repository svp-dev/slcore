[[]]
//---------------------------------
// Livemore Loops -- SLC (uTC)
// M.A.Hicks, CSA Group, UvA
// Implementation based on various
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

sl_def(innerk9, void
       , sl_glfparm(double, C0)
       , sl_glfparm(double, DM22)
       , sl_glfparm(double, DM23)
       , sl_glfparm(double, DM24)
       , sl_glfparm(double, DM25)
       , sl_glfparm(double, DM26)
       , sl_glfparm(double, DM27)
       , sl_glfparm(double, DM28)
       , sl_glparm(double*restrict, PX)
       , sl_glparm(size_t, PX_dim0))
{
    sl_index(k);

#define PX(a, b) sl_getp(PX)[(b) * sl_getp(PX_dim0) + (a)]
    double C0 = sl_getp(C0);
    double DM22 = sl_getp(DM22);
    double DM23 = sl_getp(DM23);
    double DM24 = sl_getp(DM24);
    double DM25 = sl_getp(DM25);
    double DM26 = sl_getp(DM26);
    double DM27 = sl_getp(DM27);
    double DM28 = sl_getp(DM28);

    /* copy-paste from fortran, decrease constant offsets by 1 */
    PX( 0,k)= DM28*PX(12,k) + DM27*PX(11,k) + DM26*PX(10,k) +
        DM25*PX(9,k) + DM24*PX( 8,k) + DM23*PX( 7,k) +
        DM22*PX( 6,k) +  C0*(PX( 4,k) +      PX( 5,k))+ PX( 2,k);

}
sl_enddef

sl_def(kernel9, void
       sl_glparm(size_t, ncores),
       sl_glparm(size_t, n)
       , sl_glfparm(double, C0)
       , sl_glfparm(double, DM22)
       , sl_glfparm(double, DM23)
       , sl_glfparm(double, DM24)
       , sl_glfparm(double, DM25)
       , sl_glfparm(double, DM26)
       , sl_glfparm(double, DM27)
       , sl_glfparm(double, DM28)
       , sl_glparm(double*restrict, PX)
       , sl_glparm(size_t, PX_dim0)
       , sl_glparm(size_t, PX_dim1))
{
    sl_create(,, , sl_getp(n),,,,innerk9
              , sl_glfarg(double, , sl_getp(C0))
              , sl_glfarg(double, , sl_getp(DM22))
              , sl_glfarg(double, , sl_getp(DM23))
              , sl_glfarg(double, , sl_getp(DM24))
              , sl_glfarg(double, , sl_getp(DM25))
              , sl_glfarg(double, , sl_getp(DM26))
              , sl_glfarg(double, , sl_getp(DM27))
              , sl_glfarg(double, , sl_getp(DM28))
              , sl_glarg(double*restrict, , sl_getp(PX))
              , sl_glarg(size_t, , sl_getp(PX_dim0)));
    sl_sync();
}
sl_enddef
