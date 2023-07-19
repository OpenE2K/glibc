/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/* Complex square root of double value.
   Copyright (C) 1997-2015 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Based on an algorithm by Stephen L. Moshier <moshier@world.std.com>.
   Contributed by Ulrich Drepper <drepper@cygnus.com>, 1997.

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

#include <complex.h>
#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ÚÔÁÚ·ÌÌ·  ˜˘˛ÈÛÏÒÂÙ  ÎÔÚÂÓ¯  Î˜·‰Ú·ÙÓ˘Í  È˙       %
%  ÎÔÌÏÂÎÛÓÔÁÔ  ˛ÈÛÏ·.                                  %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

__complex__ double
__csqrt (__complex__ double x)
{
    _type_double_bits max, min, absre, absim;
    DB re, im, hyp, mul, scale;
    LL sign;
    __complex__ double res;

    re = __real__ x;
    im = __imag__ x;

    max.value = im;
    sign = max.llong & 0x8000000000000000LL;
    absre.value = dfabs (re);
    absim.value = dfabs (im);

    if (__glibc_unlikely (absre.llong >= 0x7ff0000000000000LL ||
                          absim.llong >= 0x7ff0000000000000LL)) { /* |re| …Ã… |im| == Inf or Nan */
        if (absim.llong == 0x7ff0000000000000LL) {
            __real__ res = Inf;
            __imag__ res = im;
        }
        else if (absre.llong == 0x7ff0000000000000LL) {
            if (re < 0.0) {
                __real__ res = absim.llong > 0x7ff0000000000000LL ? NaN : 0;
                max.value = Inf;
                max.llong += sign;
                __imag__ res = max.value;
            }
            else {
                __real__ res = re;
                max.llong = sign;
                __imag__ res = absim.llong > 0x7ff0000000000000LL ? NaN : max.value;
            }
        }
        else {
            __real__ res = __imag__ res = NaN;
        }
        return res;
    }
    /* hyp=0.5*hypot(re,im) */
    if (absim.value < absre.value) {
        min.value = absim.value;
        max.value = absre.value;
    }
    else {
        min.value = absre.value;
        max.value = absim.value;
    }
    if (__glibc_unlikely (min.llong == 0)) { /* re = 0 …Ã… im = 0 */
        if (max.llong == 0) { /* re = 0 … im = 0 */
            __real__ res = 0;
            __imag__ res = im;
            return res;
        }
        if (absim.llong == 0) { /* re != 0, im = 0 */
            min.value = dsqrt (absre.value);
            if (re < 0) {
                __real__ res = 0;
                min.llong += sign;
            }
            else {
                __real__ res = min.value;
                min.llong = sign;
            }
            __imag__ res = min.value;
            return res;
        }
        else { /* re = 0, im != 0 */
            min.value = (max.llong < 0x0020000000000000LL) ?
                0.5 * dsqrt (2 * absim.value) : dsqrt (0.5 * absim.value);
            __real__ res = min.value;
            min.llong += sign;
            __imag__ res = min.value;
            return res;
        }
    }
    scale = 1;
    mul = 0.5;
    if (max.llong >= 0x7fd0000000000000LL) {
        mul = 0.125;
        scale = 2;
    }
    else if (max.llong < 0x0020000000000000LL) {
        mul = (DB) (1LL << 53);
        scale = 1.0 / (DB) (1 << 27);
    }
    hyp = max.value - min.value;
    if (hyp == max.value)
        hyp = mul * max.value;
    else {

        hyp = (DB) (mul * __inline_sqrtl (re * (LD) re + im * (LD) im));
    }
    /* ÀœŒ≈√ ◊Ÿﬁ…”Ã≈Œ…— 0.5*hypot(re,im) */

    min.value = dsqrt (mul * absre.value + hyp);

    if (absim.llong < 0x0020000000000000LL)
        max.value = mul * scale * (absim.value / min.value);
    else
        max.value = mul * absim.value / min.value * scale;
    min.value *= scale;
    if (re > 0) {
        __real__ res = min.value;
        max.llong += sign;
        __imag__ res = max.value;
        return res;
    }
    __real__ res = max.value;
    min.llong += sign;
    __imag__ res = min.value;
    return res;
}

weak_alias (__csqrt, csqrt)
#ifdef NO_LONG_DOUBLE
strong_alias (__csqrt, __csqrtl)
weak_alias (__csqrt, csqrtl)
#endif
