/*
 * Copyright 02/25/97 Sun Microsystems, Inc.  All Rights Reserved.
 */

#include <math.h>
#include <math_private.h>
#include <math-svid-compat.h>

float
__hypotf(float x, float y) {
	double dx, dy;
	float w;
	int ix, iy;

	ix = (*(int *) &x) & 0x7fffffff;
	iy = (*(int *) &y) & 0x7fffffff;
	if (ix >= 0x7f800000) {
		if (ix == 0x7f800000)
			*(int *) &w = x == y ? iy : ix;	/* w = |x| = inf */
		else if (iy == 0x7f800000)
			*(int *) &w = x == y ? ix : iy;	/* w = |y| = inf */
		else
			w = x + y;
	}
	else if (iy >= 0x7f800000) {
		if (iy == 0x7f800000)
			*(int *) &w = x == y ? ix : iy;	/* w = |y| = inf */
		else
			w = x + y;
	}
	else if (ix == 0)
		*(int *) &w = iy;	/* w = |y|  */
	else if (iy == 0)
		*(int *) &w = ix;	/* w = |x|  */
	else {
		dx = (double) x;
		dy = (double) y;
		w = (float) __builtin_sqrt(dx * dx + dy * dy);
	        if (__builtin_expect(w == INFINITY, 0))
	                return __kernel_standard_f(x, y, 104); /* hypot overflow */
	}
	return w;
}

strong_alias (__hypotf, __hypotf_finite)
weak_alias (__hypotf, hypotf)
weak_alias (__hypotf, __ieee754_hypotf)
