[[]]
//---------------------------------
// Livemore Loops -- SLC (uTC)
// M.A.Hicks, CSA Group, UvA
// Implementation based on various
// reference implementations
// including the original FORTRAN
// but mostly from
// Roy Longbottom, 1996.
//----------------------------------------
//      LIVERMORE KERNEL 2
// Incomplete Cholesky Conjugate Gradient
//----------------------------------------

//---------------------------------
// ii = n;
// ipntp = 0;
// do
//  {
//   ipnt = ipntp;
//   ipntp += ii;
//   ii /= 2;
//   i = ipntp;
//   for ( k=ipnt+1 ; k<ipntp ; k=k+2 )
//     {
//       i++;
//       x[i] = x[k] - v[k]*x[k-1] - v[k+1]*x[k+1];
//     }
//  } while ( ii>0 );
//---------------------------------


sl_def(innerk2,void,
       sl_glparm(double*restrict, X),
       sl_glparm(const double*restrict, V),
       sl_glparm(unsigned long, ipnt),
       sl_glparm(unsigned long, ipntp))
{
    sl_index(i);
    unsigned long ipnt = sl_getp(ipnt);
    unsigned long ipntp = sl_getp(ipntp);
    unsigned long k = ipnt + i;
    double*restrict X = sl_getp(X);
    const double*restrict V = sl_getp(V);
    // output_uint(k,2); output_char('\n',2);
    // output_int(ipntp + i / 2, 2); output_char('\n',2);
    X[ipntp + i / 2] = X[k] - V[k] * X[k-1] - V[k+1] * X[k+1];
}
sl_enddef

sl_def(outerk2, void,
       sl_glparm(double*restrict, X),
       sl_glparm(const double*restrict, V),
       sl_shparm(unsigned long, ii),
       sl_shparm(unsigned long, ipntp))
{
  sl_index(m);

  unsigned long ipnt, ii;
  unsigned long ipntp = (ii = sl_getp(ii)) + (ipnt = sl_getp(ipntp));
  sl_setp(ii, ii/2);
  sl_create(,,1,ii,2,,, innerk2,
	    sl_glarg(double*restrict, , sl_getp(X)),
	    sl_glarg(const double*restrict, , sl_getp(V)),
	    sl_glarg(unsigned long, , ipnt),
	    sl_glarg(unsigned long, , ipntp));
  sl_sync();
  sl_setp(ipntp, ipntp);
}
sl_enddef

#include <cmath.h>

sl_def(kernel2, void,
       sl_glparm(size_t, ncores),
       sl_glparm(size_t, n),
       sl_glparm(const double*restrict, V),
       sl_glparm(size_t, V_dim),
       sl_glparm(double*restrict, X),
       sl_glparm(size_t, X_dim))
{
    // output_int(sl_getp(n), 2); output_char('\n', 2);
    unsigned long upper = log2(sl_getp(n));

    sl_create(,,upper,-1,-1,2,, outerk2,
	      sl_glarg(double*restrict, , sl_getp(X)),
	      sl_glarg(const double*restrict, , sl_getp(V)),
              sl_sharg(unsigned long, ii, sl_getp(n)), 
	      sl_sharg(unsigned long, ipntp, 0));
    sl_sync();
}
sl_enddef
