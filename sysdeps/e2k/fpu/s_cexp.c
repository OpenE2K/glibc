/* Copyright (c) 2016-2018 ZAO "MCST". All rights reserved. */

/* Return value of complex exponential function for double complex value.
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
%     ÚÔÁÚ·ÌÌ·  ˜˘˛ÈÛÏÒÂÙ  ˙Ó·˛ÂÓÈÂ  ÊıÓÎ„ÈÈ            %
%  F(Z) = ÂËÚ (Z)  ÎÔÌÏÂÎÛÓÔÁÔ  ·ÚÁıÌÂÓÙ·.              %
%  ÓÂ‰ÔıÛÙÈÌ˘Ì     ˙Ó·˛ÂÓÈÂÌ     ·ÚÁıÌÂÓÙ·   Ò˜ÏÒÂÙÛÒ   %
%  RE > 709.7827128933839731 ÈÏÈ                         %
%  RE < -745.1332191019412221 .                          %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define KA13 1.5894736651849095247744e-10
#define KA11 -2.5050716974102745027602e-08
#define KA9  2.7557313376400129128204e-06
#define KA7  -1.9841269828650300013059e-04
#define KA5  8.3333333333203624567193e-03
#define KA14 -1.1360687302437253587387e-11
#define KA12 2.0875740157060361726804e-09
#define KA10 -2.7557314436057753118620e-07
#define KA8  2.4801587289509923185682e-05
#define KA6  -1.3888888888873247734608e-03
#define KA4  4.1666666666666577342883e-02
#define LKA14 0xbda8fb7f9656c8efLL
#define LKA12 0x3e21ee9fb1e54f8dLL
#define LKA10 0xbe927e4f81905bf6LL
#define LKA8  0x3efa01a019cb3bd5LL
#define LKA6  0xbf56c16c16c14feaLL
#define LKA4  0x3fa5555555555548LL
#define LKA13 0x3de5d87699eb6a31LL
#define LKA11 0xbe5ae5e37e850d20LL
#define LKA9  0x3ec71de3530aa14dLL
#define LKA7  0xbf2a01a019ba956dLL
#define LKA5  0x3f8111111110f3dcLL

#define TWOONPI 0.636619772367581343075535 /* 0x3fe45f306dc9c883 */
#define RS    6.75539944105574400e+15   /* 3*2^51 = 0x4338000000000000 */

#define KA3  -1.6666666666666616665801e-01L
#define KA1   9.9999999999999999685282e-01L
#define KA2  -4.9999999999999999852420e-01L
#define KA0   1.0000000000000000000000e+00
#define PIO2_1 (-3.1415926535897931159980 * 0.5) /* 53bits */
#define LPIO2_1 0xbff921fb54442d18LL
#define PIO2_1T (1.2246467991473531772015e-16L * 0.5)

#define KB5 8.33333478792298551112e-03
#define KB4 4.16666768487944911020e-02
#define KB3 1.66666666666665333693e-01
#define KB2 4.99999999999990669185e-01
#define LKB5 0x3f811111430bc5c6LL
#define LKB4 0x3fa55555accc11b8LL
#define LKB3 0x3fc5555555555525LL
#define LKB2 0x3fdfffffffffff58LL

__complex__ double
__cexp (__complex__ double x)
{
    _type_double_bits arg, absre, absim;
    DB re, im, y, yv2, hi;
    LL signre;
    int k, ind;

    type_union_64f darg1, si, co;
    DB t0, y0, tmp_si, xv2, xv4;
    LD ldx, ldxv2;
    LL k0;
    int k1;
#if __iset__ >= 6
    _type_double_bits ly, lyv2, lyv4, lyv8, lres, lres1, lres2;
#else /* __iset__ < 6  */
    DB xv8;
#endif /* __iset__ < 6 */

    __complex__ double res;

    re = __real__ x;
    im = __imag__ x;

    absre.value = dfabs (re);
    absim.value = dfabs (im);

    if (__glibc_unlikely (absre.llong >= 0x7ff0000000000000LL)) { /* |re| == Inf or Nan */
        if (absre.llong == 0x7ff0000000000000LL) { /* |re| = Inf */
            /* Real part is infinite.  */
            arg.value = re;
            signre = arg.llong & 0x8000000000000000LL;

            if (absim.llong < 0x7ff0000000000000LL) {
                /* Imaginary part is finite.  */
                double value = (signre) ? 0.0 : Inf;

                if (absim.llong == 0) {
                    /* Imaginary part is 0.0.  */
                    __real__ res = value;
                    __imag__ res = im;
                }
                else {
                    double sinix, cosix;

                    __sincos (im, &sinix, &cosix);
                    __real__ res = __copysign (value, cosix);
                    __imag__ res = __copysign (value, sinix);
                }
            }
            else if (signre == 0) {
                __real__ res = Inf;
                __imag__ res = NaN;
            }
            else {
                __real__ res = 0.0;
                __imag__ res = __copysign (0.0, im);
            }
        }
        else {
            /* If the real part is NaN the result is NaN + iNaN unless the
               imaginary part is zero.  */
            __real__ res = NaN;
            __imag__ res = (absim.llong == 0) ? im : NaN;
        }
        return res;
    }
    if (__glibc_unlikely (absim.llong >= 0x7ff0000000000000LL)) { /* |im| == Inf or Nan */
        /* If the imaginary part is +-inf or NaN and the real part
           is not +-inf the result is NaN + iNaN.  */
        __real__ res = __imag__ res = NaN;
        return res;
    }

    /* ◊Ÿﬁ…”Ã≈Œ…≈ si=sin(im) … co=cos(im) */
    darg1.db = im * TWOONPI;
//FIXME: –“… …”–œÃÿ⁄œ◊¡Œ…… fdtoifd ¬≈⁄ –≈“≈ƒ≈ÃÀ… - ⁄Œ¡À -0 Õ≈Œ—≈‘”— Œ¡ +0
#if __iset__ <= 2
    darg1.db += RS;
    t0 = darg1.db - RS;
    k0 = darg1.ll;
#else /*  __iset__ <= 2 */
#pragma asm_inline
    __asm ("fdtoifd 0x0,%1,%0" : "=r" (t0): "r" (darg1.db)); /* rc=0 - to nearest */
    k0 = (LL) t0;
#endif /*  __iset__ <= 2 */
#if __iset__ >= 6
    lres.value = t0;
    lres1.value = im;
    lres.llong = __builtin_e2k_fmad (LPIO2_1, lres.llong, lres1.llong);
    ldx = (LD) lres.value - t0 * PIO2_1T;
#else /* __iset__ < 6 */
    ldx = t0 * (LD) PIO2_1 + (LD) im - t0 * PIO2_1T;
#endif /* __iset__ < 6 */
    if (unlikely(absim.llong > 0x40d921fb00000000LL)) {      /* 2^13*pi */
        y = __rem_pio2 (im, &y0, &k1);
        ldx = (LD) y + (LD) y0;
        k0 = k1;
    }
    else {
        y = (DB) ldx;
    }
    ldxv2 = ldx * ldx;
    xv2 = y * y;
    xv4 = xv2 * xv2;
#if __iset__ >= 6
    lyv2.value = xv2;
    lyv4.value = xv4;
    lyv8.value = xv4 * xv4;
    lres2.llong = __builtin_e2k_fmad (LKA6, lyv2.llong, LKA4);
    lres2.value *= xv4;
    lres.llong = __builtin_e2k_fmad (LKA10, lyv2.llong, LKA8);
    lres1.llong = __builtin_e2k_fmad (LKA14, lyv2.llong, LKA12);
    lres1.llong = __builtin_e2k_fmad (lres1.llong, lyv4.llong, lres.llong);
    lres.llong = __builtin_e2k_fmad (lres1.llong, lyv8.llong, lres2.llong);
    co.db = (DB) ((KA0 + ldxv2 * KA2) + lres.value);

    lyv8.value = y * xv4;
    lres2.value = KA5 * lyv8.value;
    lres.llong = __builtin_e2k_fmad (LKA9, lyv2.llong, LKA7);
    lres1.llong = __builtin_e2k_fmad (LKA13, lyv2.llong, LKA11);
    lres1.llong = __builtin_e2k_fmad (lres1.llong, lyv4.llong, lres.llong);
    lres.value = xv2 * lyv8.value;
    lres.llong = __builtin_e2k_fmad (lres1.llong, lres.llong, lres2.llong);
    tmp_si = (DB) (KA3 * ldx * (KA1 / KA3 + ldxv2) + lres.value);
#else /* __iset__ < 6 */
    xv8 = xv4 * xv4;
    co.db = (DB) ((KA0 + ldxv2 * KA2) + (xv4 * (KA4 + KA6 * xv2) + KA14 * xv8 *
        (KA8 / KA14 + KA10 / KA14 * xv2 + xv4 * (KA12 / KA14 + xv2))));
    tmp_si = (DB) ((KA1 + KA3 * ldxv2) * ldx + (KA7 * y * xv4 * (KA5 / KA7 + xv2) +
        y * xv8 * (KA9 + KA11 * xv2 + KA13 * xv4)));
#endif /* __iset__ < 6 */
    if (im == 0) tmp_si = im;
    if (k0 & 1) {
        si.db = co.db;
        co.db = tmp_si;
    }
    else {
        si.db = tmp_si;
    }
    si.ll += ((LL)(k0 & 2)) << 62;
    co.ll += ((LL)((k0 + 1) & 2)) << 62;

    /* ◊Ÿﬁ…”Ã≈Œ…≈ re=exp(re) */
    y = re * K256ONLN2;
    k = (int) y;

    if (re > (3 * DB_MAX_LOG) || re < (3 * DB_MIN_LOG)) {
        /* Overflow (original real part of x > 3max).  */
        k = re > (3 * DB_MAX_LOG) ? 1023 * 3 : -1023 * 3;
    }
    else {
        ind = k & 255;
#if __iset__ <= 2
        hi = (DB) k;
#else /*  __iset__ <= 2 */
#pragma asm_inline
    __asm ("fdtoifd 0x3,%1,%0" : "=r" (hi): "r" (y));
#endif /*  __iset__ <= 2 */
#if __iset__ >= 6
        lres.value = re;
        ly.value = hi;
        lres.llong = __builtin_e2k_fnmad (LKLN2ON256_hi, ly.llong, lres.llong);
        y = lres.value - hi * KLN2ON256_lo;
#else /* __iset__ < 6 */
        y = re - hi * KLN2ON256_hi - hi * KLN2ON256_lo;
#endif /* __iset__ < 6 */
        yv2 = y * y;
#if __iset__ >= 6
        ly.value = y;
        lyv2.value = yv2;
        lyv4.value = yv2 * yv2;
        lres.llong = __builtin_e2k_fmad (LKB3, ly.llong, LKB2);
        lres1.llong = __builtin_e2k_fmad (lres.llong, lyv2.llong, ly.llong);
        lres.llong = __builtin_e2k_fmad (LKB5, ly.llong, LKB4);
        lres.llong = __builtin_e2k_fmad (lres.llong, lyv4.llong, lres1.llong);
        y = lres.value;
#else /* __iset__ < 6 */
        y = KB2 * y * (y + 1 / KB2) + KB3 * y * yv2 + yv2 * yv2 * (KB5 * y + KB4);
#endif /* __iset__ < 6 */
        hi = constexp[ind];
#if __iset__ >= 6
        ly.value = y;
        lres.value = constexp_lo[ind];
        arg.value = hi;
        lres.llong = __builtin_e2k_fmad (arg.llong, ly.llong, lres.llong);
        arg.value = lres.value + hi;
#else /* __iset__ < 6 */
        arg.value = hi * y + constexp_lo[ind] + hi;
#endif /* __iset__ < 6 */
        k = k >> 8;
        if (__glibc_likely (k >= -1022 && k <= 1023)) {
            arg.llong = __builtin_e2k_paddd (arg.llong, ((LL) k) << 52);
            __real__ res = arg.value * co.db;  /* exp(re)*cos(im) */
            __imag__ res = arg.value * si.db;  /* exp(re)*sin(im) */
            return res;
        }
        if (k > 1023) { /* ƒÃ— –“≈ƒœ‘◊“¡›≈Œ…— –œ‘≈“… ‘œﬁŒœ”‘… –“… œﬁ≈Œÿ Õ¡Ã≈ŒÿÀ…» sin(im) */
            co.db *= DVAIN1022;
            si.db *= DVAIN1022;
            k -= 1022;
        }
        co.db *= arg.value;  /* exp(re)*cos(im) */
        si.db *= arg.value;  /* exp(re)*sin(im) */
    }
    __real__ res = __scalbn (co.db, k);
    __imag__ res = __scalbn (si.db, k);
    return res;
}

weak_alias (__cexp, cexp)
#ifdef NO_LONG_DOUBLE
strong_alias (__cexp, __cexpl)
weak_alias (__cexp, cexpl)
#endif
