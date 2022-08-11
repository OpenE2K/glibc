/*
 * Copyright 07/16/02 Sun Microsystems, Inc.  All Rights Reserved.
 */

/*
 * This version of hypot(x,y) returns sqrt(x*x+y*y), scaling to avoid
 * premature over/underflow.  Infinite and NaN arguments are handled
 * correctly, but the inexact exception is not; the code can raise
 * spurious inexact exceptions even when the result is obviously exact
 * (e.g., one argument is zero), and it can fail to raise a deserved
 * inexact exception when neither argument is zero and one is much
 * larger than the other.
 *
 * This code uses the fact that hypot(x,y) ~ x to double precision
 * when |x/y| > 2^32.
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

static const double
	twom768	= 6.441148769597133308e-232,
	two768 = 1.552518092300708935e+231;

double
__hypot(double x, double y)
{
	int		hx, hy;

	hx = *(int*)&x & ~0x80000000;
	hy = *(int*)&y & ~0x80000000;

	if (hy > hx)
	{
		if (hy < 0x5fe00000)	/* y * y won't overflow */
		{
			if (hx >= 0x20000000) 	/* x * x won't underflow */
				return __builtin_sqrt(x * x + y * y);

			if (hy - hx > 0x02000000)	/* |y/x| > 2^32, x could be zero */
				return fabs(y);

			/* now 2^-1074 <= |y| < 2^-479 and similarly for x
			   (or x could be zero) */
			x *= two768;
			y *= two768;
			return twom768 * __builtin_sqrt(x * x + y * y);
		}

		if (hy >= 0x7ff00000)	/* y is inf or NaN */
			goto infnan;

		if (hy - hx > 0x02000000)	/* |y/x| > 2^32, x could be zero */
			return fabs(y);

		/* now 2^479 <= |y| < 2^1024 and similarly for x */
		x *= twom768;
		y *= twom768;
		{
		    double z = two768 * __builtin_sqrt(x * x + y * y);
		    if(__builtin_expect(!__finite(z), 0))
	                return __kernel_standard(x, y, 4); /* hypot overflow */
		    return z;
		}
	}
	else
	{
		if (hx < 0x5fe00000)	/* x * x won't overflow */
		{
			if (hy >= 0x20000000) 	/* y * y won't underflow */
				return __builtin_sqrt(x * x + y * y);

			if (hx - hy > 0x02000000)	/* |x/y| > 2^32, y could be zero */
				return fabs(x);

			/* now either x is zero or 2^-1074 <= |x| < 2^-479
			   and similarly for y */
			x *= two768;
			y *= two768;
			return twom768 * __builtin_sqrt(x * x + y * y);
		}

		if (hx >= 0x7ff00000)	/* x is inf or NaN */
			goto infnan;

		if (hx - hy > 0x02000000)	/* |x/y| > 2^32, y could be zero */
			return fabs(x);

		/* now 2^479 <= |x| < 2^1024 and similarly for y */
		x *= twom768;
		y *= twom768;
		{
		    double z = two768 * __builtin_sqrt(x * x + y * y);
		    if(__builtin_expect(z == INFINITY, 0))
	                return __kernel_standard(x, y, 4); /* hypot overflow */
		    return z;
		}
	}

infnan:
	/* At least one of x or y is inf or NaN. */
       if (isinf(x) || isinf(y)) /* x or y is inf */
           return INFINITY;
	return x + y; /* x or y is nan */
}

strong_alias (__hypot, __hypot_finite)
weak_alias (__hypot, hypot)
weak_alias (__hypot, __ieee754_hypot)
