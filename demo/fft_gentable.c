#include <math.h>
#include <stdio.h>

// #define PI 3.1415926535897932384626433832795
#define PI 0x3.243F6A8885A308D313198A2E03707344A4093822299F31D008p0L

#define INTEGER unsigned long long
#define PRINTF_MOD1 "ll" /* integer modifier */

#define FLOATTYPE long double
#define SIN sinl
#define COS cosl
#define LITTERAL_SUFFIX "L"
#define PRINTF_MOD2 "L" /* fp modifier */

int main(void) {

  unsigned long long k;

  printf("FT sc_table[][2] = { { /*k=0*/ 0, 0 },\n"); 
  for (k = 1; k < 37; ++k) {
    INTEGER LE2    = 1ULL << (k - 1);
    FLOATTYPE d0 = (FLOATTYPE)(PI / LE2);
    printf("{ /* k=%" PRINTF_MOD1 "d, LE2=%" PRINTF_MOD1 "d, PI/LE2=%" PRINTF_MOD2 "a:", k, LE2, d0);
    FLOATTYPE d1 = COS(d0);
    FLOATTYPE d2 = -SIN(d0);
    printf(" cos(PI/LE2), -sin(PI/LE2) =\n"
	   "\t%" PRINTF_MOD2 "e,\t%" PRINTF_MOD2 "e */\n"
	   "\t%" PRINTF_MOD2 "a" LITTERAL_SUFFIX ",\t%" PRINTF_MOD2 "a" LITTERAL_SUFFIX " },\n", 
	   d1, d2, d1, d2);
  }
  printf("};\n");

  return 0;
}
