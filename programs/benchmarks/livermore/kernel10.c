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
//      LIVERMORE KERNEL 10
//     Difference Predictors
//---------------------------------

//---------------------------------
// for ( i=0 ; i<n ; i++ )
// {
//   ar        =      cx[i][ 4];
//   br        = ar - px[i][ 4];
//   px[i][ 4] = ar;
//   cr        = br - px[i][ 5];
//   px[i][ 5] = br;
//   ar        = cr - px[i][ 6];
//   px[i][ 6] = cr;
//   br        = ar - px[i][ 7];
//   px[i][ 7] = ar;
//   cr        = br - px[i][ 8];
//   px[i][ 8] = br;
//   ar        = cr - px[i][ 9];
//   px[i][ 9] = cr;
//   br        = ar - px[i][10];
//   px[i][10] = ar;
//   cr        = br - px[i][11];
//   px[i][11] = br;
//   px[i][13] = cr - px[i][12];
//   px[i][12] = cr;
// }
//---------------------------------


sl_def(innerk10, void,
       , sl_glparm(const double*restrict, CX)
       , sl_glparm(size_t, CX_dim0)
       , sl_glparm(double*restrict, PX)
       , sl_glparm(size_t, PX_dim0))
{
    sl_index(k);

    double (*restrict cx)[][sl_getp(CX_dim0)] = (double (*)[][sl_getp(CX_dim0)])sl_getp(CX);
    double (*restrict px)[][sl_getp(PX_dim0)] = (double (*)[][sl_getp(PX_dim0)])sl_getp(PX);

#define CX(a, b) (*cx)[b][a]
#define PX(a, b) (*px)[b][a]

    double AR, AR2, AR3, AR4, BR, BR2, BR3, CR, CR2, CR3;
	
      AR      =      CX(4,k);
    BR      = AR - PX(4,k);
    CR      = BR - PX(5,k);
    AR2      = CR - PX(6,k);
    BR2      = AR2 - PX(7,k);
    CR2      = BR2 - PX(8,k);
    AR3      = CR2 - PX(9,k);
    BR3      = AR3 - PX(10,k);
    CR3      = BR3 - PX(11,k);
    AR4      = CR3 - PX(12, k);
    PX(4,k) = AR;
    PX(5,k) = BR;
    PX(6,k) = CR;
    PX(7,k) = AR2;
    PX(8,k) = BR2;
    PX(9,k)= CR2;
    PX(10,k)= AR3;
    PX(11,k)= BR3;
    PX(12,k)= CR3;
    PX(13,k)= AR4;
}
sl_enddef

sl_def(kernel10,void
       sl_glparm(size_t, ncores),
       sl_glparm(size_t, n)
       , sl_glparm(const double*restrict, CX)
       , sl_glparm(size_t, CX_dim0)
       , sl_glparm(size_t, CX_dim1)
       , sl_glparm(double*restrict, PX)
       , sl_glparm(size_t, PX_dim0)
       , sl_glparm(size_t, PX_dim1)
    )
{
    sl_create(,, ,sl_getp(n),,0,,innerk10,
              sl_glarg(const double*, , sl_getp(CX)),
              sl_glarg(size_t, , sl_getp(CX_dim0)),
              sl_glarg(double*, , sl_getp(PX)),
              sl_glarg(size_t, , sl_getp(PX_dim0)));
    sl_sync();	
}
sl_enddef
