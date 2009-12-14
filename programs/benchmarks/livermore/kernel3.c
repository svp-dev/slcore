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
//      LIVERMORE KERNEL 3
//        Inner Product
//---------------------------------

//---------------------------------
//         q = 0.0;
//         for ( k=0 ; k<n ; k++ )
//         {
//             q += z[k]*x[k];
// 	   }
//---------------------------------

#ifndef NAIVE
#define REDUCTIONS
#endif

sl_def(innerk3, void,
       sl_shfparm(double, Q),
       sl_glparm(const double*restrict, Z),
       sl_glparm(const double*restrict, X))
{
    sl_index(i);
    
    sl_setp(Q, (sl_getp(Z)[i] * sl_getp(X)[i]) + sl_getp(Q));
}
sl_enddef

#ifdef REDUCTIONS

// method to perform a graph reduction of the above dependent kernel over CORES
sl_def(reductionk3, void,
       sl_shfparm(double, Q),
       sl_glparm(const double*restrict, Z),
       sl_glparm(const double*restrict, X),
       sl_glparm(long, iternum))
{
    sl_index(redindex);
  
    long lower = sl_getp(iternum) * redindex;
    long upper = lower + sl_getp(iternum);

    sl_create(,, lower, upper, 1,,, innerk3,
              sl_shfarg(double, Qr, 0.0),
              sl_glarg(const double*, , sl_getp(Z)),
              sl_glarg(const double*, , sl_getp(X)));	
    sl_sync();
  
    //now accumilate the results
    sl_setp(Q, sl_geta(Qr) + sl_getp(Q) );
}
sl_enddef

#endif

sl_def(kernel3, void,
       sl_glparm(size_t, ncores),
       sl_glparm(size_t, n),
       sl_shfparm(double, Q),
       sl_glparm(const double*restrict, X),
       sl_glparm(size_t, X_dim),
       sl_glparm(const double*restrict, Z),
       sl_glparm(size_t, Z_dim))
{
#ifndef REDUCTIONS
    sl_create(,, 0, sl_getp(n), 1, , , innerk3,
              sl_shfarg(double, Qr, sl_getp(Q)),
              sl_glarg(const double*, , sl_getp(Z)),
              sl_glarg(const double*, , sl_getp(X)));	
    sl_sync();
#else
    //reduction execution
    long blocking = 4; // some arbitrary blocking factor
    long usecores = sl_getp(ncores) * blocking; 
    long span = sl_getp(n) / usecores;

    sl_create(,, , usecores, , blocking, , reductionk3,
              sl_shfarg(double, Qr, sl_getp(Q)),
              sl_glarg(const double*, , sl_getp(Z)),
              sl_glarg(const double*, , sl_getp(X)),
              sl_glarg(long, , span) );
    sl_sync();
#endif

    sl_setp(Q, sl_geta(Qr));
}
sl_enddef
