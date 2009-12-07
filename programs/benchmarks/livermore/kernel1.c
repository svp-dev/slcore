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
//      LIVERMORE KERNEL 1
//        Hydro Fragment
//---------------------------------

//---------------------------------
// 'Original' C:
//        for ( k=0 ; k<n ; k++ ) {
//            x[k] = q + y[k]*( r*z[k+10] + t*z[k+11] );
//        }
//---------------------------------

//Break down kernel into two families
//this one does the 'meat'
sl_def(innerk1, void,
       sl_glparm(double*restrict, X),
       sl_glfparm(double, Q),
       sl_glparm(const double*restrict, Y),
       sl_glfparm(double, R),
       sl_glparm(const double*restrict, ZX),
       sl_glfparm(double, T) )
{
    sl_index(i);
	
    //now the actual calculation
    sl_getp(X)[i] = sl_getp(Q) + sl_getp(Y)[i] * 
        ( sl_getp(R) * sl_getp(ZX)[i+10] 
          + sl_getp(T) * sl_getp(ZX)[i+11] );
}
sl_enddef

sl_def(kernel1, void,
       sl_glparm(size_t, ncores),
       sl_glparm(size_t, n),
       sl_glfparm(double, Q),
       sl_glfparm(double, R),
       sl_glfparm(double, T),
       sl_glparm(double*restrict, X),
       sl_glparm(size_t, X_dim),
       sl_glparm(const double*restrict, Y),
       sl_glparm(size_t, Y_dim),
       sl_glparm(const double*restrict, ZX),
       sl_glparm(size_t, ZX))
{
    // calculate array base pointers and values here
    // to avoid calculatoin in each subordinate
    // thread, by passing them as global
    // parameters
	
    //create the family of the appropriate size
    //specified in the 'inner' array
    sl_create(,, , sl_getp(n), , , , innerk1,
              sl_glarg(double*, , sl_getp(X)),
              sl_glfarg(double, , sl_getp(Q)),
              sl_glarg(const double*, , sl_getp(Y)),
              sl_glfarg(double, , sl_getp(R)),
              sl_glarg(const double*, , sl_getp(ZX)),
              sl_glfarg(double, , sl_getp(T)));
    sl_sync();

}
sl_enddef
