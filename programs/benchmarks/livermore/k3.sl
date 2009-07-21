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

// muTC/SL implementation courtesy of M.A.Hicks

sl_def(innerk3, void,
        sl_shfparm(double,ql),sl_glparm(double*,zl),sl_glparm(double*,xl))
{
	sl_index(iteration);

	sl_setp(ql, sl_getp(ql) + (sl_getp(zl)[iteration]*sl_getp(xl)[iteration]) );

}
sl_enddef

sl_def(kernel3, void)
{
	//create the kernel, not forgetting to initialise
	//q to zero
	sl_create(,, 0,inner[KERNEL],1,SHARED_BLOCK,,innerk3,
		     sl_shfarg(double, qql,0.0),
		     sl_glarg(double*,zzl, z),
		     sl_glarg(double*,xxl, x));	
	sl_sync();
	
	//now get the returned shared and place it
	//into the data store
	q = sl_geta(qql);
}
sl_enddef
