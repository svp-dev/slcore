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

// muTC/SL implementation courtesy of M.A.Hicks

// NB on Implementation 
// Access to array b[][]
// subscripts are reversed so that
// a pointer (b[]) can be passed to
// innerk6. This avoids a lot of extra
// pointer arithmetic in each leaf thread 

sl_def(innerk6, void,
       sl_shfparm(double, total),
       sl_glparm(double*restrict, wl),
       sl_glparm(double*restrict, bl),
       sl_glparm(int, outer))
{
  sl_index(i);
  
  double temp = sl_getp(total) + (sl_getp(bl)[i] * sl_getp(wl)[(sl_getp(outer)-i)-1]);
  
  sl_setp(total,temp);
  
  //if this is the last thread, write back the value!
  //doing this here exploits the ability of the outer sync
  //to ensure this write has completed
  if (i == (sl_getp(outer) -1))
    sl_getp(wl)[sl_getp(outer)] = temp;
}
sl_enddef

sl_def(outerk6,void,
       sl_shparm(int, syncher),
       sl_glparm(double*restrict, wwl),
       sl_glparm(array2d, bbl))
{
  sl_index(i);	
  int temp;
  
  //creation here needs to be sequentialised
  temp = sl_getp(syncher);
  sl_create(,, 0, i, 1, SHARED_BLOCK,, innerk6,
	    sl_shfarg(double,ttotal, 0.01),
	    sl_glarg(double*restrict, wwwl, sl_getp(wwl)),
	    sl_glarg(double*restrict, bbbl, sl_getp(bbl)[i]),
	    sl_glarg(int,oouter, i));
  sl_sync();
  //sl_getp(wwl)[i] = sl_geta(ttotal);
  //write to synchroniser
  sl_setp(syncher, temp);
}
sl_enddef

// LL_USE: W B
sl_def(kernel6,void)
{
  sl_create(,, 1, inner[KERNEL], 1, SHARED_BLOCK,, outerk6,
	    sl_sharg(int, syncherr, 0),
	    sl_glarg(double*restrict, ww, W),
	    sl_glarg(array2d, bb, B));
  sl_sync();
}
sl_enddef
