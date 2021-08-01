/*
 * Copyright 08/21/03 Sun Microsystems, Inc.  All Rights Reserved.
 */
/* acos(x)
 * Method :                  
 *	acos(x)  = pi/2 - asin(x)
 *	acos(-x) = pi/2 + asin(x)
 * For |x|<=0.5
 *	acos(x) = pi/2 - (x + x*x^2*R(x^2))	(see e_asin.c)
 * For x>0.5
 * 	acos(x) = pi/2 - (pi/2 - 2asin(sqrt((1-x)/2)))
 *		= 2asin(sqrt((1-x)/2))  
 *		= 2s + 2s*z*R(z) 	...z=(1-x)/2, s=sqrt(z)
 *		= 2f + (2c + 2s*z*R(z))
 *     where f=hi part of s, and c = (z-f*f)/(s+f) is the correction term
 *     for f so that f+c ~ sqrt(z).
 * For x<-0.5
 *	acos(x) = pi - 2asin(sqrt((1-|x|)/2))
 *		= pi - 0.5*(s+s*z*R(z)), where z=(1-|x|)/2,s=sqrt(z)
 *
 * Special cases:
 *	if x is NaN, return x itself;
 *	if |x|>1, return NaN with invalid signal.
 *
 * Function needed: sqrt
 */
/* Copyright (C) 2011-2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Ulrich Drepper <drepper@gmail.com>, 2011.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#include <math.h>
#include <math_private.h>
#include <math-svid-compat.h>

static const double xxx[] = {
/* one */	 1.00000000000000000000e+00,	/* 3FF00000, 00000000 */
/* pi */	 3.14159265358979311600e+00,	/* 400921FB, 54442D18 */
/* pio2_hi */	 1.57079632679489655800e+00,	/* 3FF921FB, 54442D18 */
/* pio2_lo */	 6.12323399573676603587e-17,	/* 3C91A626, 33145C07 */
/* pS0 */	 1.66666666666666657415e-01,	/* 3FC55555, 55555555 */
/* pS1 */	-3.25565818622400915405e-01,	/* BFD4D612, 03EB6F7D */
/* pS2 */	 2.01212532134862925881e-01,	/* 3FC9C155, 0E884455 */
/* pS3 */	-4.00555345006794114027e-02,	/* BFA48228, B5688F3B */
/* pS4 */	 7.91534994289814532176e-04,	/* 3F49EFE0, 7501B288 */
/* pS5 */	 3.47933107596021167570e-05,	/* 3F023DE1, 0DFDF709 */
/* qS1 */	-2.40339491173441421878e+00,	/* C0033A27, 1C8A2D4B */
/* qS2 */	 2.02094576023350569471e+00,	/* 40002AE5, 9C598AC8 */
/* qS3 */	-6.88283971605453293030e-01,	/* BFE6066C, 1B8D0159 */
/* qS4 */	 7.70381505559019352791e-02	/* 3FB3B8C5, B12E9282 */
};
#define	one	xxx[0]
#define	pi	xxx[1]
#define	pio2_hi	xxx[2]
#define	pio2_lo	xxx[3]
#define	pS0	xxx[4]
#define	pS1	xxx[5]
#define	pS2	xxx[6]
#define	pS3	xxx[7]
#define	pS4	xxx[8]
#define	pS5	xxx[9]
#define	qS1	xxx[10]
#define	qS2	xxx[11]
#define	qS3	xxx[12]
#define	qS4	xxx[13]

double
__acos(double x){
	double z, p, q, r, w, s, c, df;
	int hx, ix;

	hx = ((int *) &x)[HIGH_HALF];
	ix = hx & 0x7fffffff;
	if (ix >= 0x3ff00000) {	/* |x| >= 1 */
		if (((ix - 0x3ff00000) | ((int *) &x)[LOW_HALF]) == 0) {
			/* |x| == 1 */
			if (hx > 0)	/* acos(1) = 0 */
				return 0.0;
			else		/* acos(-1) = pi */
				return pi + 2.0 * pio2_lo;
		}
		else if (__isnan(x))
			return (x - x) / (x - x);	/* acos(|x|>1) is NaN */
		else
			return __kernel_standard(x, x, 1);
	}
	if (ix < 0x3fe00000) {	/* |x| < 0.5 */
		if (ix <= 0x3c600000)
			return pio2_hi + pio2_lo;	/* if |x| < 2**-57 */
		z = x * x;
		p = z * (pS0 + z * (pS1 + z * (pS2 + z * (pS3 +
			z * (pS4 + z * pS5)))));
		q = one + z * (qS1 + z * (qS2 + z * (qS3 + z * qS4)));
		r = p / q;
		return pio2_hi - (x - (pio2_lo - x * r));
	}
	else if (hx < 0) {	/* x < -0.5 */
		z = (one + x) * 0.5;
		p = z * (pS0 + z * (pS1 + z * (pS2 + z * (pS3 +
			z * (pS4 + z * pS5)))));
		q = one + z * (qS1 + z * (qS2 + z * (qS3 + z * qS4)));
		s = __builtin_sqrt(z);
		r = p / q;
		w = r * s - pio2_lo;
		return pi - 2.0 * (s + w);
	}
	else {			/* x > 0.5 */
		z = (one - x) * 0.5;
		s = __builtin_sqrt(z);
		df = s;
		((int *) &df)[LOW_HALF] = 0;
		c = (z - df * df) / (s + df);
		p = z * (pS0 + z * (pS1 + z * (pS2 + z * (pS3 +
			z * (pS4 + z * pS5)))));
		q = one + z * (qS1 + z * (qS2 + z * (qS3 + z * qS4)));
		r = p / q;
		w = r * s + c;
		return 2.0 * (df + w);
	}
}

strong_alias (__acos, __acos_finite)
weak_alias (__acos, acos)
weak_alias (__acos, __ieee754_acos)
