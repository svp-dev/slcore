//---------------------------------
// Livemore Loops -- SLC (uTC)
// M.A.Hicks, CSA Group, UvA
// Implementation based of various
// reference implementations
// including the original FORTRAN
// but mostly from
// Roy Longbottom, 1996.
//---------------------------------
//      LIVERMORE KERNEL 8
//        ADI Integration
//---------------------------------

double du1[51],du2[51],du3[51];
double a11, a12,a13,a21,a22,a23,a31,a32,a33, sig;
double u1[51][51][51],u2[51][51][51],u3[51][51][51];

sl_def(innerk8,void,
      	sl_glparm(unsigned int, outer),
      	sl_glparm(double*, ldu1),
      	sl_glparm(double*, ldu2),
      	sl_glparm(double*, ldu3),
      	sl_glfparm(double, lsig),
      	sl_glfparm(double, la11),
      	sl_glfparm(double, la12),
      	sl_glfparm(double, la13),
        sl_glfparm(double, la21),
        sl_glfparm(double, la22),
        sl_glfparm(double, la23),
        sl_glfparm(double, la31),
        sl_glfparm(double, la32),
        sl_glfparm(double, la33))
{
	sl_index(inner);
	
	sl_getp(ldu1)[inner] = u1[0][inner+1][sl_getp(outer)] - u1[0][inner-1][sl_getp(outer)];
	sl_getp(ldu2)[inner] = u2[0][inner+1][sl_getp(outer)] - u2[0][inner-1][sl_getp(outer)];
	sl_getp(ldu3)[inner] = u3[0][inner+1][sl_getp(outer)] - u3[0][inner-1][sl_getp(outer)];

	u1[1][inner][sl_getp(outer)]=
		 u1[0][inner][sl_getp(outer)]+sl_getp(la11)*sl_getp(ldu1)[inner]+sl_getp(la12)*sl_getp(ldu2)[inner]+sl_getp(la13)*sl_getp(ldu3)[inner] + sl_getp(lsig) *
		(u1[0][inner][sl_getp(outer)+1]-2.0*u1[0][inner][sl_getp(outer)]+u1[0][inner][sl_getp(outer)-1]);

	u2[1][inner][sl_getp(outer)]=
		u2[0][inner][sl_getp(outer)]+sl_getp(la21)*sl_getp(ldu1)[inner]+sl_getp(la22)*sl_getp(ldu2)[inner]+sl_getp(la23)*sl_getp(ldu3)[inner] + sl_getp(lsig)*
		(u2[0][inner][sl_getp(outer)+1]-2.0*u2[0][inner][sl_getp(outer)]+u2[0][inner][sl_getp(outer)-1]);

	u3[1][inner][sl_getp(outer)]=
		u3[0][inner][sl_getp(outer)]+sl_getp(la31)*sl_getp(ldu1)[inner]+sl_getp(la32)*sl_getp(ldu2)[inner]+sl_getp(la33)*sl_getp(ldu3)[inner] + sl_getp(lsig)*
		(u3[0][inner][sl_getp(outer)+1]-2.0*u3[0][inner][sl_getp(outer)]+u3[0][inner][sl_getp(outer)-1]);
}
sl_enddef
	
sl_def(outerk8,void,
       sl_glparm(double*, lldu1),
       sl_glparm(double*, lldu2),
       sl_glparm(double*, lldu3),
       sl_glfparm(double, llsig),
       sl_glfparm(double, lla11),
       sl_glfparm(double, lla12),
       sl_glfparm(double, lla13),
       sl_glfparm(double, lla21),
       sl_glfparm(double, lla22),
       sl_glfparm(double, lla23),
       sl_glfparm(double, lla31),
       sl_glfparm(double, lla32),
       sl_glfparm(double, lla33))
{
	sl_index(iteration);

	
	sl_create(,, 1,10,1,,,innerk8,
		sl_glarg(unsigned int, oouter,iteration),
		sl_glarg(double*, ldu1,sl_getp(lldu1)),
		sl_glarg(double*, ldu2,sl_getp(lldu2)),
		sl_glarg(double*, ldu3,sl_getp(lldu3)),
		sl_glfarg(double, lsig, sl_getp(llsig)),
		sl_glfarg(double, la11, sl_getp(lla11)),
		sl_glfarg(double, la12, sl_getp(lla12)),
		sl_glfarg(double, la13, sl_getp(lla13)),
		sl_glfarg(double, la21, sl_getp(lla21)),
		sl_glfarg(double, la22, sl_getp(lla22)),
		sl_glfarg(double, la23, sl_getp(lla23)),
		sl_glfarg(double, la31, sl_getp(lla31)),
		sl_glfarg(double, la32, sl_getp(lla32)),
		sl_glfarg(double, la33, sl_getp(lla33)));
	sl_sync();
}
sl_enddef

sl_def(t_main,void)
{

	        sl_create(,, 1,3,1,,,outerk8,
		sl_glarg(double*, ldu1,du1),
		sl_glarg(double*, ldu2,du2),
		sl_glarg(double*, ldu3,du3),
		sl_glfarg(double, lsig, sig),
		sl_glfarg(double, la11, a11),
		sl_glfarg(double, la12, a12),
		sl_glfarg(double, la13, a13),
		sl_glfarg(double, la21, a21),
		sl_glfarg(double, la22, a22),
		sl_glfarg(double, la23, a23),
		sl_glfarg(double, la31, a31),
		sl_glfarg(double, la32, a32),
		sl_glfarg(double, la33, a33));
		sl_sync();
}
sl_enddef
