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

#ifndef NAIVE_CODE
#define REDUCTIONS
#endif

sl_def(innerk4, void,
       sl_shfparm(double, total),
       sl_glparm(const double*restrict, XZ),
       sl_glparm(const double*restrict, Y),
       sl_glparm(long, lw))
{
    sl_index(i);
    //j -> 5step with a starting value of 4
    sl_setp(total, -(sl_getp(XZ)[sl_getp(lw)+i] * sl_getp(Y)[(i*5) + 4]) + sl_getp(total));
}
sl_enddef

#ifdef REDUCTIONS
//method to perform a graph reduction of the above dependent kernel over CORES
sl_def(reductionk4, void,
       sl_shfparm(double, total),
       sl_glparm(const double*, XZ),
       sl_glparm(const double*, Y),
       sl_glparm(long, lw),
       sl_glparm(long, iternum))
{
    sl_index(redindex);
  
    long lower = sl_getp(iternum) * redindex;
    long upper = lower + sl_getp(iternum);
	
    if (redindex == 0) lower=4;

    sl_create(,, lower, upper, 1,,, innerk4,
              sl_shfarg(double, totalr, sl_getp(XZ)[sl_getp(lw)+5]),
              sl_glarg(const double*, , sl_getp(XZ)),
              sl_glarg(const double*, , sl_getp(Y)),
              sl_glarg(long, , sl_getp(lw)));
    sl_sync();

    //now accumulate the results
    sl_setp(total, -sl_geta(totalr) + sl_getp(total));

}
sl_enddef
#endif

sl_def(outerk4, void,
#ifdef REDUCTIONS
       sl_glparm(size_t, ncores4),
       sl_glparm(long, span),
#endif
       sl_glparm(long, range),
       sl_glparm(double*restrict, XZ),
       sl_glparm(const double*restrict, Y))
{
    sl_index(k);
    unsigned int lw = k - 6;
	
	
    //check that previous loop iteration has finished
    //because innerk4 reads previous result from xl[]
//	int temp = sl_getp(writelock);

    //loop uses an internal stride of 5 by multiplying a 1 stride counter
    //this means the range of the loop must be divided by 5
	

#ifndef REDUCTIONS	
    sl_create(,, 0, sl_getp(range),1,,,innerk4,
              sl_shfarg(double, totalr, sl_getp(XZ)[k-1]),
              sl_glarg(const double*, ,sl_getp(XZ)),
              sl_glarg(const double*, , sl_getp(Y)),
              sl_glarg(long, , lw));
    sl_sync();
#else
    sl_create(,, 0, sl_getp(ncores4), 1, 4,, reductionk4,
              sl_shfarg(double, totalr, sl_getp(XZ)[k-1]),
              sl_glarg(const double*, ,sl_getp(XZ)),
              sl_glarg(const double*, , sl_getp(Y)),
              sl_glarg(unsigned int, ,lw),
              sl_glarg(long, , sl_getp(span)));
    sl_sync();
#endif

    sl_getp(XZ)[k-1] = sl_getp(Y)[4] * sl_geta(totalr);
    //now set this lock as free by writing to shared, since xl[]
    // has been written safely.
//	sl_setp(writelock, temp + 1);
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
    long m = (1001-7) / 2;

#ifdef REDUCTIONS
    long ncores4 = sl_getp(ncores) * 4;
    ncores4 = ncores4 ? ncores4 : 4;
    //loop uses an internal stride of 5 by multiplying a 1 stride counter
    //this means the range of the loop must be divided by 5
    long range = sl_getp(n) / 5;
    long span = range / ncores4;
#endif

    sl_create(,, 6, 1001, m, 2,, outerk4,
#ifdef REDUCTIONS
              sl_glarg(size_t, , ncores4),
              sl_glarg(long, , span),
#endif
              sl_glarg(long, , range),
              sl_glarg(double*, , sl_getp(XZ)),
              sl_glarg(const double*, , sl_getp(Y)));
    sl_sync();
}
sl_enddef

