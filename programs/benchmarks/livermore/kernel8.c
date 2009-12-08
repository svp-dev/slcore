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

sl_def(innerk8,void,
       sl_glfparm(double, fw)
       , sl_glfparm(double, A11)
       , sl_glfparm(double, A12)
       , sl_glfparm(double, A13)
       , sl_glfparm(double, A21)
       , sl_glfparm(double, A22)
       , sl_glfparm(double, A23)
       , sl_glfparm(double, A31)
       , sl_glfparm(double, A32)
       , sl_glfparm(double, A33)
       , sl_glfparm(double, SIG)
       , sl_glparm(double*restrict, U1)
       , sl_glparm(size_t, U1_dim1)
       , sl_glparm(double*restrict, U2)
       , sl_glparm(size_t, U2_dim1)
       , sl_glparm(double*restrict, U3)
       , sl_glparm(size_t, U3_dim1))
{
    sl_index(i);
    long kx = i % 2 + 1;
    long ky = i / 2 + 1;
    double SIG = sl_getp(SIG);
    const double fw = sl_getp(fw);
    const long nl1 = 0;
    const long nl2 = 1;

#define accessU(N, a, b, c)                                             \
    sl_getp(U##N)[ ((c)*sl_getp(U##N##_dim1)+(b))*4 + (a)]

#define U1(a, b, c) accessU(1, a, b, c)
#define U2(a, b, c) accessU(2, a, b, c)
#define U3(a, b, c) accessU(3, a, b, c)

    double A11 = sl_getp(A11);
    double A12 = sl_getp(A12);
    double A13 = sl_getp(A13);
    double A21 = sl_getp(A21);
    double A22 = sl_getp(A22);
    double A23 = sl_getp(A23);
    double A31 = sl_getp(A31);
    double A32 = sl_getp(A32);
    double A33 = sl_getp(A33);
        
        
    /* copy-paste from fortran */
    double du1 = U1(kx,ky+1,nl1)  -  U1(kx,ky-1,nl1);
    double du2 = U2(kx,ky+1,nl1)  -  U2(kx,ky-1,nl1);
    double du3 = U3(kx,ky+1,nl1)  -  U3(kx,ky-1,nl1);

    U1(kx,ky,nl2) = U1(kx,ky,nl1) + A11*du1 + A12*du2 + A13*du3
        + SIG*(U1(kx+1,ky,nl1) - fw*U1(kx,ky,nl1) + U1(kx-1,ky,nl1));
    U2(kx,ky,nl2) = U2(kx,ky,nl1) + A21*du1 + A22*du2 + A23*du3
        + SIG*(U2(kx+1,ky,nl1) - fw*U2(kx,ky,nl1) + U2(kx-1,ky,nl1));
    U3(kx,ky,nl2) = U3(kx,ky,nl1) + A31*du1 + A32*du2 + A33*du3
        + SIG*(U3(kx+1,ky,nl1) - fw*U3(kx,ky,nl1) + U3(kx-1,ky,nl1));

}
sl_enddef


sl_def(kernel8,void,
       sl_glparm(size_t, ncores),
       sl_glparm(size_t, n)
       , sl_glfparm(double, A11)
       , sl_glfparm(double, A12)
       , sl_glfparm(double, A13)
       , sl_glfparm(double, A21)
       , sl_glfparm(double, A22)
       , sl_glfparm(double, A23)
       , sl_glfparm(double, A31)
       , sl_glfparm(double, A32)
       , sl_glfparm(double, A33)
       , sl_glfparm(double, SIG)
       , sl_glparm(double*restrict, U1)
       , sl_glparm(size_t, U1_dim0)
       , sl_glparm(size_t, U1_dim1)
       , sl_glparm(size_t, U1_dim2)
       , sl_glparm(double*restrict, U2)
       , sl_glparm(size_t, U2_dim0)
       , sl_glparm(size_t, U2_dim1)
       , sl_glparm(size_t, U2_dim2)
       , sl_glparm(double*restrict, U3)
       , sl_glparm(size_t, U3_dim0)
       , sl_glparm(size_t, U3_dim1)
       , sl_glparm(size_t, U3_dim2))
{
    svp_assert(sl_getp(U1_dim0) == 2);
    svp_assert(sl_getp(U2_dim0) == 2);
    svp_assert(sl_getp(U3_dim0) == 2);
    svp_assert(sl_getp(U1_dim2) == 4);
    svp_assert(sl_getp(U2_dim2) == 4);
    svp_assert(sl_getp(U3_dim2) == 4);
    sl_create(,, , sl_getp(n)*2, , ,, innerk8
              , sl_glfarg(double, , 2.0)
              , sl_glfarg(double, , sl_getp(A11))
              , sl_glfarg(double, , sl_getp(A12))
              , sl_glfarg(double, , sl_getp(A13))
              , sl_glfarg(double, , sl_getp(A21))
              , sl_glfarg(double, , sl_getp(A22))
              , sl_glfarg(double, , sl_getp(A23))
              , sl_glfarg(double, , sl_getp(A31))
              , sl_glfarg(double, , sl_getp(A32))
              , sl_glfarg(double, , sl_getp(A33))
              , sl_glfarg(double, , sl_getp(SIG))
              , sl_glarg(double*restrict, , sl_getp(U1))
              , sl_glarg(size_t, , sl_getp(U1_dim1))
              , sl_glarg(double*restrict, , sl_getp(U2))
              , sl_glarg(size_t, , sl_getp(U2_dim1))
              , sl_glarg(double*restrict, , sl_getp(U3))
              , sl_glarg(size_t, , sl_getp(U3_dim1))
        );
    sl_sync();

}
sl_enddef
