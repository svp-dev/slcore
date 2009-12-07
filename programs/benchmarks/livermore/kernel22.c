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
//      LIVERMORE KERNEL 22
//    Planckian distribution
//---------------------------------

//---------------------------------
// 'Original' C:
//    expmax = 20.0;
//     z[n-1] = 0.99*expmax*v[n-1];
// 
//         for ( k=0 ; k<n ; k++ )
//         {
//             y[k] = z[k] / v[k];
//             dex[k] = x[k] / ( exp( y[k] ) -1.0 );
//         }
//---------------------------------

#include <cmath.h>

sl_def(innerk22, void,
       sl_glfparm(double, fw)
       , sl_glparm(double*restrict, U)
       , sl_glparm(const double*restrict, V)
       , sl_glparm(double*restrict, W)
       , sl_glparm(const double*restrict, X)
       , sl_glparm(double*restrict, Y)
    )
{
    sl_index(k);
    const double fw = sl_getp(fw);
    const double X_k = sl_getp(X)[k];
    const double Y_k = sl_getp(U)[k] / sl_getp(V)[k];
    const double W_k = X_k / ( exp ( Y_k ) - fw );
    sl_getp(W)[k] = W_k;
    sl_getp(Y)[k] = Y_k;
}
sl_enddef

sl_def(kernel22, void,
       sl_glparm(size_t, ncores),
       sl_glparm(size_t, n)
       , sl_glparm(double*restrict, U)
       , sl_glparm(size_t, U_dim0)
       , sl_glparm(const double*restrict, V)
       , sl_glparm(size_t, V_dim0)
       , sl_glparm(double*restrict, W)
       , sl_glparm(size_t, W_dim0)
       , sl_glparm(const double*restrict, X)
       , sl_glparm(size_t, X_dim0)
       , sl_glparm(double*restrict, Y)
       , sl_glparm(size_t, Y_dim0)
    )
{
    const double fw = 2.0;
    const double expmax = 20.0;
    sl_getp(U)[sl_getp(n)-1] = 0.99 * expmax * sl_getp(V)[sl_getp(n)-1];

    sl_create(,, , sl_getp(n),,,,innerk22,
              sl_glfarg(double, , fw)
              , sl_glarg(double*restrict, , sl_getp(U))
              , sl_glarg(const double*restrict, , sl_getp(V))
              , sl_glarg(double*restrict, , sl_getp(W))
              , sl_glarg(const double*restrict, , sl_getp(X))
              , sl_glarg(double*restrict, , sl_getp(Y)));
    sl_sync();
}
sl_enddef
