#include <math.h>
#include <stdio.h>

// #define PI 3.1415926535897932384626433832795
#define PI 0x3.243F6A8885A308D313198A2E03707344A4093822299F31D008p0

int main(void) {

  unsigned long long k;

  printf("FT sc_table[][2] = { { /*k=0*/ 0, 0 },\n"); 
  for (k = 1; k < 37; ++k) {
    unsigned long long LE    = 1LL << k;
    unsigned long long LE2   = LE / 2LL;
    printf("{/*k=%lld,LE=%lld,LE2=%lld:", k, LE, LE2);

    long double d1 = cosl((long double)PI / (long double)LE2);
    long double d2 = -sinl((long double)PI / (long double)LE2);
    printf(" cos(PI/LE2), -sin(PI/LE2) = */ %La, %La },\n", d1, d2);
  }
  printf("};\n");

  return 0;
}
