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
//      LIVERMORE KERNEL 4
//     Banded Linear Equations
//---------------------------------

//---------------------------------
//     int m = (1001 - 7) / 2;
//     for ( k=6 ; k<1001 ; k=k+m )
//      {
//         lw = k - 6;
//         temp = x[k-1];
//         for ( j=4 ; j<n ; j=j+5 )
//          {
//             temp -= x[lw]*y[j];
//             lw++;
//          }
//         x[k-1] = y[4]*temp;
//      }
//---------------------------------

sl_def(innerk4, void,
       sl_shfparm(double, total),
       sl_glparm(const double*restrict, XZ),
       sl_glparm(const double*restrict, Y),
       sl_glparm(long, i))
{
    sl_index(j);
//    output_char('R',2); output_int(sl_getp(i)+j-6, 2); output_char('\n', 2);
//    output_char('Y',2); output_int(j*5+4, 2); output_char('\n', 2);

    sl_setp(total, sl_getp(XZ)[sl_getp(i) + j - 6] * sl_getp(Y)[j * 5  + 4] + sl_getp(total));
}
sl_enddef

sl_def(outerk4, void,
       sl_glparm(long, ndiv5),
       sl_glparm(double*restrict, XZ),
       sl_glparm(const double*restrict, Y))
{
    sl_index(i);

    sl_create(,, 0, sl_getp(ndiv5), 1,,,innerk4,
              sl_shfarg(double, totalr, 0.0),
              sl_glarg(const double*, , sl_getp(XZ)),
              sl_glarg(const double*, , sl_getp(Y)),
              sl_glarg(long, , i));
    sl_sync();

//    output_char('W',2); output_int(i-1, 2); output_char('\n', 2);

    sl_getp(XZ)[i-1] = sl_getp(Y)[4] * (sl_getp(XZ)[i-1] - sl_geta(totalr));
}
sl_enddef

sl_def(kernel4, void,
       sl_glparm(size_t, ncores),
       sl_glparm(size_t, n),
       sl_glparm(double*restrict, XZ),
       sl_glparm(size_t, XZ_dim),
       sl_glparm(const double*restrict, Y),
       sl_glparm(size_t, Y_dim))
{
//    output_int(sl_getp(XZ_dim), 2); output_char('\n', 2);

    long m = (sl_getp(n)-7) / 2;
    //loop uses an internal stride of 5 by multiplying a 1 stride counter
    //this means the range of the loop must be divided by 5
    long ndiv5 = sl_getp(n) / 5;

    sl_create(,, 6, sl_getp(XZ_dim), m,,, outerk4,
              sl_glarg(long, , ndiv5),
              sl_glarg(double*, , sl_getp(XZ)),
              sl_glarg(const double*, , sl_getp(Y)));
    sl_sync();
}
sl_enddef

