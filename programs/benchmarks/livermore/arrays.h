// Kernel Data:
// Modelled after R.Longbottom
double U[LARGEST];
double V[LARGEST];
double W[LARGEST];
double X[LARGEST];
double Y[LARGEST];
double Z[LARGEST+10]; // plus 10 -- read past LARGEST in some loops
double G[LARGEST];
double Du1[101];
double Du2[101];
double Du3[101];
double Grd[LARGEST];
double Dex[LARGEST];
double Xi[LARGEST];
double Ex[LARGEST];
double Ex1[LARGEST];
double Dex1[LARGEST];
double Vx[LARGEST];
double Xx[LARGEST];
double Rx[LARGEST];
double Rh[2*LARGEST];
double Vsp[101];
double Vstp[101];
double Vxne[101];
double Vxnd[101];
double Ve3[101];
double Vlr[101];
double Vlin[101];
double B5[101];
double Plan[300];
double D[300];
double Sa[101];
double Sb[101];
double P[LARGEST][LARGEST];
double Px[LARGEST][LARGEST];
double Cx[LARGEST][LARGEST]; // loop10
double Vy[25][101];
double Vh[7][101];
double Vf[7][101];
double Vg[7][101];
double Vs[7][101];
double Za[7][101];
double Zp[7][101];
double Zq[7][101];
double Zr[7][101];
double Zm[7][101];
double Zb[7][101];
double Zu[7][101];
double Zv[7][101];
double Zz[7][101];
double B[LARGEST][LARGEST]; // currently just used by loop 6 & 10
double C[64][64];
double H[LARGEST][LARGEST];
double U1[2][LARGEST][5]; // loop8
double U2[2][LARGEST][5]; // loop8
double U3[2][LARGEST][5]; // loop8
long   E[96];
long   F[96];
long   Ix[LARGEST];
long   Ir[LARGEST];
long   Zone[301];
static double*const Q = &Xtra[28];
static double*const R = &Xtra[30];
static double*const T = &Xtra[36];
static double*const Dm22 = &Xtra[16];
static double*const Dm23 = &Xtra[17];
static double*const Dm24 = &Xtra[18];
static double*const Dm25 = &Xtra[19];
static double*const Dm26 = &Xtra[20];
static double*const Dm27 = &Xtra[21];
static double*const Dm28 = &Xtra[22];
static double*const C0 = &Xtra[12];
static double*const A11 = &Xtra[1];
static double*const A12 = &Xtra[2];
static double*const A13 = &Xtra[3];
static double*const A21 = &Xtra[4];
static double*const A22 = &Xtra[5];
static double*const A23 = &Xtra[6];
static double*const A31 = &Xtra[7];
static double*const A32 = &Xtra[8];
static double*const A33 = &Xtra[9];
static double*const Sig = &Xtra[9];
static double*const Flx = &Xtra[27];
