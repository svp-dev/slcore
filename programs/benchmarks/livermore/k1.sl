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

// muTC/SL implementation courtesy of M.A.Hicks

//Break down kernel into two families
//this one does the 'meat'
sl_def(innerk1, void,
       sl_glparm(double*,xl),sl_glfparm(double,ql),sl_glparm(double*,yl),
       sl_glfparm(double,rrl),sl_glparm(double*,zl),sl_glfparm(double,tl) )
{
  sl_index(iteration);
  
  //now the actual calculation
  sl_getp(xl)[iteration] = sl_getp(ql) + sl_getp(yl)[iteration] * ( sl_getp(rrl)*sl_getp(zl)[iteration+10] + sl_getp(tl)*sl_getp(zl)[iteration+11] );
}
sl_enddef

sl_def(kernel1, void)
{
	// calculate array base pointers and values here
	// to avoid calculatoin in each subordinate
	// thread, by passing them as global
	// parameters
	
	//create the family of the appropriate size
	//specified in the 'inner' array
	sl_create(,, 0,inner[KERNEL],1,blocksize[KERNEL],,innerk1,
     		sl_glarg(double*,xxl,x),
		sl_glfarg(double,qql,q),
		sl_glarg(double*,yyl,y),
		sl_glfarg(double,rrrl,rr),
		sl_glarg(double*,zzl,z),
		sl_glfarg(double,ttl,t));
	sl_sync();
}
sl_enddef
