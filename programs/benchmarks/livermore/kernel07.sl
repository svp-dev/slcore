m4_define(KERNEL, [[7]])
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
//      LIVERMORE KERNEL 7
//      equation of state
//          fragment
//---------------------------------

//---------------------------------
// for ( k=0 ; k<n ; k++ )
// {
//   x[k] = u[k] + r*( z[k] + r*y[k] ) +
//    t*( u[k+3] + r*( u[k+2] + r*u[k+1] ) +
//    t*( u[k+6] + q*( u[k+5] + q*u[k+4] ) ) );
//  }
//---------------------------------

//requires: x,u,z,y,(r),(t),(q)
//independent loop
sl_def(innerk7, void,
      	sl_glparm(double*,xl),
      	sl_glparm(double*,ul),
      	sl_glparm(double*,zl),
      	sl_glparm(double*,yl),
      	sl_glparm(double,rl),
      	sl_glparm(double,tl),
      	sl_glparm(double,ql))
{
  sl_index(iteration);

  sl_getp(xl)[iteration] = sl_getp(ul)[iteration] + sl_getp(rl)*( sl_getp(zl)[iteration] + sl_getp(rl)*sl_getp(yl)[iteration] ) +
    sl_getp(tl)*( sl_getp(ul)[iteration+3] + sl_getp(rl)*( sl_getp(ul)[iteration+2] + sl_getp(rl)*sl_getp(ul)[iteration+1] ) +
    sl_getp(tl)*( sl_getp(ul)[iteration+6] + sl_getp(ql)*( sl_getp(ul)[iteration+5] + sl_getp(ql)*sl_getp(ul)[iteration+4] ) ) );
}
sl_enddef

sl_def(kernel,void)
{
	sl_create(,, 0,inner[KERNEL],1,blocksize[KERNEL],,innerk7,
		sl_glarg(double*,xx,x),
		sl_glarg(double*,uu,u),
		sl_glarg(double*,zz,z),
		sl_glarg(double*,yy,y),
		sl_glarg(double,rrr,rr),
		sl_glarg(double,tt,t),
		sl_glarg(double,qq,q));
	sl_sync();
}
sl_enddef
