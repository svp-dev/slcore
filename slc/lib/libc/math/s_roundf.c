#include "math_private.h"

#define FLT_EPSILON 1.1920928955078125e-07F
static const float toint = 1/FLT_EPSILON;

float roundf(float x)
{
	union {float f; u_int32_t i;} u = {x};
	int e = u.i >> 23 & 0xff;
	float y;

	if (e >= 0x7f+23)
		return x;
	if (u.i >> 31)
		x = -x;
	if (e < 0x7f-1) {
		volatile float __x;
		__x = x + toint;
		return 0*u.f;
	}
	y = x + toint - toint - x;
	if (y > 0.5f)
		y = y + x - 1;
	else if (y <= -0.5f)
		y = y + x + 1;
	else
		y = y + x;
	if (u.i >> 31)
		y = -y;
	return y;
}
