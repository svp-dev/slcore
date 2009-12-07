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
//      LIVERMORE KERNEL 21
//    Matrix*Matrix Product
//---------------------------------

//---------------------------------
// 'Original' C:
// for ( k=0 ; k<25 ; k++ )
// {
//   for ( i=0 ; i<25 ; i++ )
//   {
//      for ( j=0 ; j<n ; j++ )
//      {
//          px[j][i] += vy[k][i] * cx[j][k];
//      }
//   }
// }
//---------------------------------

//cell to calculate is computed by taking the remainder
//and result of dividing the thread index by the matrix
//width. 
sl_def(cell, void,
       sl_glparm(size_t, n),
       sl_glparm(const double*restrict, VY),
       sl_glparm(const double*restrict, CX),
       sl_glparm(double*restrict, PX))
{
    sl_index(jk);
    
    double (*restrict PX)[(size_t)sl_getp(n)][25] = (double (*)[(size_t)sl_getp(n)][25])(double*)sl_getp(PX);
    const double (*restrict CX)[(size_t)sl_getp(n)][25] = (const double (*)[(size_t)sl_getp(n)][25])(const double*)sl_getp(CX);
    const double (*restrict VY)[25][(size_t)sl_getp(n)] = (const double (*)[25][(size_t)sl_getp(n)])(const double*)sl_getp(VY);

    long k = jk / 25;
    long j = jk % 25;
    double CX_kj = (*CX)[j][k];

    //N.B. can easily make the following into a new family,
    //but requires a reduction over the number of cores
    long i;
    double px_ij = 0.0;
    for (i = 0; i < sl_getp(n); ++i)
        px_ij += (*VY)[k][i] * CX_kj;
    //save result
    (*PX)[j][i] = px_ij;
}
sl_enddef

sl_def(kernel21, void,
       sl_glparm(size_t, ncores),
       sl_glparm(size_t, n)
       , sl_glparm(const double*restrict, CX)
       , sl_glparm(size_t, CX_dim0)
       , sl_glparm(size_t, CX_dim1)
       , sl_glparm(double*restrict, PX)
       , sl_glparm(size_t, PX_dim0)
       , sl_glparm(size_t, PX_dim1)
       , sl_glparm(const double*restrict, VY)
       , sl_glparm(size_t, VY_dim0)
       , sl_glparm(size_t, VY_dim1)
    )
{
    svp_assert(sl_getp(PX_dim1) == 25);
    svp_assert(sl_getp(CX_dim1) == 25);
    svp_assert(sl_getp(VY_dim0) == 25);

    //create the family of the appropriate size
    //specified in the 'inner' array
    sl_create(,, 0, 25*25,1, 0,, cell,
              sl_glarg(size_t, , sl_getp(n)),
              sl_glarg(const double*, , sl_getp(VY)),
              sl_glarg(const double*, , sl_getp(CX)),
              sl_glarg(double*, , sl_getp(PX)));
    sl_sync();
}
sl_enddef

