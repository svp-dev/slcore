//---------------------------------
//      LIVERMORE KERNEL 8
//        ADI Integration
//---------------------------------

//---------------------------------
//    nl1 = 0;
//    nl2 = 1;
// for ( kx=1 ; kx<3 ; kx++ )
// {
//  for ( ky=1 ; ky<n ; ky++ )
//  {
//    du1[ky] = u1[nl1][ky+1][kx] - u1[nl1][ky-1][kx];
//    
//    du2[ky] = u2[nl1][ky+1][kx] - u2[nl1][ky-1][kx];
//    
//    du3[ky] = u3[nl1][ky+1][kx] - u3[nl1][ky-1][kx];
//    
//    u1[nl2][ky][kx]=
//     u1[nl1][ky][kx]+a11*du1[ky]+a12*du2[ky]+a13*du3[ky] + sig*
//     (u1[nl1][ky][kx+1]-2.0*u1[nl1][ky][kx]+u1[nl1][ky][kx-1]);
//               
//    u2[nl2][ky][kx]=
//     u2[nl1][ky][kx]+a21*du1[ky]+a22*du2[ky]+a23*du3[ky] + sig*
//     (u2[nl1][ky][kx+1]-2.0*u2[nl1][ky][kx]+u2[nl1][ky][kx-1]);
//               
//     u3[nl2][ky][kx]=
//      u3[nl1][ky][kx]+a31*du1[ky]+a32*du2[ky]+a33*du3[ky] + sig*
//     (u3[nl1][ky][kx+1]-2.0*u3[nl1][ky][kx]+u3[nl1][ky][kx-1]);
//   }
// }
//---------------------------------

// muTC/SL implementation courtesy of M.A.Hicks

// LL_USE: Du1 Du2 Du3 Sig A11 A12 A13 A21 A22 A23 A31 A32 A33 U1 U2 U3

sl_def(innerk8,void,
       sl_glparm(unsigned int, outer),
       sl_glparm(double*restrict, ldu1),
       sl_glparm(double*restrict, ldu2),
       sl_glparm(double*restrict, ldu3),
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
	
  sl_getp(ldu1)[inner] = U1[0][inner+1][sl_getp(outer)] - U1[0][inner-1][sl_getp(outer)];
  sl_getp(ldu2)[inner] = U2[0][inner+1][sl_getp(outer)] - U2[0][inner-1][sl_getp(outer)];
  sl_getp(ldu3)[inner] = U3[0][inner+1][sl_getp(outer)] - U3[0][inner-1][sl_getp(outer)];
  
  U1[1][inner][sl_getp(outer)] =
    U1[0][inner][sl_getp(outer)] + 
    sl_getp(la11) * sl_getp(ldu1)[inner] + 
    sl_getp(la12) * sl_getp(ldu2)[inner] + 
    sl_getp(la13) * sl_getp(ldu3)[inner] + 
    sl_getp(lsig) * (U1[0][inner][sl_getp(outer)+1] - 
		     2.0*U1[0][inner][sl_getp(outer)] + 
		     U1[0][inner][sl_getp(outer)-1]);
  
  U2[1][inner][sl_getp(outer)] =
    U2[0][inner][sl_getp(outer)] + 
    sl_getp(la21) * sl_getp(ldu1)[inner] + 
    sl_getp(la22) * sl_getp(ldu2)[inner] + 
    sl_getp(la23) * sl_getp(ldu3)[inner] + 
    sl_getp(lsig) * (U2[0][inner][sl_getp(outer)+1] - 
		     2.0*U2[0][inner][sl_getp(outer)] + 
		     U2[0][inner][sl_getp(outer)-1]);
  
  U3[1][inner][sl_getp(outer)] =
    U3[0][inner][sl_getp(outer)] + 
    sl_getp(la31) * sl_getp(ldu1)[inner] + 
    sl_getp(la32) * sl_getp(ldu2)[inner] + 
    sl_getp(la33) * sl_getp(ldu3)[inner] + 
    sl_getp(lsig) * (U3[0][inner][sl_getp(outer)+1] - 
		     2.0*U3[0][inner][sl_getp(outer)] + 
		     U3[0][inner][sl_getp(outer)-1]);
}
sl_enddef


sl_def(outerk8,void,
       sl_glparm(double*restrict, lldu1),
       sl_glparm(double*restrict, lldu2),
       sl_glparm(double*restrict, lldu3),
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
	sl_index(i);


	sl_create(,, 1, inner[KERNEL], 1,,, innerk8,
		  sl_glarg(unsigned int, oouter, i),
		  sl_glarg(double*restrict, ldu1, sl_getp(lldu1)),
		  sl_glarg(double*restrict, ldu2, sl_getp(lldu2)),
		  sl_glarg(double*restrict, ldu3, sl_getp(lldu3)),
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

sl_def(kernel8,void)
{
  sl_create(,, 1,3,1,blocksize[KERNEL],,outerk8,
	    sl_glarg(double*restrict, ldu1, Du1),
	    sl_glarg(double*restrict, ldu2, Du2),
	    sl_glarg(double*restrict, ldu3, Du3),
	    sl_glfarg(double, lsig, *Sig),
	    sl_glfarg(double, la11, *A11),
	    sl_glfarg(double, la12, *A12),
	    sl_glfarg(double, la13, *A13),
	    sl_glfarg(double, la21, *A21),
	    sl_glfarg(double, la22, *A22),
	    sl_glfarg(double, la23, *A23),
	    sl_glfarg(double, la31, *A31),
	    sl_glfarg(double, la32, *A32),
	    sl_glfarg(double, la33, *A33));
  sl_sync();
}
sl_enddef
