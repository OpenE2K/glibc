/*
 * Copyright 12/10/02 Sun Microsystems, Inc.  All Rights Reserved.
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

static const int
        is = (int) 0x80000000,
        im = 0x007fffff,
        ii = 0x7f800000,
        iu = 0x00800000;

static const float zero = 0.0;

float
__fmodf(float x, float y) {
        float w;
        int hx, ix, iy, iz, k, ny, nd;

        hx = *(int *) &x;
        ix = hx & 0x7fffffff;
        iy = *(int *) &y & 0x7fffffff;

        /* purge off exception values */
        if (ix >= ii || iy > ii || iy == 0) {
                if ((iy == 0 || ix == ii) && ix <= ii && iy <= ii) /* fmod(+-Inf,y) or fmod(x,0) */
                        return __kernel_standard_f(x, y, 127);
                w = x * y;
                w = w / w;
        } else if (ix <= iy) {
                if (ix < iy)
                        w = x;  /* return x if |x|<|y| */
                else
                        w = zero * x;   /* return sign(x)*0.0  */
        } else {
                /*
                 * scale x,y to "normal" with
                 *      ny = exponent of y
                 *      nd = exponent of x minus exponent of y
                 */
                ny = iy >> 23;
                k = ix >> 23;

                /* special case for subnormal y or x */
                if (ny == 0) {
                        ny = 1;
                        while (iy < iu) {
                                ny -= 1;
                                iy += iy;
                        }
                        nd = k - ny;
                        if (k == 0) {
                                nd += 1;
                                while (ix < iu) {
                                        nd -= 1;
                                        ix += ix;
                                }
                        } else
                                ix = iu | (ix & im);
                } else {
                        nd = k - ny;
                        ix = iu | (ix & im);
                        iy = iu | (iy & im);
                }

                /* fix point fmod for normalized ix and iy */
                /*
                 * while (nd--) {
                 *      iz = ix - iy;
                 * if (iz < 0)
                 *      ix = ix + ix;
                 * else if (iz == 0) {
                 *      *(int *) &w = is & hx;
                 *      return w;
                 * }
                 * else
                 *      ix = iz + iz;
                 * }
                 */
                /* unroll the above loop 4 times to gain performance */
                k = nd >> 2;
                nd -= k << 2;
                while (k--) {
                        iz = ix - iy;
                        if (iz >= 0)
                                ix = iz + iz;
                        else
                                ix += ix;
                        iz = ix - iy;
                        if (iz >= 0)
                                ix = iz + iz;
                        else
                                ix += ix;
                        iz = ix - iy;
                        if (iz >= 0)
                                ix = iz + iz;
                        else
                                ix += ix;
                        iz = ix - iy;
                        if (iz >= 0)
                                ix = iz + iz;
                        else
                                ix += ix;
                        if (iz == 0) {
                                *(int *) &w = is & hx;
                                return (w);
                        }
                }
                while (nd--) {
                        iz = ix - iy;
                        if (iz >= 0)
                                ix = iz + iz;
                        else
                                ix += ix;
                }
                /* end of unrolling */

                iz = ix - iy;
                if (iz >= 0)
                        ix = iz;

                /* convert back to floating value and restore the sign */
                if (ix == 0) {
                        *(int *) &w = is & hx;
                        return (w);
                }
                while (ix < iu) {
                        ix += ix;
                        ny -= 1;
                }
                while (ix > (iu + iu)) {
                        ny += 1;
                        ix >>= 1;
                }
                if (ny > 0)
                        *(int *) &w = (is & hx) | (ix & im) | (ny << 23);
                else {          /* subnormal output */
                        k = -ny + 1;
                        ix >>= k;
                        *(int *) &w = (is & hx) | ix;
                }
        }
        return (w);
}

strong_alias (__fmodf, __fmodf_finite)
weak_alias (__fmodf, fmodf)
weak_alias (__fmodf, __ieee754_fmodf)
