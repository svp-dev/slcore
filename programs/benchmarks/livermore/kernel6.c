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
//      LIVERMORE KERNEL 6
//         general linear
//      recurrence equations
//---------------------------------

//---------------------------------
// for ( i=1 ; i<n ; i++ )
//  {
//     w[i] = 0.01;
//     for ( k=0 ; k<i ; k++ )
//     {
//        w[i] += b[k][i] * w[(i-k)-1];
//     }
//  }
//---------------------------------

sl_def(innerk6,void,
       sl_shfparm(double, Wi),
       sl_glparm(double*restrict, W),
       sl_glparm(const double*restrict,B),
       sl_glparm(size_t, n),
       sl_glparm(long, i))
{
    sl_index(k);
    long i = sl_getp(i);

    const size_t n = sl_getp(n);
    const double (*restrict B)[n][n] = (const double(*)[n][n])(const double*)sl_getp(B);

    double temp = ((*B)[k][i] * sl_getp(W)[i-k-1]) + sl_getp(Wi);
    sl_setp(Wi, temp);

    //if this is the last thread, write back the value!
    //doing this here exploits the ability of the outer sync
    //to ensure this write has completed
    if (k == i - 1)
        sl_getp(W)[i] = temp;
}
sl_enddef

sl_def(outerk6,void,
       sl_shparm(long, token),
       sl_glfparm(double, Wi_init),
       sl_glparm(double*restrict, W),
       sl_glparm(const double*restrict, B),
       sl_glparm(size_t, n))
{
    sl_index(i);

    //creation here needs to be sequentialised
    long temp = sl_getp(token);
    sl_create(,, 0, i, 1,,, innerk6,
              sl_shfarg(double, Wi, sl_getp(Wi_init)),
              sl_glarg(double*, , sl_getp(W)),
              sl_glarg(const double*, , sl_getp(B)),
              sl_glarg(size_t, , sl_getp(n)),
              sl_glarg(long, , i));
    sl_sync();

    // FIXME: we assume here that the write to W[i] by the last
    // thread innerk6 is completed and made visible to the next thread
    // of the outerk6 after this sync... 

    sl_setp(token, temp);
}
sl_enddef

sl_def(kernel6, void,
       sl_glparm(size_t, ncores),
       sl_glparm(size_t, n)
       , sl_glparm(const double*restrict, B)
       , sl_glparm(size_t, B_dim0)
       , sl_glparm(size_t, B_dim1)
       , sl_glparm(double*restrict, W)
       , sl_glparm(size_t, W_dim0)
    )
{
    svp_assert(sl_getp(B_dim0) == sl_getp(n));
    svp_assert(sl_getp(B_dim1) == sl_getp(n));
    svp_assert(sl_getp(W_dim0) == sl_getp(n));
    sl_create(,, 1, sl_getp(n), , 2, , outerk6,
              sl_sharg(long, , 0),
              sl_glfarg(double, , 0.01),
              sl_glarg(double*, , sl_getp(W)),
              sl_glarg(const double*, ,sl_getp(B)),
              sl_glarg(size_t, , sl_getp(n)));
    sl_sync();
}
sl_enddef
