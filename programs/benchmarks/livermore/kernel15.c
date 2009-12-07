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
//      LIVERMORE KERNEL 15
//        Casual Fortran
//---------------------------------

//---------------------------------
// 'Original' C:
//         ng = 7;
//         nz = n;
//         ar = 0.053;
//         br = 0.073;
//         for ( outer=1 ; outer<ng ; outer++ )
//         {
//             for ( k=1 ; k<nz ; k++ )
//             {
//                 if ( (outer+1) >= ng )
//                 {
//                     vy[outer][k] = 0.0;
//                     continue;
//                 }
//                 if ( vh[outer+1][k] > vh[outer][k] )
//                 {
//                     t = ar;
//                 }
//                 else
//                 {
//                     t = br;
//                 }
//                 if ( vf[outer][k] < vf[outer][k-1] )
//                 {
//                     if ( vh[outer][k-1] > vh[outer+1][k-1] )
//                         r = vh[outer][k-1];
//                     else
//                         r = vh[outer+1][k-1];
//                     s = vf[outer][k-1];
//                 }
//                 else
//                 {
//                     if ( vh[outer][k] > vh[outer+1][k] )
//                         r = vh[outer][k];
//                     else
//                         r = vh[outer+1][k];
//                     s = vf[outer][k];
//                 }
//                 vy[outer][k] = sqrt( vg[outer][k]*vg[outer][k] + r*r )* t/s;
//                 if ( (k+1) >= nz )
//                 {
//                     vs[outer][k] = 0.0;
//                     continue;
//                 }
//                 if ( vf[outer][k] < vf[outer-1][k] )
//                 {
//                     if ( vg[outer-1][k] > vg[outer-1][k+1] )
//                         r = vg[outer-1][k];
//                     else
//                         r = vg[outer-1][k+1];
//                     s = vf[outer-1][k];
//                     t = br;
//                 }
//                 else
//                 {
//                     if ( vg[outer][k] > vg[outer][k+1] )
//                         r = vg[outer][k];
//                     else
//                         r = vg[outer][k+1];
//                     s = vf[outer][k];
//                     t = ar;
//                 }
//                 vs[outer][k] = sqrt( vh[outer][k]*vh[outer][k] + r*r )* t / s;
//             }
//         }
//---------------------------------

#include <cmath.h>

#define NG 7
#define SQRT sqrt

// we "know better" than fmax
#define MAX(a,b) ((a)>(b) ? (a) : (b))
#define AR_init 0.053
#define BR_init 0.073

sl_def(innerk15, void,
       sl_glparm(size_t, n),
       sl_glfparm(double, AR),
       sl_glfparm(double, BR) 
       , sl_glparm(const double*restrict, VF)
       , sl_glparm(size_t, VF_dim0)
       , sl_glparm(const double*restrict, VG)
       , sl_glparm(size_t, VG_dim0)
       , sl_glparm(const double*restrict, VH)
       , sl_glparm(size_t, VH_dim0)
       , sl_glparm(double*restrict, VS)
       , sl_glparm(size_t, VS_dim0)
       , sl_glparm(double*restrict, VY)
       , sl_glparm(size_t, VY_dim0)
    )
{
    sl_index(iter);

    long j = iter / NG + 1;
    long k = iter % NG + 1;

    const double (*restrict VF)[][(size_t)sl_getp(VF_dim0)] = (const double (*)[][(size_t)sl_getp(VF_dim0)])(const double*)sl_getp(VF);
    const double (*restrict VG)[][(size_t)sl_getp(VG_dim0)] = (const double (*)[][(size_t)sl_getp(VG_dim0)])(const double*)sl_getp(VG);
    const double (*restrict VH)[][(size_t)sl_getp(VH_dim0)] = (const double (*)[][(size_t)sl_getp(VH_dim0)])(const double*)sl_getp(VH);
    double (*restrict VS)[][(size_t)sl_getp(VS_dim0)] = (double (*)[][(size_t)sl_getp(VS_dim0)])(double*)sl_getp(VS);
    double (*restrict VY)[][(size_t)sl_getp(VY_dim0)] = (double (*)[][(size_t)sl_getp(VY_dim0)])(double*)sl_getp(VY);

    double AR = sl_getp(AR);
    double BR = sl_getp(BR);

    double T, R, S;

    if ( (j+1) >= NG )
    {
        (*VY)[j][k] = 0.0;
        sl_end_thread;
    }
    
    if ( (*VG)[j+1][k] > (*VH)[j][k] )
        T = AR;
    else
        T = BR;

    if ( (*VF)[j][k] < (*VF)[j][k-1] )
    {
        R = MAX( (*VH)[j][k-1], (*VH)[j+1][k-1] );
        S = (*VF)[j][k-1];
    }
    else
    {
        R = MAX( (*VH)[j][k], (*VH)[j+1][k] );
        S = (*VF)[j][k];
    }

    (*VY)[j][k] = SQRT( (*VG)[j][k] * (*VG)[j][k] + R*R )* T / S;

    if ( (k+1) >= sl_getp(n) )
    {
        (*VS)[j][k] = 0.0;
        sl_end_thread;
    }

    if ( (*VF)[j][k] < (*VF)[j-1][k] )
    {
        R = MAX ( (*VG)[j-1][k], (*VG)[j-1][k+1] );
        S = (*VF)[j-1][k];
        T = BR;
    }
    else
    {
        R = MAX ( (*VG)[j][k], (*VG)[j][k+1] );
        S = (*VF)[j][k];
        T = AR;
    }

    (*VS)[j][k] = SQRT( (*VH)[j][k]*(*VH)[j][k] + R*R )* T / S;

}
sl_enddef

sl_def(kernel15, void,
       sl_glparm(size_t, ncores),
       sl_glparm(size_t, n)
       , sl_glparm(const double*restrict, VF)
       , sl_glparm(size_t, VF_dim0)
       , sl_glparm(size_t, VF_dim1)
       , sl_glparm(const double*restrict, VG)
       , sl_glparm(size_t, VG_dim0)
       , sl_glparm(size_t, VG_dim1)
       , sl_glparm(const double*restrict, VH)
       , sl_glparm(size_t, VH_dim0)
       , sl_glparm(size_t, VH_dim1)
       , sl_glparm(double*restrict, VS)
       , sl_glparm(size_t, VS_dim0)
       , sl_glparm(size_t, VS_dim1)
       , sl_glparm(double*restrict, VY)
       , sl_glparm(size_t, VY_dim0)
       , sl_glparm(size_t, VY_dim1)
    )
{
    sl_create(,,, sl_getp(n)*NG, , ,, innerk15
              , sl_glarg(size_t, , sl_getp(n))
              , sl_glfarg(double, , AR_init)
              , sl_glfarg(double, , BR_init)
              , sl_glarg(const double*, , sl_getp(VF))
              , sl_glarg(size_t, , sl_getp(VF_dim0))
              , sl_glarg(const double*, , sl_getp(VG))
              , sl_glarg(size_t, , sl_getp(VG_dim0))
              , sl_glarg(const double*, , sl_getp(VH))
              , sl_glarg(size_t, , sl_getp(VH_dim0))
              , sl_glarg(double*, , sl_getp(VS))
              , sl_glarg(size_t, , sl_getp(VS_dim0))
              , sl_glarg(double*, , sl_getp(VY))
              , sl_glarg(size_t, , sl_getp(VY_dim0))
        );
    sl_sync();
}
sl_enddef



