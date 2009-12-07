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
//      LIVERMORE KERNEL 18
       
//---------------------------------

/* original Fortran:
 1018           T= 0.003700d0
                S= 0.004100d0
               KN= 6
               JN= n
         DO 70  k= 2,KN
         DO 70  j= 2,JN
          ZA(j,k)= (ZP(j-1,k+1)+ZQ(j-1,k+1)-ZP(j-1,k)-ZQ(j-1,k))
     1            *(ZR(j,k)+ZR(j-1,k))/(ZM(j-1,k)+ZM(j-1,k+1))
          ZB(j,k)= (ZP(j-1,k)+ZQ(j-1,k)-ZP(j,k)-ZQ(j,k))
     1            *(ZR(j,k)+ZR(j,k-1))/(ZM(j,k)+ZM(j-1,k))
   70    CONTINUE
c
         DO 72  k= 2,KN
         DO 72  j= 2,JN
          ZU(j,k)= ZU(j,k)+S*(ZA(j,k)*(ZZ(j,k)-ZZ(j+1,k))
     1                    -ZA(j-1,k) *(ZZ(j,k)-ZZ(j-1,k))
     2                    -ZB(j,k)   *(ZZ(j,k)-ZZ(j,k-1))
     3                    +ZB(j,k+1) *(ZZ(j,k)-ZZ(j,k+1)))
          ZV(j,k)= ZV(j,k)+S*(ZA(j,k)*(ZR(j,k)-ZR(j+1,k))
     1                    -ZA(j-1,k) *(ZR(j,k)-ZR(j-1,k))
     2                    -ZB(j,k)   *(ZR(j,k)-ZR(j,k-1))
     3                    +ZB(j,k+1) *(ZR(j,k)-ZR(j,k+1)))
   72    CONTINUE
c
         DO 75  k= 2,KN
         DO 75  j= 2,JN
          ZR(j,k)= ZR(j,k)+T*ZU(j,k)
          ZZ(j,k)= ZZ(j,k)+T*ZV(j,k)
   75    CONTINUE
 */

//---------------------------------
// 'Original' C:
//    for ( l=1 ; l<=loop ; l++ ) {
//         t = 0.0037;
//         s = 0.0041;
//         kn = 6;
//         jn = n;
//         for ( k=1 ; k<kn ; k++ ) {
// #pragma nohazard
//           for ( j=1 ; j<jn ; j++ ) {
//               za[k][j] = ( zp[k+1][j-1] +zq[k+1][j-1] -zp[k][j-1] -zq[k][j-1] )*
//                          ( zr[k][j] +zr[k][j-1] ) / ( zm[k][j-1] +zm[k+1][j-1]);
//               zb[k][j] = ( zp[k][j-1] +zq[k][j-1] -zp[k][j] -zq[k][j] ) *
//                          ( zr[k][j] +zr[k-1][j] ) / ( zm[k][j] +zm[k][j-1]);
//           }
//         }
//         for ( k=1 ; k<kn ; k++ ) {
// #pragma nohazard
//             for ( j=1 ; j<jn ; j++ ) {
//                 zu[k][j] += s*( za[k][j]   *( zz[k][j] - zz[k][j+1] ) -
//                                 za[k][j-1] *( zz[k][j] - zz[k][j-1] ) -
//                                 zb[k][j]   *( zz[k][j] - zz[k-1][j] ) +
//                                 zb[k+1][j] *( zz[k][j] - zz[k+1][j] ) );
//                 zv[k][j] += s*( za[k][j]   *( zr[k][j] - zr[k][j+1] ) -
//                                 za[k][j-1] *( zr[k][j] - zr[k][j-1] ) -
//                                 zb[k][j]   *( zr[k][j] - zr[k-1][j] ) +
//                                 zb[k+1][j] *( zr[k][j] - zr[k+1][j] ) );
//             }
//         }
//         for ( k=1 ; k<kn ; k++ ) {
// #pragma nohazard
//             for ( j=1 ; j<jn ; j++ ) {
//                 zr[k][j] = zr[k][j] + t*zu[k][j];
//                 zz[k][j] = zz[k][j] + t*zv[k][j];
//             }
//         }
//     }
//---------------------------------

#include <svp/abort.h>

sl_def(kernel18, void
   sl_glparm(size_t, ncores),
   sl_glparm(size_t, n)
     , sl_glparm(double*restrict, ZA)
     , sl_glparm(size_t, ZA_dim0)
     , sl_glparm(size_t, ZA_dim1)
     , sl_glparm(double*restrict, ZB)
     , sl_glparm(size_t, ZB_dim0)
     , sl_glparm(size_t, ZB_dim1)
     , sl_glparm(const double*restrict, ZM)
     , sl_glparm(size_t, ZM_dim0)
     , sl_glparm(size_t, ZM_dim1)
     , sl_glparm(const double*restrict, ZP)
     , sl_glparm(size_t, ZP_dim0)
     , sl_glparm(size_t, ZP_dim1)
     , sl_glparm(const double*restrict, ZQ)
     , sl_glparm(size_t, ZQ_dim0)
     , sl_glparm(size_t, ZQ_dim1)
     , sl_glparm(double*restrict, ZR)
     , sl_glparm(size_t, ZR_dim0)
     , sl_glparm(size_t, ZR_dim1)
     , sl_glparm(double*restrict, ZU)
     , sl_glparm(size_t, ZU_dim0)
     , sl_glparm(size_t, ZU_dim1)
     , sl_glparm(double*restrict, ZV)
     , sl_glparm(size_t, ZV_dim0)
     , sl_glparm(size_t, ZV_dim1)
     , sl_glparm(double*restrict, ZZ)
     , sl_glparm(size_t, ZZ_dim0)
     , sl_glparm(size_t, ZZ_dim1)
)
{
	#warning Kernel not implemented!
    output_string("\n!!Kernel Not Implemented!!\n", 2);
    svp_abort();
}
sl_enddef
