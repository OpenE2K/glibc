/* Copyright (c) 2016-2018 ZAO "MCST". All rights reserved. */

/* Complex sine hyperbole function for float.
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
%    ðòïçòáííá ÷ùþéóìñåô úîáþåîéå æõîëãéé   'óéîõó'      %
%  çéðåòâïìéþåóëéê' ëïíðìåëóîïçï áòçõíåîôá. îåäïðõóôéíùí %
%  úîáþåîéåí áòçõíåîôá ñ÷ìñåôóñ IM > 710.4758600739439771%
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define KB9 2.82880520977637142049e-06
#define KB7 1.98346674682791327720e-04
#define KB5 8.33335237769337037738e-03
#define KB3 1.66666665809424354159e-01
#define LKB9 0x3ec7bad01c45387eLL
#define LKB7 0x3f29ff68f649f313LL
#define LKB5 0x3f8111139f6d0773LL
#define LKB3 0x3fc55555537e0f34LL

#define KE2 3.66556671660783833261e-06
#define KE1 2.70760782821392980564e-03

#define KA8  9.05183071609456021563e-04
#define KA6 -2.08608471166298886810e-02
#define KA4  2.53669325182559742429e-01
#define KA2 -1.23370054671140197041e+00
#define KA9  1.58206303556477327304e-04
#define KA7 -4.68126625672624547281e-03
#define KA5  7.96925827256306198148e-02
#define KA3 -6.45964096146795350000e-01
#define KA1  1.57079632678809990359e+00
#define LKA8 0x3f4da939d840136bLL
#define LKA6 0xbf955c8bc087f7d0LL
#define LKA4 0x3fd03c1e43ea1435LL
#define LKA2 0xbff3bd3cc8d2ecb7LL
#define LKA9 0x3f24bc85ccb213adLL
#define LKA7 0xbf732ca9d79fc665LL
#define LKA5 0x3fb466bbac8a54e8LL
#define LKA3 0xbfe4abbce56ae6e6LL
#define LKA1 0x3ff921fb5443b587LL

__complex__ float
__csinhf (__complex__ float x)
{
    _type_double_bits exp, exm;
    _type_float_bits arg, absre, absim;
    DB yp, ym;
    LL k, km;
    FL re, im;
    int signre, signim;

    _type_double_bits psi, pco;
    DB y, yv2, yv4;
#if __iset__ >= 6
    _type_double_bits ly, lyv2, lyv3, lyv4, lres, lres1, lres2;
#endif /* __iset__ < 6 */
    long ind;

    __complex__ float res;

    re = __real__ x;
    im = __imag__ x;

    absre.value = re;
    signre = absre.int0 & 0x80000000;
    absim.value = im;
    signim = absim.int0 & 0x80000000;
    absre.value = ffabs (re);
    absim.value = ffabs (im);

    if (__glibc_unlikely (absre.int0 >= 0x7f800000)) { /* |re| == Inf or Nan */
        if (absre.int0 == 0x7f800000) { /* |re| == Inf */
            /* Real part is infinite.  */
            if (absim.int0 == 0) {
                /* Imaginary part is 0.0.  */
                absre.value = Inf_F;
                absre.int0 += signre;
                __real__ res = absre.value;
                __imag__ res = im;
            }
            else if (absim.int0 < 0x7f800000) {
                /* Imaginary part is finite.  */
                _type_float_bits sinix, cosix;

                __sincosf (im, &sinix.value, &cosix.value);
                sinix.int0 &= 0x80000000;
                cosix.int0 &= 0x80000000;
                sinix.int0 += 0x7f800000;
                cosix.int0 += 0x7f800000 + signre;
                __real__ res = cosix.value;
                __imag__ res = sinix.value;
            }
            else {
                __real__ res = Inf_F;
                __imag__ res = NaN_F;
            }
        }
        else {
            __real__ res = NaN_F;
            __imag__ res = (absim.int0 == 0) ? im : NaN_F;
        }
        return res;
    }
    if (__glibc_unlikely (absim.int0 >= 0x7f800000)) { /* |im| == Inf or Nan */
        absim.int0 = signre;
        __real__ res = (absre.int0 == 0) ? absim.value : NaN_F;
        __imag__ res = NaN_F;
        return res;
    }

    /* ×ÙÞÉÓÌÅÎÉÅ si=sin(im) É co=cos(im) */
    arg.value = absim.value;
    if (__glibc_unlikely (arg.int0 > 0x43c90fdb)) { /* 2^7*pi */
        y = __rem_pio2f (arg.value, &ind);
        k = ind;
    }
    else {
        y = K2ONPI * (DB) arg.value;
#if __iset__ <= 2
        psi.value = y + DVAIN52;
        y -= (DB) (k = (int) psi.llong);
#else /*  __iset__ <= 2 */
#pragma asm_inline
        __asm ("fdtoifd 0x0,%1,%0" : "=r" (psi.value): "r" (y)); /* rc=0 - to nearest */
        y -= psi.value;
        k = (int) psi.value;
#endif /*  __iset__ <= 2 */
    }

    yv2 = y * y;
    yv4 = yv2 * yv2;
#if __iset__ >= 6
    lyv2.value = yv2;
    lyv4.value = yv4;
    lres.llong = __builtin_e2k_fmad (LKA2, lyv2.llong, 0x3ff0000000000000LL);
    lres.llong = __builtin_e2k_fmad (LKA4, lyv4.llong, lres.llong);
    lyv3.value = yv2 * yv4;
    lres1.llong = __builtin_e2k_fmad (LKA8, lyv2.llong, LKA6);
    pco.llong = __builtin_e2k_fmad (lres1.llong, lyv3.llong, lres.llong);
    lyv3.value = y * yv2;
    lres.value = KA1 * y;
    lres1.llong = __builtin_e2k_fmad (LKA5, lyv2.llong, LKA3);
    lres.llong = __builtin_e2k_fmad (lres1.llong, lyv3.llong, lres.llong);
    lyv3.value *= yv4; /* x^7 */
    lres1.llong = __builtin_e2k_fmad (LKA9, lyv2.llong, LKA7);
    psi.llong = __builtin_e2k_fmad (lres1.llong, lyv3.llong, lres.llong);
    y = pco.value;
#else /* __iset__ < 6 */
    psi.value = KA1 * y + KA5 * y * yv2 * (KA3/KA5 + yv2) + KA9 * y * yv2 * yv4 * (KA7/KA9 + yv2);
    y = 1.0 + KA2 * yv2 + KA4 * yv4 + yv2 * yv4 * (KA6 + KA8 * yv2);
#endif /* __iset__ < 6 */
    if (k & 1) {
        pco.value = psi.value;
        psi.value = y;
    }
    else pco.value = y;
    psi.llong += (LL) (((signim >> 30) + k) & 2) << 62;
    pco.llong += (LL) (k + (k & 1)) << 62;

    /* ×ÙÞÉÓÌÅÎÉÅ exm=sinh(re) É exp=cosh(re) */
    pco.llong += (LL) signre << 32;    /* cos(im) with sinh(re)'s sign */
    y = K256ONLN2 * (DB) absre.value;

    if (absre.value > (3 * FL_MAX_LOG)) {
        /* Overflow (original real part of x > 3max).  */
        exp.value = exm.value = DB_MAX;
    }
    else {
        k = (LL) y;
        km = -k;
#if __iset__ <= 2
        y -= (DB) k;
#else /*  __iset__ <= 2 */
#pragma asm_inline
        __asm ("fdtoifd 0x3,%1,%0" : "=r" (ym): "r" (y));
        y -= ym;
#endif /*  __iset__ <= 2 */
#if __iset__ >= 6
        ly.value = y;
        lres.value = KE2 * y;
        lres.llong = __builtin_e2k_fmad (lres.llong, ly.llong, 0x3ff0000000000000LL);
        ym = lres.value;
#else /* __iset__ < 6 */
        ym = KE2 * y * y + 1.0;
#endif /* __iset__ < 6 */
        y = KE1 * y;
        yp = ym + y;
        ym -= y;
        exp.value = constnexp[k & 255];
        exp.llong = __builtin_e2k_paddd (exp.llong, (((k >> 8) - 1) << 52));
        exm.value = constnexp[km & 255];
        exm.llong = __builtin_e2k_paddd (exm.llong, (((km >> 8) - 1) << 52));
#if 0
//#if __iset__ >= 6
        ly.value = ym;
        lres.value = exp.value * yp;
        exp.llong = __builtin_e2k_fmad (exm.llong, ly.llong, lres.llong);
        lres.llong = __builtin_e2k_fnmad (exm.llong, ly.llong, lres.llong);
        y = lres.value;
#else /* __iset__ < 6 */
        y = exp.value * yp - exm.value * ym;
        exp.value = exp.value * yp + exm.value * ym; /* cosh(re) */
#endif /* __iset__ < 6 */

        if (absre.int0 < 0x3f99999a) {   /* |re| < 1.2 */
            y = (DB) absre.value;
            yp = y * y;
#if __iset__ >= 6
            ly.value = y;
            lyv2.value = yp;
            lyv3.value = y * yp;
            lres1.llong = __builtin_e2k_fmad (LKB5, lyv2.llong, LKB3);
            lres2.llong = __builtin_e2k_fmad (lres1.llong, lyv3.llong, ly.llong);
            lyv3.value *= yp * yp; /* x^7 */
            lres1.llong = __builtin_e2k_fmad (LKB9, lyv2.llong, LKB7);
            exm.llong = __builtin_e2k_fmad (lres1.llong, lyv3.llong, lres2.llong);
#else /* __iset__ < 6 */
            exm.value = y + KB5 * y * yp * (yp + KB3 / KB5) + y * yp * (yp * yp) * (KB7 + KB9 * yp);
#endif /* __iset__ < 6 */
        }
        else exm.value = y;                              /* sinh(re) */
    }
    __real__ res = (FL) (exm.value * pco.value);   /* cos(im)*sinh(re) */
    __imag__ res = (FL) (exp.value * psi.value);   /* sin(im)*cosh(re) */
    return res;
}

#ifndef __csinhf
weak_alias (__csinhf, csinhf)
#endif
