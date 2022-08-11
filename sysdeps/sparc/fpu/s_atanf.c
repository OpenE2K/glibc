/*
 * Copyright 12/11/00 Sun Microsystems, Inc.  All Rights Reserved.
 */
/* float atanf(float x);
 * Table look-up algorithm
 * By K.C. Ng, March 9, 1989
 *
 * Algorithm.
 *
 * The algorithm is based on atan(x)=atan(y)+atan((x-y)/(1+x*y)).
 * We use poly1(x) to approximate atan(x) for x in [0,1/8] with
 * error (relative)
 * 	|(atan(x)-poly1(x))/x|<= 2^-115.94 	long double
 * 	|(atan(x)-poly1(x))/x|<= 2^-58.85	double
 * 	|(atan(x)-poly1(x))/x|<= 2^-25.53 	float
 * and use poly2(x) to approximate atan(x) for x in [0,1/65] with
 * error (absolute)
 *	|atan(x)-poly2(x)|<= 2^-122.15		long double
 *	|atan(x)-poly2(x)|<= 2^-64.79		double
 *	|atan(x)-poly2(x)|<= 2^-35.36		float
 * and use poly3(x) to approximate atan(x) for x in [1/8,7/16] with
 * error (relative, on for single precision)
 * 	|(atan(x)-poly1(x))/x|<= 2^-25.53 	float
 *
 * Here poly1-3 are odd polynomial with the following form:
 *		x + x^3*(a1+x^2*(a2+...))
 *
 * (0). Purge off Inf and NaN and 0
 * (1). Reduce x to positive by atan(x) = -atan(-x).
 * (2). For x <= 1/8, use
 *	(2.1) if x < 2^(-prec/2-2), atan(x) =  x  with inexact
 *	(2.2) Otherwise
 *	   	atan(x) = poly1(x)
 * (3). For x >= 8 then
 *	(3.1) if x >= 2^(prec+2),   atan(x) = atan(inf) - pio2lo
 *	(3.2) if x >= 2^(prec/3+2), atan(x) = atan(inf) - 1/x
 *	(3.3) if x >  65,           atan(x) = atan(inf) - poly2(1/x)
 *	(3.4) Otherwise,	    atan(x) = atan(inf) - poly1(1/x)
 *
 * (4). Now x is in (0.125, 8)
 *      Find y that match x to 4.5 bit after binary (easy).
 *	If iy is the high word of y, then
 *		single : j = (iy - 0x3e000000) >> 19
 *		(single is modified to (iy-0x3f000000)>>19)
 *		double : j = (iy - 0x3fc00000) >> 16
 *		quad   : j = (iy - 0x3ffc0000) >> 12
 *
 *	Let s = (x-y)/(1+x*y). Then
 *	atan(x) = atan(y) + poly1(s)
 *		= _TBL_r_atan_hi[j] + (_TBL_r_atan_lo[j] + poly2(s) )
 *
 *	Note. |s| <= 1.5384615385e-02 = 1/65. Maxium occurs at x = 1.03125
 *
 */

#include <math.h>
#include <math_private.h>

const float _TBL_r_atan_hi[] = {
  4.636476040e-0001, 4.883339405e-0001, 5.123894811e-0001, 5.358112454e-0001,
  5.585992932e-0001, 5.807563663e-0001, 6.022873521e-0001, 6.231993437e-0001,
  6.435011029e-0001, 6.632030010e-0001, 6.823165417e-0001, 7.008544207e-0001,
  7.188299894e-0001, 7.362574339e-0001, 7.531512976e-0001, 7.695264816e-0001,
  7.853981853e-0001, 8.156919479e-0001, 8.441540003e-0001, 8.709034324e-0001,
  8.960554004e-0001, 9.197195768e-0001, 9.420000315e-0001, 9.629943371e-0001,
  9.827937484e-0001, 1.001483083e+0000, 1.019141316e+0000, 1.035841227e+0000,
  1.051650167e+0000, 1.066630363e+0000, 1.080839038e+0000, 1.094328880e+0000,
  1.107148767e+0000, 1.130953789e+0000, 1.152572036e+0000, 1.172273874e+0000,
  1.190289974e+0000, 1.206817389e+0000, 1.222025275e+0000, 1.236059427e+0000,
  1.249045730e+0000, 1.261093378e+0000, 1.272297382e+0000, 1.282740831e+0000,
  1.292496681e+0000, 1.301628828e+0000, 1.310193896e+0000, 1.318242073e+0000,
  1.325817704e+0000, 1.339705706e+0000, 1.352127433e+0000, 1.363300085e+0000,
  1.373400807e+0000, 1.382574797e+0000, 1.390942812e+0000, 1.398605466e+0000,
  1.405647635e+0000, 1.412141085e+0000, 1.418146968e+0000, 1.423717976e+0000,
  1.428899288e+0000, 1.433730125e+0000, 1.438244820e+0000, 1.442473054e+0000,
  1.446441293e+0000,
};

const float _TBL_r_atan_lo[] = {
  5.012158688e-0009, 1.055042365e-0008,-2.075691974e-0008,-7.480973174e-0009,
  2.211159789e-0008,-1.268522887e-0008,-5.950149262e-0009,-1.374726910e-0008,
  5.868937336e-0009,-8.316245470e-0009, 1.320299514e-0008,-1.277747597e-0008,
  1.018833551e-0008,-4.909868068e-0009,-1.660708016e-0008,-1.222759671e-0009,
 -2.185569414e-0008,-2.462078896e-0008,-1.416911655e-0008, 2.470642002e-0008,
 -1.580020736e-0008, 2.851478520e-0008, 8.908211058e-0009,-6.400973085e-0009,
 -2.513142405e-0008, 5.292293181e-0008, 2.785247055e-0008, 2.643104224e-0008,
  4.603683834e-0008, 1.851388043e-0009,-3.735403453e-0008, 2.701113111e-0008,
 -4.872354964e-0008,-4.477816518e-0008,-3.857382325e-0008, 6.845639611e-0009,
 -2.453011483e-0008,-1.824929363e-0008, 4.798058129e-0008, 6.221672777e-0008,
  4.276110843e-0008, 4.185424007e-0009, 1.285398099e-0008, 4.836914869e-0008,
 -1.342359379e-0008, 5.960489879e-0009, 3.875391386e-0008,-2.204224536e-0008,
 -4.053271141e-0008,-4.604370218e-0008,-5.190222652e-0008, 1.529194549e-0008,
 -4.043566193e-0008, 2.481348993e-0008, 1.503647518e-0008, 4.638297924e-0008,
  1.392036975e-0008,-2.006252586e-0008, 3.051175312e-0008,-4.209960824e-0009,
 -1.598675681e-0008, 2.705746205e-0008,-2.514289044e-0008, 4.517691110e-0008,
  3.948537852e-0008,
};

static const float
	big	=   1.0e37F,
	one	=   1.0F,
	p1      =  -3.333185951111688247225368498733544672172e-0001F,
	p2      =   1.969352894213455405211341983203180636021e-0001F,
	q1      =  -3.332921964095646819563419704110132937456e-0001F,
	a1	=  -3.333323465223893614063523351509338934592e-0001F,
	a2	=   1.999425625935277805494082274808174062403e-0001F,
	a3	=  -1.417547090509737780085769846290301788559e-0001F,
	a4	=   1.016250813871991983097273733227432685084e-0001F,
	a5	=  -5.137023693688358515753093811791755221805e-0002F,
	pio2hi  =   1.570796371e+0000F,
	pio2lo  =  -4.371139000e-0008F;

float
__atanf(float xx) {
	float x, y, z, r, p, s;
	volatile double dummy;
	int ix, iy, sign, j;

	x = xx;
	ix = *(int *) &x;
	sign = ix & 0x80000000;
	ix ^= sign;

	/* for |x| < 1/8 */
	if (ix < 0x3e000000) {
		if (ix < 0x38800000) {	/* if |x| < 2**(-prec/2-2) */
			dummy = big + x;	/* get inexact flag if x!=0 */
			return x;
		}
		z = x * x;
		if (ix < 0x3c000000) {	/* if |x| < 2**(-prec/4-1) */
			x = x + (x * z) * p1;
			return x;
		}
		else {
			x = x + (x * z) * (p1 + z * p2);
			return x;
		}
	}

	/* for |x| >= 8.0 */
	if (ix >= 0x41000000) {
		*(int *) &x = ix;
		if (ix < 0x42820000) {	/* x <  65 */
			r = one / x;
			z = r * r;
			y = r * (one + z * (p1 + z * p2));	/* poly1 */
			y -= pio2lo;
		}
		else if (ix < 0x44800000) {	/* x <  2**(prec/3+2) */
			r = one / x;
			z = r * r;
			y = r * (one + z * q1);	/* poly2 */
			y -= pio2lo;
		}
		else if (ix < 0x4c800000) {	/* x <  2**(prec+2) */
			y = one / x - pio2lo;
		}
		else if (ix < 0x7f800000) {	/* x <  inf */
			y = -pio2lo;
		}
		else {		/* x is inf or NaN */
			if (ix > 0x7f800000) {
				x = x - x;
				return x;
			}
			y = -pio2lo;
		}

		if (sign == 0)
			x = pio2hi - y;
		else
			x = y - pio2hi;
		return x;
	}


	/* now x is between 1/8 and 8 */
	if (ix < 0x3f000000) {	/* between 1/8 and 1/2 */
		z = x * x;
		x = x + (x * z) * (a1 + z * (a2 + z * (a3 + z * (a4 +
			z * a5))));
		return x;
	}
	*(int *) &x = ix;
	iy = (ix + 0x00040000) & 0x7ff80000;
	*(int *) &y = iy;
	j = (iy - 0x3f000000) >> 19;

	if (ix == iy)
		p = x - y;	/* p=0.0 */
	else {
		if (sign == 0)
			s = (x - y) / (one + x * y);
		else
			s = (y - x) / (one + x * y);
		z = s * s;
		p = s * (one + z * q1);
	}
	if (sign == 0) {
		r = p + _TBL_r_atan_lo[j];
		x = r + _TBL_r_atan_hi[j];
	}
	else {
		r = p - _TBL_r_atan_lo[j];
		x = r - _TBL_r_atan_hi[j];
	}
	return x;
}

weak_alias (__atanf, atanf)
