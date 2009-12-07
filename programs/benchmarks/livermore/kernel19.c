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
//      LIVERMORE KERNEL 19
//  general linear recurrence 
//          equations
//---------------------------------

//---------------------------------
//   kb5i = 0;
//   for ( k=0 ; k<n ; k++ ) {
//     b5[k+kb5i] = sa[k] + stb5*sb[k];
//     stb5 = b5[k+kb5i] - stb5;
//   }
//   for ( i=1 ; i<=n ; i++ ) {
//     k = n - i ;
//     b5[k+kb5i] = sa[k] + stb5*sb[k];
//     stb5 = b5[k+kb5i] - stb5;
//   }
//---------------------------------

//define the data needed needed by this kernel:
//kb5i is apparently always zero, so defined here as such
//and will be optimised out by gcc
#define KB5I 0

sl_def(forwardpass19, void,
       sl_glparm(double*restrict, B5),
       sl_glparm(const double*restrict, SA),
       sl_glparm(const double*restrict, SB),
       sl_shfparm(double, STB5))
{
    sl_index(k);

    double temp = sl_getp(SA)[k] + sl_getp(SB)[k]*sl_getp(STB5);
    sl_setp(STB5, temp - sl_getp(STB5));
    sl_getp(B5)[k+KB5I] = temp;
}
sl_enddef

sl_def(backwardpass19, void,
       sl_glparm(size_t, n),
       sl_glparm(double*restrict, B5),
       sl_glparm(const double*restrict, SA),
       sl_glparm(const double*restrict, SB),
       sl_shfparm(double, STB5))
{
    sl_index(i);
    long k = sl_getp(n) - i;
	
    double temp = sl_getp(SA)[k] + sl_getp(SB)[k]*sl_getp(STB5);
    sl_setp(STB5, temp - sl_getp(STB5));
    sl_getp(B5)[k+KB5I] = temp;
}
sl_enddef

sl_def(kernel19,void,
       sl_glparm(size_t, ncores),
       sl_glparm(size_t, n)
       , sl_glparm(double*restrict, B5)
       , sl_glparm(size_t, B5_dim0)
       , sl_glparm(const double*restrict, SA)
       , sl_glparm(size_t, SA_dim0)
       , sl_glparm(const double*restrict, SB)
       , sl_glparm(size_t, SB_dim0)
       , sl_shfparm(double, STB5)
    )
{
    // calculate array base pointers and values here
    // to avoid calculatoin in each subordinate
    // thread, by passing them as global
    // parameters
    //create the family of the appropriate size
    //specified in the 'inner' array

    sl_create(,, ,sl_getp(n),,,,forwardpass19,
              sl_glarg(double*, ,sl_getp(B5)),
              sl_glarg(const double*, ,sl_getp(SA)),
              sl_glarg(const double*, ,sl_getp(SB)),
              sl_shfarg(double, STB5_1, sl_getp(STB5)));
    sl_sync();
	
    sl_create(,, 1,sl_getp(n)+1,,,,backwardpass19,
              sl_glarg(size_t, , sl_getp(n)),
              sl_glarg(double*, , sl_getp(B5)),
              sl_glarg(const double*, , sl_getp(SA)),
              sl_glarg(const double*, , sl_getp(SB)),
              sl_shfarg(double, STB5_2, sl_geta(STB5_1)));
    sl_sync();
    sl_setp(STB5, sl_geta(STB5_2));
}
sl_enddef
