/* Copyright (c) 2016-2018 ZAO "MCST". All rights reserved. */

/* Compute complex natural logarithm.
   Copyright (C) 1997-2015 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
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
#include <e2kintrin.h>

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ÚÔÁÚ·ÌÌ·  ˜˘˛ÈÛÏÒÂÙ  Ó·ÙıÚ·Ï¯Ó˘Í  ÏÔÁ·ÚÈÊÌ  ÎÔÌ- %
%  ÏÂÎÛÓÔÁÔ  ˛ÈÛÏ·.  ÓÂ‰ÔıÛÙÈÌ˘ÌÈ  ˙Ó·˛ÂÓÈÒÌÈ  ·ÚÁı-    %
%  ÌÂÓÙÔ˜  Ò˜ÏÒ‡ÙÛÒ  RE = 0  È  IM = 0.                  %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define KB5 1.99999455044662533610e-01
#define KB3 -3.33333333333073478475e-01

#define KA15 -6.35431043675711160064e-02
#define KA13 7.68566158858773828350e-02
#define KA11 -9.09083861491049053210e-02
#define KA9 1.11111107266816104432e-01
#define KA7 -1.42857142847146228750e-01
#define KA5 1.99999999999990244796e-01
#define KA3 -3.33333333333333331778e-01
#define LKA15 0xbfb0445c632520ddLL
#define LKA13 0x3fb3ace00bb6092cLL
#define LKA11 0xbfb745c5a1714aa0LL
#define LKA9  0x3fbc71c70beeee43LL
#define LKA7  0xbfc249249243a5acLL
#define LKA5  0x3fc999999999983aLL
#define LKA3  0xbfd5555555555555LL

#define KC9 2.226555769734402668e-01
#define KC7 2.857139299728113646e-01
#define KC5 4.000000001150663675e-01
#define KC3 6.666666666666550833e-01
#define LKC9 0x3fcc7ffa5aafbc63LL
#define LKC7 0x3fd24923144fabc5LL
#define LKC5 0x3fd9999999b93aadLL
#define LKC3 0x3fe55555555554edLL

#define K2 4.00006488775094945494e-01
#define K1 6.66666666629825839191e-01

__complex__ double
__clog (__complex__ double x)
{
    _type_double_bits absre, absim;
    DB re, im, y, yv2;
    LL signre, signim;

    _type_double_bits arg1, arg2, max, min, tmp;
    DB b, y0, yv4, yv8;
    LL ind, exp;
#if __iset__ >= 6
    _type_double_bits ly, ly0, lyv2, lyv4, lyv8, lres, lres1;
#endif /* __iset__ < 6 */

    __complex__ double res;

    re = __real__ x;
    im = __imag__ x;

    absre.value = re;
    signre = (absre.llong >> 62) & 2;
    absim.value = im;
    signim = (absim.llong >> 62) & 2;
    absre.value = dfabs (re);
    absim.value = dfabs (im);

    if (__glibc_unlikely (absre.llong == 0 && absim.llong == 0)) {
        /* Real and imaginary part are 0.0.  */
        __imag__ res = signre ? M_PI : 0.0;
        __imag__ res = __copysign (__imag__ res, im);
        /* Yes, the following line raises an exception.  */
        __real__ res = -1.0 / absre.value;
        return res;
    }
    if (__glibc_unlikely (absre.llong > 0x7ff0000000000000LL ||
                          absim.llong > 0x7ff0000000000000LL)) { /* |re| …Ã… |im| == Nan */
        __imag__ res = NaN;
        if (absre.llong == 0x7ff0000000000000LL || absim.llong == 0x7ff0000000000000LL)
          /* Real or imaginary part is infinite.  */
            __real__ res = Inf;
        else
            __real__ res = NaN;
        return res;
    }

    /* ◊Ÿﬁ…”Ã≈Œ…≈ im=atan2(im,re) */
    arg1.value = absre.value;
    arg2.value = absim.value;
    b = arg1.value;
    if (__glibc_unlikely (arg2.llong == 0x7ff0000000000000LL)) {
        /* |Y| == Inf */
        __real__ res = Inf;

        if (arg1.llong == 0x7ff0000000000000LL) {  /* |X| == Inf */
            /* atan2(+/-Inf,-Inf) returns +/-3pi/4 */
            /* atan2(+/-Inf,Inf) returns +/-pi/4 */
            y0 = (DB) ((signre + 1) * (1 - signim));
            __imag__ res = (PION2 * 0.5) * y0;
        }
        else { /* atan2(+/-Inf,x) returns +/-pi/2 for finite x */
            __imag__ res = PION2 * ((DB) (1 - signim));
        }
        return res;
    }
    if (__glibc_unlikely (arg1.llong == 0x7ff0000000000000LL || im == 0)) {
        /* |X| == Inf or |Y| == 0 */
        if (arg1.llong == 0x7ff0000000000000LL) {
            __real__ res = Inf;
        }
        else {
            __real__ res = __ieee754_log (arg1.value);
        }
        /* atan2(+/-y,-Inf) returns +/-pi for finite |y| > 0; */
        /* atan2(+/-0,x) returns +/-pi for x < 0 or x = -0 */
        /* atan2(+/-y,Inf) returns +/-0 for finite |y| > 0 */
        /* atan2(+/-0,x) returns +/-0 for x > 0 or x = +0 */
        arg1.value = ((DB) (signre >> 1)) * PI;
        arg1.llong |= signim << 62;
        __imag__ res = arg1.value;
        return res;
    }

    if (b < arg2.value) {
        y = b / arg2.value;
        min.value = b;
        max.value = arg2.value;
    }
    else {
        y = arg2.value / b;
        min.value = arg2.value;
        max.value = b;
    }
    arg1.value = y;
    if (arg1.twolong.int0 < 0x3fc00000) {    /* |arg|<0.125 */
        _type_double_bits max1, min1;
        max1.value = max.value;
        min1.value = min.value;
        if (max.twolong.int0 < 0x3fc00000) {
            max1.value *= DVAIN1022;
            min1.value *= DVAIN1022;
        }
        arg1.llong = __builtin_e2k_pandd (arg1.llong, 0xffffffff00000000LL);
        y0 = max1.value;
        tmp.llong = __builtin_e2k_pandd (max1.llong, 0xffffffff00000000LL);
#if __iset__ >= 6
        lres.llong = __builtin_e2k_fmsd (arg1.llong, tmp.llong, min1.llong);
        y0 = (lres.value + arg1.value * (y0 - tmp.value)) * (1.0 / y0);
#else /* __iset__ < 6 */
        y0 = (arg1.value * tmp.value - min1.value + arg1.value * (y0 - tmp.value)) * (1.0 / y0);
#endif /* __iset__ < 6 */
        yv2 = y * y;
        yv4 = yv2 * yv2;
        yv8 = yv4 * yv4;
#if __iset__ >= 6
        ly.value = y * yv2;
        ly0.value = y0;
        lyv2.value = yv2;
        lyv4.value = yv4;
        lyv8.value = yv8;
        lres.llong = __builtin_e2k_fmad (LKA5, lyv2.llong, LKA3);
        lres1.llong = __builtin_e2k_fmad (LKA9, lyv2.llong, LKA7);
        lres.llong = __builtin_e2k_fmad (lres1.llong, lyv4.llong, lres.llong);
        lres1.llong = __builtin_e2k_fmad (LKA15, lyv2.llong, LKA13);
        lres1.llong = __builtin_e2k_fmad (lres1.llong, lyv2.llong, LKA11);
        lres.llong = __builtin_e2k_fmad (lres1.llong, lyv8.llong, lres.llong);
        lres.llong = __builtin_e2k_fnmad (ly.llong, lres.llong, ly0.llong);
        if (y != 0) y0 = lres.value; /* for y = -0.0 only */
#else /* __iset__ < 6 */
        y0 -= KA5 * y * yv2 * (KA3 / KA5 + yv2) + KA7 * y * yv2 * yv4 + KA11 * y *
              (KA9 / KA11 + yv2) * yv8 + y * yv4 * yv8 * (KA13 + KA15 * yv2);
#endif /* __iset__ < 6 */
        if (arg1.twolong.int0 < 0x3f000000) {
            y0 -= arg1.value;
            arg1.value = 0.0;
        }
    }
    else {
        arg1.llong = __builtin_e2k_paddd (arg1.llong, 0x100000000000LL);
        arg1.llong = __builtin_e2k_pandd (arg1.llong, 0x7fffe00000000000LL);
        ind = (arg1.llong >> 45) - 0x1fe00;
#if __iset__ >= 6
        lres.value = y;
        lres.llong = __builtin_e2k_fmad (lres.llong, arg1.llong, 0x3ff0000000000000LL);
        y = (arg1.value - y) / lres.value;
#else /* __iset__ < 6 */
        y = (arg1.value - y) / (1.0 + y * arg1.value);
#endif /* __iset__ < 6 */
        yv2 = y * y;
#if __iset__ >= 6
        ly.value = y - constarctg_lo[ind];
        arg1.value = KB5 * y * yv2;
        lres.value = KB3 / KB5 + yv2;
        lres.llong = __builtin_e2k_fmad (arg1.llong, lres.llong, ly.llong);
        y0 = lres.value;
#else /* __iset__ < 6 */
        y0 = y - constarctg_lo[ind] + KB5 * y * yv2 * (KB3 / KB5 + yv2);
#endif /* __iset__ < 6 */
        arg1.llong = ((ULL) ((unsigned *) constarctg)[ind]) << 32;
    }
    if (arg2.value > b) {
        if (signre) {
            __imag__ res = (signim) ? (-PION2_lo + y0) - (PION2 + arg1.value) : (PION2 + arg1.value) - (-PION2_lo + y0);
        }
        else
            __imag__ res = (signim) ? (-PION2_lo - y0) - (PION2 - arg1.value) : (PION2 - arg1.value) - (-PION2_lo - y0);
    }
    else if (signre) {
        __imag__ res = (signim) ? (-PI_lo - y0) - (PI - arg1.value) : (PI - arg1.value) - (-PI_lo - y0);
    }
    else
        __imag__ res = (signim) ? y0 - arg1.value : arg1.value - y0;

   /* ◊Ÿﬁ…”Ã≈Œ…≈ max=Mmax^2+min^2*2^-max … exp=2*max */
    exp = -1022;
    if (max.value - min.value == max.value) {
        if (max.llong == 0x3ff0000000000000LL) { /* max == 1 */
            min.value *= min.value;
            re = 0.5;
        }
        else {
            min.value = 0;
            re = 1.0;
        }
        if (max.llong >= 0x0010000000000000LL)
            exp = 0;
        else
            max.value *= DVAIN1022;     /* ƒ≈Œœ“Õ¡Ã…⁄œ◊¡ŒŒŸ  max */
    }
    else {
        re = 0.5;
        exp = -2044;
        if (min.llong < 0x0010000000000000LL) {
            max.value *= DVAIN1022;     /* ƒ≈Œœ“Õ¡Ã…⁄œ◊¡ŒŒŸ  min */
            min.value *= DVAIN1022;
        }
        else {
            exp = (max.llong & 0x7ff0000000000000LL) - 0x3ff0000000000000LL;
            if (exp != 0xfff0000000000000LL) { /* exp != -1 */
                max.llong -= exp;
                min.llong -= exp;
                exp >>= 51;
            }
            else exp = 0; /* 0.5 > max < 1 */
        }
        b = max.value - min.value;
        arg1.llong &= ~0xffffffffLL;
        if (b < min.value) {    /* ¬Ã…⁄À…≈ max … min */
            arg1.llong = max.llong & ~0xfffffffLL; /* 25 … 28 “¡⁄“—ƒœ◊ */
            y = max.value - arg1.value;
            arg2.llong = min.llong & ~0xfffffffLL; /* 25 … 28 “¡⁄“—ƒœ◊ */
            y0 = min.value - arg2.value;
            min.value = y * (max.value + arg1.value) + y0 * (min.value + arg2.value);
            max.value = arg1.value * arg1.value + arg2.value * arg2.value;
        }
        else {
            arg1.llong = max.llong & ~0x7ffffffLL; /* 26 … 27 “¡⁄“—ƒœ◊ */
            min.value = (max.value - arg1.value) * (max.value + arg1.value) + min.value * min.value;
            max.value = arg1.value * arg1.value;
        }
    }
    /* ◊Ÿﬁ…”Ã≈Œ…≈ re=0.5*(exp*log(2)+log(max)) */
    y = max.value - 1.0 + min.value;
    max.value += min.value;
    arg2.value = dfabs (y);
    if (arg2.twolong.int0 <= 0x3faeb851) {      /* |x-1|<=0.06 */
        y0 = y / (max.value + 1.0);
        yv2 = y0 * y0;
#if __iset__ >= 6
        lyv2.value = yv2;
        lyv4.value = yv2 * yv2;
        lyv8.value = y0 * yv2;
        lres.value = exp * KLN2_lo - y0 * y;
        lres1.llong = __builtin_e2k_fmad (LKC5, lyv2.llong, LKC3);
        lres.llong = __builtin_e2k_fmad (lres1.llong, lyv8.llong, lres.llong);
        lyv8.value *= lyv4.value;
        lres1.llong = __builtin_e2k_fmad (LKC9, lyv2.llong, LKC7);
        lres.llong = __builtin_e2k_fmad (lres1.llong, lyv8.llong, lres.llong);
        __real__ res = (lres.value + y + exp * KLN2_hi) * re;
#else /* __iset__ < 6 */
        __real__ res = (exp * KLN2_lo - y0 * y + KC5 * y0 * yv2 * (KC3 / KC5 + yv2) +
            KC9 * y0 * yv2 * (yv2 * yv2 * (KC7 / KC9 + yv2)) + y + exp * KLN2_hi) * re;
#endif /* __iset__ < 6 */
        return res;
    }
    exp = exp - 1023 + exponentabs (max);
    arg2.llong = (ind = max.llong & 0xfe00000000000LL) | 0x3ff0000000000000LL;
    max.llong &= 0xfffffffffffffLL;
    max.llong |= 0x3ff0000000000000LL;
    y0 = (max.value - arg2.value) / (max.value + arg2.value);
    yv2 = y0 * y0;
    arg2.llong = ((LL) ((unsigned *) constln)[ind >>= 45]) << 32;
#if __iset__ >= 6
    lres.value = y0;
    ly.value = exp * KLN2_lo + constln_lo[ind];
    ly.llong = __builtin_e2k_fmad (/* 2.0 */ 0x4000000000000000LL, lres.llong, ly.llong);
    lres1.value = K2 * y0 * yv2;
    lres.value = yv2 + K1 / K2;
    lres.llong = __builtin_e2k_fmad (lres1.llong, lres.llong, ly.llong);
   __real__ res = (lres.value + (arg2.value + exp * KLN2_hi)) * re;
#else /* __iset__ < 6 */
    __real__ res = (y0 + y0 + exp * KLN2_lo + constln_lo[ind] + K2 * y0 * yv2 *
                   (yv2 + K1 / K2) + (arg2.value + exp * KLN2_hi)) * re;
#endif /* __iset__ < 6 */
    return res;
}

weak_alias (__clog, clog)
#ifdef NO_LONG_DOUBLE
strong_alias (__clog, __clogl)
weak_alias (__clog, clogl)
#endif
