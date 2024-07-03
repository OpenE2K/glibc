/*
 * Copyright 12/22/99 Sun Microsystems, Inc.  All Rights Reserved.
 */

#include <math.h>
#include <math_private.h>

static const double
	one	=  1.0,
	S0	=  1.66666552424430847168e-01,	/* 2^ -3 *  1.5555460000000 */
	S1	= -8.33219196647405624390e-03,	/* 2^ -7 * -1.11077E0000000 */
	S2	=  1.95187909412197768688e-04,	/* 2^-13 *  1.9956B60000000 */
	half	=  0.5,
	C0	=  4.16666455566883087158e-02,	/* 2^ -5 *  1.55554A0000000 */
	C1	= -1.38873036485165357590e-03,	/* 2^-10 * -1.6C0C1E0000000 */
	C2	=  2.44309903791872784495e-05,	/* 2^-16 *  1.99E24E0000000 */
	invpio2	= 0.636619772367581343075535,	/* 2^ -1  * 1.45F306DC9C883 */
//	c3two51	= 6755399441055744.0,		/* 2^ 52  * 1.8000000000000 */
	pio2_1  = 1.570796326734125614166,	/* 2^  0  * 1.921FB54400000 */
	pio2_t	= 6.077100506506192601475e-11;	/* 2^-34  * 1.0B4611A626331 */

void
__sincosf(float x, float *s, float *c)
{
	double	y, z, w, t0, t1, t2;
	float	fy[2];
	int	n, ix, hx;

	hx = *((int*)&x);
	ix = hx & 0x7fffffff;

	y = (double) x;

	if (ix <= 0x4016cbe4) {		/* |x| < 3*pi/4 */
		if (ix <= 0x3f490fdb) {		/* |x| < pi/4 */
			if (ix == 0) {
				*s = x;
				*c = one;
				return;
			}
			z = y * y;
			t0 = y * S0;
			t1 = y * S1;
			t2 = y * S2;
			w = z * z;
			*s = (float) ((y - z * t0) - w * (t1 + z * t2));
			*c = (float) ((one - z * half) + w *
				(C0 + z * C1 + w * C2));
		}
		else if (hx > 0) {
			y = (y - pio2_1) - pio2_t;
			z = y * y;
			t0 = y * S0;
			t1 = y * S1;
			t2 = y * S2;
			w = z * z;
			*s = (float) ((one - z * half) + w *
				(C0 + z * C1 + w * C2));
			*c = (float) (w * (t1 + z * t2) - (y - z * t0));
		}
		else {
			y = (y + pio2_1) + pio2_t;
			z = y * y;
			t0 = y * S0;
			t1 = y * S1;
			t2 = y * S2;
			w = z * z;
			*s = (float) ((z * half - one) - w *
				(C0 + z * C1 + w * C2));
			*c = (float) ((y - z * t0) - w * (t1 + z * t2));
		}
		return;
	}
	else if (ix <= 0x49c90fdb) {	/* |x| < 2^19*pi */
//		w = y * invpio2 + c3two51;
//		n = *(1+(int*)&w);
//		w -= c3two51;
		w = y * invpio2 + ((hx < 0) ? -half : half);
		n = (int) w;
		w = (double) n;
		y = (y - w * pio2_1) - w * pio2_t;
	}
	else {
		if (ix >= 0x7f800000) {
			*s = *c = x - x;/* {sin,cos}(Inf or NaN) is NaN */
			return;
		}
	        n = __ieee754_rem_pio2f(x,fy);
		y = fy[0] + (double) fy[1];
	}

	z = y * y;
	t0 = y * S0;
	t1 = y * S1;
	t2 = y * S2;
	w = z * z;
	switch (n & 3) {
	case 0:
		*s = (float) ((y - z * t0) - w * (t1 + z * t2));
		*c = (float) ((one - z * half) + w * (C0 + z * C1 + w * C2));
		break;
	case 1:
		*s = (float) ((one - z * half) + w * (C0 + z * C1 + w * C2));
		*c = (float) ((z * t0 - y) + w * (t1 + z * t2));
		break;
	case 2:
		*s = (float) ((z * t0 - y) + w * (t1 + z * t2));
		*c = (float) ((z * half - one) - w * (C0 + z * C1 + w * C2));
		break;
	default:
		*s = (float) ((z * half - one) - w * (C0 + z * C1 + w * C2));
		*c = (float) ((y - z * t0) - w * (t1 + z * t2));
	}
}

weak_alias (__sincosf, sincosf)
