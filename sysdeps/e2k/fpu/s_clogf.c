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

#if __iset__ >= 6
#define KB19 -1.7011700640685973283e-03
#define KB17  1.0487649254949306388e-02
#define KB15 -3.0351864785781172001e-02
#define KB13  5.7089555930306193354e-02
#define KB11 -8.3497249683016014354e-02
#define KB9   1.0932341501303093068e-01
#define KB7  -1.4260016082820906047e-01
#define KB5   1.9998075281148713911e-01
#define KB3  -3.3333276291982549822e-01
#define KB1   9.9999999716054543963e-01
#define LKB19 0xbf5bdf39729124b6LL
#define LKB17 0x3f857a8c747ed314LL
#define LKB15 0xbf9f148f2a829af1LL
#define LKB13 0x3fad3ad79f53e142LL
#define LKB11 0xbfb5601364b1cac3LL
#define LKB9  0x3fbbfc9e8c2b0243LL
#define LKB7  0xbfc240b8d994abf9LL
#define LKB5  0x3fc998f82493d066LL
#define LKB3  0xbfd55552f0db4320LL
#define LKB1  0x3feffffffe79bf93LL
#else /* __iset__ < 6 */
#define KA9 1.00523913835726148843e-01
#define KA7 -1.42452689179072812273e-01
#define KA5 1.99994988952939798604e-01
#define KA3 -3.33333323562239806408e-01
#define KB3 -3.33327229950751257532e-01
#endif

#define LN2 0.6931471805599453094
#define KC3 6.66690704586487441045e-01
#define KD5 (4.00297520051000810194e-01*0.5)
#define KD3 (6.66666627963472760429e-01*0.5)

__complex__ float
__clogf (__complex__ float x)
{
    _type_float_bits absre, absim;
    FL re, im;
    DB y, yp, yv2;
    int signre, signim;

    _type_float_bits arg1, arg2;
    _type_double_bits y0, darg;
#if __iset__ >= 6
    _type_double_bits lyv2, lyv4, lyv5, lres, lres1, lres2;
#else /* __iset__ < 6 */
    LL ind;
#endif /* __iset__ < 6 */

    __complex__ float res;

    re = __real__ x;
    im = __imag__ x;

    absre.value = re;
    signre = (absre.int0 >> 30) & 2;
    absim.value = im;
    signim = (absim.int0 >> 30) & 2;
    absre.value = ffabs (re);
    absim.value = ffabs (im);

    if (__glibc_unlikely (absre.int0 == 0 && absim.int0 == 0)) {
        /* Real and imaginary part are 0.0.  */
        __imag__ res = signre ? (FL) M_PI : 0;
        __imag__ res = __copysignf (__imag__ res, im);
        /* Yes, the following line raises an exception.  */
        __real__ res = -1.0f / absre.value;
        return res;
    }
    if (__glibc_unlikely (absre.int0 > 0x7f800000 || absim.int0 > 0x7f800000)) { /* |re| …Ã… |im| == Nan */
        __imag__ res = NaN_F;
        if (absre.int0 == 0x7f800000 || absim.int0 == 0x7f800000)
          /* Real or imaginary part is infinite.  */
            __real__ res = Inf_F;
        else
            __real__ res = NaN_F;
        return res;
    }

    /* ◊Ÿﬁ…”Ã≈Œ…≈ im=atan2(im,re) */
    arg1.value = absre.value;
    arg2.value = absim.value;
    if (__glibc_unlikely (arg2.int0 == 0x7f800000)) {
        /* |Y| == Inf */
        __real__ res = Inf_F;

        if (arg1.int0 == 0x7f800000) {  /* |X| == Inf */
            /* atan2(+/-Inf,-Inf) returns +/-3pi/4 */
            /* atan2(+/-Inf,Inf) returns +/-pi/4 */
            __imag__ res = (FL) (PION2 * 0.5) * (FL) ((signre + 1) * (1 - signim));
        }
        else { /* atan2(+/-Inf,x) returns +/-pi/2 for finite x */
            __imag__ res = (FL) PION2 * ((FL) (1 - signim));
        }
        return res;
    }
    if (__glibc_unlikely (arg1.int0 == 0x7f800000 || im == 0)) {
        /* |X| == Inf or |Y| == 0 */
        if (arg1.int0 == 0x7f800000) {
            __real__ res = Inf_F;
        }
        else {
            __real__ res = __ieee754_logf (arg1.value);
        }
        /* atan2(+/-y,-Inf) returns +/-pi for finite |y| > 0; */
        /* atan2(+/-0,x) returns +/-pi for x < 0 or x = -0 */
        /* atan2(+/-y,Inf) returns +/-0 for finite |y| > 0 */
        /* atan2(+/-0,x) returns +/-0 for x > 0 or x = +0 */
        arg1.value = ((FL) (signre >> 1)) * (FL) PI;
        arg1.int0 |= signim << 30;
        __imag__ res = arg1.value;
        return res;
    }

    y = (DB) arg1.value;
    yv2 = (DB) arg2.value;
    yp = y * y + yv2 * yv2;
    if (arg2.int0 > arg1.int0) {
        darg.value = y / yv2; /* min / max */
        y0.value = (yv2 - 1) * (yv2 + 1) + y * y; /* (max-1)*(max+1)+min^2 */
    }
    else {
        darg.value = yv2 / y; /* min / max */
        y0.value = (y - 1) * (y + 1) + yv2 * yv2; /* (max-1)*(max+1)+min^2 */
    }
#if __iset__ >= 6
    lyv2.value = darg.value * darg.value;
    lyv4.value = lyv2.value * lyv2.value;
    lres.value = KB1 * darg.value;
    lres1.value = KB3 * darg.value;
    lres2.llong = __builtin_e2k_fmad (lres1.llong, lyv2.llong, lres.llong);
    lres.llong = __builtin_e2k_fmad (LKB7, lyv2.llong, LKB5);
    lres1.llong = __builtin_e2k_fmad (LKB11, lyv2.llong, LKB9);
    lres1.llong = __builtin_e2k_fmad (lres1.llong, lyv4.llong, lres.llong);
    lyv5.value = lyv4.value * darg.value;
    lres.llong = __builtin_e2k_fmad (lres1.llong, lyv5.llong, lres2.llong);
    y = lres.value;
    lres.llong = __builtin_e2k_fmad (LKB15, lyv2.llong, LKB13);
    lres1.llong = __builtin_e2k_fmsd (LKB19 ^ 0x8000000000000000LL, lyv2.llong, LKB17);
    lres.llong = __builtin_e2k_fmsd (lres1.llong, lyv4.llong, lres.llong);
    yv2 = lyv5.value * (lyv4.value * lyv4.value) * lres.value;
//    y = KB1 * darg.value + KB5 * darg.value * yv2 * (KB3/KB5 + yv2) +
//        darg.value * yv2 * yv4 * (KB7 + KB9 * yv2 + KB11 * yv4);
//    yv2 = -(KB19 * darg.value * yv4 * (yv4 * yv4) * (KB13/KB19 + KB15/KB19 * yv2 + yv4 * (yv2 + KB17/KB19)));
#else /* __iset__ < 6 */
    if (darg.llong <= 0x3fd0000000000000LL) { /* |x/y|<=0.25 */
        yv2 = darg.value * darg.value;
        y = darg.value + KA5 * darg.value * yv2 * (KA3/KA5 + yv2);
        yv2 = -KA9 * darg.value * yv2 * (KA7/KA9 + yv2) * (yv2 * yv2);
    }
    else {
        ind = (darg.llong - 0x3fd0000000000000LL) >> 46;
        y = darg.value;
        darg.llong = __builtin_e2k_pandd (darg.llong, 0x3fffc00000000000LL);
        y = (y - darg.value) / (1.0 + y * darg.value);
        yv2 = -KB3 * y * (y * y);
        y = y + constnarctg[ind];
    }
#endif /* __iset__ < 6 */
    if (arg2.int0 > arg1.int0) {
        darg.value = (signre) ? -PION2 : PION2;
        signre ^= 2;
    }
    else darg.value = (signre) ? PI : 0.0;
    y -= darg.value;
    y = (signre) ? yv2 - y : y - yv2;
    __imag__ res = (FL) ((signim) ? -y : y);

    /* ◊Ÿﬁ…”Ã≈Œ…≈ re=0.5*log(im^2+re^2) */
    if (dfabs (y0.value) <= 0.0625) { /* |im^2+re^2-1| <= 0.0625 */
        yp = y0.value / (yp + 1);
        yv2 = yp * yp;
        y = y0.value * 0.5;
#if __iset__ >= 6
        y0.value = y;
        lyv2.value = y * y;
        lres1.llong = __builtin_e2k_fnmad (y0.llong, y0.llong, y0.llong);
        lres.value = yp;
        y0.llong = __builtin_e2k_fmad (lres.llong, lyv2.llong, lres1.llong);
        lres.value = KD5 * yp * yv2;
        lres1.value = KD3 / KD5 + yv2;
        lres.llong = __builtin_e2k_fmad (lres.llong, lres1.llong, y0.llong);
        y = lres.value;
#else /* __iset__ < 6 */
        y = y + y * y * (yp - 1) + KD5 * yp * yv2 * (KD3 / KD5 + yv2);
#endif /* __iset__ < 6 */
        __real__ res = (FL) y;
        return res;
    }
    y0.value = yp;
    y0.llong = __builtin_e2k_paddd (y0.llong, 0x400000000000LL);
    y0.llong = __builtin_e2k_pandd (y0.llong, 0x7fff800000000000LL);
    y = (yp - y0.value) / (yp + y0.value);
#if __iset__ >= 6
    y0.value = y + (LN2 / 2.0 * (DB) ((y0.llong >> 52) - 1023) + 0.5 * constnln[(y0.llong >> 47) & 0x1f]);
    lyv2.value = y * y;
    lres1.value = KC3 / 2.0 * y;
    lres.llong = __builtin_e2k_fmad (lres1.llong, lyv2.llong, y0.llong);
    y = lres.value;
#else /* __iset__ < 6 */
    y = y + (LN2 / 2.0 * (DB) ((y0.llong >> 52) - 1023)
        + 0.5 * constnln[(y0.llong >> 47) & 0x1f]) + KC3 / 2.0 * y * (y * y);
#endif /* __iset__ < 6 */
    __real__ res = (FL) y;
    return res;
}

#ifndef __clogf
weak_alias (__clogf, clogf)
#endif
