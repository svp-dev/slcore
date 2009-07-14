//m4_define([[SHFWA]]) //shared float work around
m4_define(KERNEL, [[3]])
m4_include(livermore.slh)
//---------------------------------
// Livemore Loops -- SLC (uTC)
// M.A.Hicks, CSA Group, UvA
// Implementation based of various
// reference implementations
// including the original FORTRAN
// but mostly from
// Roy Longbottom, 1996.
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


sl_def(innerk3, void,
        sl_shfparm(double,ql),sl_glparm(double*,zl),sl_glparm(double*,xl))
{
	sl_index(iteration);

	sl_setp(ql, sl_getp(ql) + (sl_getp(zl)[iteration]*sl_getp(xl)[iteration]) );

}
sl_enddef

sl_def(kernel, void)
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
