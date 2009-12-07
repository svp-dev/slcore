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
//      LIVERMORE KERNEL 5
//        tri-diagonal
//   elimination, below diagonal
//---------------------------------

//---------------------------------
// for ( i=1 ; i<n ; i++ )
// {
//  x[i] = z[i]*( y[i] - x[i-1] );
// }
//---------------------------------

sl_def(innerk5,void,
       sl_shfparm(double, prevx),
       sl_glparm(double*restrict, X),
       sl_glparm(const double*restrict, Y),
       sl_glparm(const double*restrict, Z))
{
    sl_index(i);
	
    double newx = sl_getp(Z)[i] * (sl_getp(Y)[i] - sl_getp(prevx));
    sl_setp(prevx, newx);

    sl_getp(X)[i] = newx;
}
sl_enddef

sl_def(kernel5,void,
       sl_glparm(size_t, ncores),
       sl_glparm(size_t, n)
       , sl_glparm(double*restrict, X)
       , sl_glparm(size_t, X_dim0)
       , sl_glparm(const double*restrict, Y)
       , sl_glparm(size_t, Y_dim0)
       , sl_glparm(const double*restrict, Z)
       , sl_glparm(size_t, Z_dim0)
    )
{
    sl_create(,, 1, sl_getp(n), , , , innerk5,
              sl_shfarg(double, , sl_getp(X)[0]),
              sl_glarg(double*, , sl_getp(X)),
              sl_glarg(const double*, ,sl_getp(X)),
              sl_glarg(const double*, , sl_getp(Z)));
    sl_sync();
}
sl_enddef
