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
//      LIVERMORE KERNEL 12
//       First Difference
//---------------------------------

//---------------------------------
// for ( k=0 ; k<n ; k++ )
// {
//   x[k] = y[k+1] - y[k];
// }
//---------------------------------

sl_def(innerk12,void,
       sl_glparm(double*restrict, X),
       sl_glparm(const double*restrict, Y))
{
    sl_index(k);
    sl_getp(X)[k] = sl_getp(Y)[k+1] - sl_getp(Y)[k];
}
sl_enddef

sl_def(kernel,void,
       sl_glparm(size_t, ncores),
       sl_glparm(size_t, n)
       , sl_glparm(double*restrict, X)
       , sl_glparm(size_t, X_dim0)
       , sl_glparm(const double*restrict, Y)
       , sl_glparm(size_t, Y_dim0)
    )
{
    sl_create(,, ,sl_getp(n),,0,,innerk12,
              sl_glarg(double*, , sl_getp(X)),
              sl_glarg(const double*, , sl_getp(Y)));
    sl_sync();
}
sl_enddef
