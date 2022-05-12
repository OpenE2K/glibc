/* Copyright (c) 2016-2018 ZAO "MCST". All rights reserved. */

/* Return cosine of complex double value.
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
%     ÚÔÁÚ·ÌÌ·  ˜˘˛ÈÛÏÒÂÙ  ˙Ó·˛ÂÓÈÂ  ÊıÓÎ„ÈÈ  'ÎÔÛÈÓıÛ' %
%  ÎÔÌÏÂÎÛÓÔÁÔ  ·ÚÁıÌÂÓÙ·.  ÓÂ‰ÔıÛÙÈÌ˘Ì  ˙Ó·˛ÂÓÈÂÌ     %
%  ·ÚÁıÌÂÓÙ·  Ò˜ÏÒÂÙÛÒ  IM > 710.4758600739439771.       %
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

#define KD15 7.72727099093074677037e-13 /* sinh for |x| < 0.9 */
#define KD13 1.60581501778014887911e-10
#define KD11 2.50521133247516736418e-08
#define KD9 2.75573192099768740862e-06
#define KD7 1.98412698412887581019e-04
#define KD5 8.33333333333332375550e-03
#define KD3 1.66666666666666666746e-01
#define LKD15 0x3d6b301b612530f6LL
#define LKD13 0x3de611f59548abd6LL
#define LKD11 0x3e5ae645c0efd0d5LL
#define LKD9  0x3ec71de3a5244e2aLL
#define LKD7  0x3f2a01a01a01bb5dLL
#define LKD5  0x3f8111111111110cLL
#define LKD3  0x3fc5555555555555LL

#define KE14 1.15588856334052064971e-11 /* cosh for |x| < 0.7 */
#define KE12 2.08761083951577682032e-09
#define KE10 2.75573216530826621342e-07
#define KE8 2.48015872967165158214e-05
#define KE6 1.38888888888939018688e-03
#define KE4 4.16666666666666439202e-02
#define LKE14 0x3da96b12fa1d084dLL
#define LKE12 0x3e21eeb46cc41955LL
#define LKE10 0x3e927e4fd2d2046cLL
#define LKE8  0x3efa01a019ebb078LL
#define LKE6  0x3f56c16c16c1751fLL
#define LKE4  0x3fa5555555555552LL

__complex__ double
__ccos (__complex__ double x)
{
    _type_double_bits arg, absre, absim, exp, exm;
    DB re, im, y, yv2, yp, ym;
    LL signre, signim;
    LL k, ind, km, indm;

    type_union_64f darg1, si, co;
    DB t0, y0, tmp_si, xv2, xv4;
    LD ldx, ldxv2;
    LL k0;
    int k1;
#if __iset__ >= 6
    _type_double_bits ly, lyv2, lyv4, lyv8, lres, lres1, lres2;
#else /* __iset__ < 6  */
    DB xv8;
#endif /* __iset__ < 6  */

    __complex__ double res;

    re = -__imag__ x;
    im = __real__ x;

    /* ƒ¡Ã≈≈ …”»œƒŒŸ  ‘≈À”‘ ccosh() */
    absre.value = re;
    signre = absre.llong & 0x8000000000000000LL;
    absim.value = im;
    signim = absim.llong & 0x8000000000000000LL;
    absre.value = dfabs (re);
    absim.value = dfabs (im);

    if (__glibc_unlikely (absre.llong >= 0x7ff0000000000000LL)) { /* |re| == Inf or Nan */
        if (absre.llong == 0x7ff0000000000000LL) { /* |re| == Inf */
            /* Real part is infinite.  */
            if (absim.llong == 0) {
                /* Imaginary part is 0.0.  */
                __real__ res = Inf;
                absre.llong = signre ^ signim;
                __imag__ res = absre.value;
            }
            else if (absim.llong < 0x7ff0000000000000LL) {
                /* Imaginary part is finite.  */
                _type_double_bits sinix, cosix;

                __sincos (im, &sinix.value, &cosix.value);
                sinix.llong &= 0x8000000000000000LL;
                cosix.llong &= 0x8000000000000000LL;
                sinix.llong += 0x7ff0000000000000LL + signre;
                cosix.llong += 0x7ff0000000000000LL;
                __real__ res = cosix.value;
                __imag__ res = sinix.value;
            }
            else {
                __real__ res = Inf;
                __imag__ res = NaN;
            }
        }
        else {
            __real__ res = NaN;
            __imag__ res = (absim.llong == 0) ? im : NaN;
        }
        return res;
    }
    if (__glibc_unlikely (absim.llong >= 0x7ff0000000000000LL)) { /* |im| == Inf or Nan */
        __real__ res = NaN;
        __imag__ res = (absre.llong == 0) ? 0 : NaN;
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

    /* ◊Ÿﬁ…”Ã≈Œ…≈ exm=sinh(re) … exp=cosh(re) */
    si.ll += signre;    /* sin(im) with sinh(re)'s sign */
    y = absre.value * K256ONLN2;
    k = (LL) y;

    if (absre.value > (3 * DB_MAX_LOG)) {
        /* Overflow (original real part of x > 3max).  */
        k = 1023 * 3;
    }
    else {
        km = -k;
#if __iset__ <= 2
        y = (DB) k;
#else /*  __iset__ <= 2 */
#pragma asm_inline
    __asm ("fdtoifd 0x3,%1,%0" : "=r" (y): "r" (y));
#endif /*  __iset__ <= 2 */
#if __iset__ >= 6
        ly.value = y;
        lres.llong = __builtin_e2k_fnmad (LKLN2ON256_hi, ly.llong, absre.llong);
        y = lres.value - y * KLN2ON256_lo;
#else /* __iset__ < 6 */
        y = absre.value - y * KLN2ON256_hi - y * KLN2ON256_lo;
#endif /* __iset__ < 6 */
        yv2 = y * y;
#if __iset__ >= 6
        ly.value = y;
        lyv2.value = yv2;
        lyv4.value = yv2 * y;
        lres.llong = __builtin_e2k_fmad (LKB4, lyv2.llong, LKB2);
        ym = lres.value * yv2;
        lres.llong = __builtin_e2k_fmad (LKB5, lyv2.llong, LKB3);
        lres.llong = __builtin_e2k_fmad (lres.llong, lyv4.llong, ly.llong);
        yv2 = lres.value;
#else /* __iset__ < 6 */
        ym = KB4 * yv2 * (KB2 / KB4 + yv2);
        yv2 = y + KB5 * y * yv2 * (KB3 / KB5 + yv2);
#endif /* __iset__ < 6 */
        yp = ym + yv2;
        ym = ym - yv2;
        ind = k & 255;
        arg.value = constexp[ind];
#if __iset__ >= 6
        ly.value = yp;
        exp.value = constexp_lo[ind];
        exp.llong = __builtin_e2k_fmad (arg.llong, ly.llong, exp.llong);
#else /* __iset__ < 6 */
        exp.value = arg.value * yp + constexp_lo[ind];
#endif /* __iset__ < 6 */
        k = k >> 8;

        if (__glibc_likely (k >= -1021 && k <= 1024)) {
            k = (k - 1) << 52;
            indm = km & 255;
            km = ((km >> 8) + 2) << 52;
            arg.llong += k;
            exp.llong += k;
            exm.value = constexp[indm];
            exm.llong += km;
#if __iset__ >= 6
            ly.value = ym;
            exm.llong = __builtin_e2k_fmad (exm.llong, ly.llong, exm.llong);
            exm.value *= 0.125;
#else /* __iset__ < 6 */
            exm.value = (exm.value * ym + exm.value) * 0.125;
#endif /* __iset__ < 6 */
            ym = exp.value - exm.value + arg.value; /* sinh(re) */

            yv2 = re * re;
            yp = yv2 * yv2;

            if (absre.llong < 0x3fe6666666666666LL) {        /* cosh for |re| < 0.7 */
#if __iset__ >= 6
                lyv2.value = yv2;
                lyv4.value = yp;
                lyv8.value = yp * yp;
                lres.llong = __builtin_e2k_fmad (LKE6, lyv2.llong, LKE4);
                lres1.llong = __builtin_e2k_fmad (LKE10, lyv2.llong, LKE8);
                lres1.llong = __builtin_e2k_fmad (lres1.llong, lyv4.llong, lres.llong);
                lres.llong = __builtin_e2k_fmad (LKE14, lyv2.llong, LKE12);
                lres1.llong = __builtin_e2k_fmad (lres.llong, lyv8.llong, lres1.llong);
                lres.value = 0.5 * yv2;
                lres.llong = __builtin_e2k_fmad (lres1.llong, lyv4.llong, lres.llong);
                exp.value = lres.value + 1.0;
#else /* __iset__ < 6 */
                exp.value = yv2 * (KE4 * yv2) + yv2 * yp * (KE8 * yv2 + KE6) + yp * yp * (KE14
                    * yv2 * yp + KE12 * yv2 * (yv2 + KE10 / KE12)) + 0.5 * yv2 + 1.0;
#endif /* __iset__ < 6 */
            }
            else exp.value = exp.value + exm.value + arg.value;

            if (absre.llong < 0x3fecccccccccccccLL) {        /* sinh for |re| < 0.9 */
#if __iset__ >= 6
                lyv2.value = yv2;
                lyv4.value = yp;
                lyv8.value = yp * yp;
                lres.llong = __builtin_e2k_fmad (LKD5, lyv2.llong, LKD3);
                lres1.llong = __builtin_e2k_fmad (LKD9, lyv2.llong, LKD7);
                lres1.llong = __builtin_e2k_fmad (lres1.llong, lyv4.llong, lres.llong);
                lres.llong = __builtin_e2k_fmad (LKD15, lyv2.llong, LKD13);
                lres.llong = __builtin_e2k_fmad (lres.llong, lyv2.llong, LKD11);
                lres1.llong = __builtin_e2k_fmad (lres.llong, lyv8.llong, lres1.llong);
                lres.value = absre.value * yv2;
                exm.llong = __builtin_e2k_fmad (lres1.llong, lres.llong, absre.llong);
#else /* __iset__ < 6 */
                exm.value = KD5 * absre.value * yv2 * (KD3 / KD5 + yv2) + KD11 * absre.value * yv2 * yp * (yv2 * (KD9 / KD11
                    + yv2) + KD7 / KD11) + absre.value * yp * (yp * yp) * (KD15 * yv2 + KD13) + absre.value;
#endif /* __iset__ < 6 */
            }
            else exm.value = ym;
            __real__ res = exp.value * co.db;   /* cos(im)*cosh(re) */
            __imag__ res = exm.value * si.db;   /* sin(im)*sinh(re) */
            return res;
        }

        if (k > 1024) { /* ƒÃ— –“≈ƒœ‘◊“¡›≈Œ…— –œ‘≈“… ‘œﬁŒœ”‘… –“… œﬁ≈Œÿ Õ¡Ã≈ŒÿÀ…» sin(im) */
            co.db *= DVAIN1022;
            si.db *= DVAIN1022;
            k -= 1022;
        }
        k = k - 1;
        exp.value = exp.value + arg.value; /* cosh(re) == sinh(re) */
        co.db = exp.value * co.db;   /* cos(im)*cosh(re) */
        si.db = exp.value * si.db;   /* sin(im)*sinh(re) */
    }
    __real__ res = __scalbn (co.db, k);
    __imag__ res = __scalbn (si.db, k);
    return res;
}

weak_alias (__ccos, ccos)
#ifdef NO_LONG_DOUBLE
strong_alias (__ccos, __ccosl)
weak_alias (__ccos, ccosl)
#endif
