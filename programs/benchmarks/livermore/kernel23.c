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
//      LIVERMORE KERNEL 23
//        2-D Implicit
//   Hydrodynamics Fragment
//---------------------------------

//---------------------------------
// 'Original' C:
//  for ( j=1 ; j<6 ; j++ )
//  {
//    for ( k=1 ; k<n ; k++ )
//    {
//      qa = za[j+1][k]*zr[j][k] + za[j-1][k]*zb[j][k] +
//       za[j][k+1]*zu[j][k] + za[j][k-1]*zv[j][k] + zz[j][k];
//      za[j][k] += 0.175*( qa - za[j][k] );
//    }
//  }
//---------------------------------

sl_def(innerk23, void,
       sl_shfparm(double, ZA_km1_j),
       sl_glparm(long, j),
       sl_glparm(size_t, n),
       sl_glparm(double*restrict, ZA),
       sl_glparm(const double*restrict, ZB),
       sl_glparm(const double*restrict, ZR),
       sl_glparm(const double*restrict, ZU),
       sl_glparm(const double*restrict, ZV),
       sl_glparm(const double*restrict, ZZ),
       sl_glfparm(double, fw)
    )
{
    sl_index(k);
    long j = sl_getp(j);

#define access(C, A, a, b) (*(C double (*restrict)[7][(size_t)sl_getp(n)])(C double*)sl_getp(A))[b][a]
#define ZA(a, b) access( , ZA, a, b)
#define ZB(a, b) access(const, ZB, a, b)
#define ZR(a, b) access(const ,ZR, a, b)
#define ZU(a, b) access(const, ZU, a, b)
#define ZZ(a, b) access(const, ZZ, a, b)
#define ZV(a, b) access(const, ZV, a, b)

    double fw = sl_getp(fw);

    /* nearly copy-paste from fortran, place read to ZA_jm1_j at the end */
    double QA= ZA(k,j+1)*ZR(k,j) +ZA(k,j-1)*ZB(k,j) +
        ZA(k+1,j)*ZU(k,j) +ZZ(k,j);
    double ZA_k_j = ZA(k,j) +fw*(QA -ZA(k,j) + ZV(k,j)*sl_getp(ZA_km1_j));
    sl_setp(ZA_km1_j, ZA_k_j);
    ZA(k,j) = ZA_k_j;
}
sl_enddef

sl_def(outerk23, void, 
       sl_shparm(long, token),
       sl_glparm(size_t, n), 
       sl_glparm(double*restrict, ZA),
       sl_glparm(const double*restrict, ZB),
       sl_glparm(const double*restrict, ZR),
       sl_glparm(const double*restrict, ZU),
       sl_glparm(const double*restrict, ZV),
       sl_glparm(const double*restrict, ZZ),
       sl_glfparm(double, fw)
    )
{
    sl_index(j);

    long temp = sl_getp(token);
    sl_create(,, 1, sl_getp(n),,,,innerk23,
              sl_shfarg(double, , ZA(0, j)),
              sl_glarg(long , , j),
              sl_glarg(size_t, , sl_getp(n)),
              sl_glarg(double*restrict, , sl_getp(ZA))
              , sl_glarg(const double*restrict, , sl_getp(ZB))
              , sl_glarg(const double*restrict, , sl_getp(ZR))
              , sl_glarg(const double*restrict, , sl_getp(ZU))
              , sl_glarg(const double*restrict, , sl_getp(ZV))
              , sl_glarg(const double*restrict, , sl_getp(ZZ))
              , sl_glfarg(double , , sl_getp(fw))
        );
    sl_sync();
    sl_setp(token, temp);
}
sl_enddef

sl_def(kernel23, void,
       sl_glparm(size_t, ncores),
       sl_glparm(size_t, n)
       , sl_glparm(double*restrict, ZA)
       , sl_glparm(size_t, ZA_dim0)
       , sl_glparm(size_t, ZA_dim1)
       , sl_glparm(const double*restrict, ZB)
       , sl_glparm(size_t, ZB_dim0)
       , sl_glparm(size_t, ZB_dim1)
       , sl_glparm(const double*restrict, ZR)
       , sl_glparm(size_t, ZR_dim0)
       , sl_glparm(size_t, ZR_dim1)
       , sl_glparm(const double*restrict, ZU)
       , sl_glparm(size_t, ZU_dim0)
       , sl_glparm(size_t, ZU_dim1)
       , sl_glparm(const double*restrict, ZV)
       , sl_glparm(size_t, ZV_dim0)
       , sl_glparm(size_t, ZV_dim1)
       , sl_glparm(const double*restrict, ZZ)
       , sl_glparm(size_t, ZZ_dim0)
       , sl_glparm(size_t, ZZ_dim1)
    )
{
    svp_assert(sl_getp(ZA_dim0) == sl_getp(n)+1);
    svp_assert(sl_getp(ZB_dim0) == sl_getp(n)+1);
    svp_assert(sl_getp(ZR_dim0) == sl_getp(n)+1);
    svp_assert(sl_getp(ZU_dim0) == sl_getp(n)+1);
    svp_assert(sl_getp(ZV_dim0) == sl_getp(n)+1);
    svp_assert(sl_getp(ZZ_dim0) == sl_getp(n)+1);
    svp_assert(sl_getp(ZA_dim1) == 7);
    svp_assert(sl_getp(ZB_dim1) == 7);
    svp_assert(sl_getp(ZR_dim1) == 7);
    svp_assert(sl_getp(ZU_dim1) == 7);
    svp_assert(sl_getp(ZV_dim1) == 7);
    svp_assert(sl_getp(ZZ_dim1) == 7);

    sl_create(,, 1,6,,2,,outerk23,
              sl_sharg(long, , 0),
              sl_glarg(size_t, , sl_getp(n))
              , sl_glarg(double*restrict, , sl_getp(ZA))
              , sl_glarg(const double*restrict, , sl_getp(ZB))
              , sl_glarg(const double*restrict, , sl_getp(ZR))
              , sl_glarg(const double*restrict, , sl_getp(ZU))
              , sl_glarg(const double*restrict, , sl_getp(ZV))
              , sl_glarg(const double*restrict, , sl_getp(ZZ))
              , sl_glfarg(double, , 0.175)
        );
    sl_sync();
}
sl_enddef
