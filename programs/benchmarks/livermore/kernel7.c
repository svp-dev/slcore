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
//      LIVERMORE KERNEL 7
//      equation of state
//          fragment
//---------------------------------

//---------------------------------
// for ( k=0 ; k<n ; k++ )
// {
//   x[k] = u[k] + r*( z[k] + r*y[k] ) +
//    t*( u[k+3] + r*( u[k+2] + r*u[k+1] ) +
//    t*( u[k+6] + q*( u[k+5] + q*u[k+4] ) ) );
//  }
//---------------------------------

//independent loop
sl_def(innerk7, void,
       sl_glparm(double*restrict, X),
       sl_glparm(const double*restrict, U),
       sl_glparm(const double*restrict, Z),
       sl_glparm(const double*restrict, Y),
       sl_glfparm(double, R),
       sl_glfparm(double, T),
       sl_glfparm(double, Q))
{
    sl_index(k);

    sl_getp(X)[k] =    sl_getp(U)[k  ] + sl_getp(R) * ( sl_getp(Z)[k  ] + sl_getp(R) * sl_getp(Y)[k  ] ) +
        sl_getp(T) * ( sl_getp(U)[k+3] + sl_getp(R) * ( sl_getp(U)[k+2] + sl_getp(R) * sl_getp(U)[k+1] ) +
        sl_getp(T) * ( sl_getp(U)[k+6] + sl_getp(Q) * ( sl_getp(U)[k+5] + sl_getp(Q) * sl_getp(U)[k+4] ) ) );
}
sl_enddef

sl_def(kernel,void
       sl_glparm(size_t, ncores),
       sl_glparm(size_t, n)
       , sl_glfparm(double, Q)
       , sl_glfparm(double, R)
       , sl_glfparm(double, T)
       , sl_glparm(const double*restrict, U)
       , sl_glparm(size_t, U_dim0)
       , sl_glparm(double*restrict, X)
       , sl_glparm(size_t, X_dim0)
       , sl_glparm(const double*restrict, Y)
       , sl_glparm(size_t, Y_dim0)
       , sl_glparm(const double*restrict, Z)
       , sl_glparm(size_t, Z_dim0))
{
    sl_create(,,, sl_getp(n),,,, innerk7
              , sl_glarg(double*restrict, , sl_getp(X))
              , sl_glarg(const double*restrict, , sl_getp(U))
              , sl_glarg(const double*restrict, , sl_getp(Z))
              , sl_glarg(const double*restrict, , sl_getp(Y))
              , sl_glfarg(double, , sl_getp(R))
              , sl_glfarg(double, , sl_getp(T))
              , sl_glfarg(double, , sl_getp(Q)));
    sl_sync();
}
sl_enddef
