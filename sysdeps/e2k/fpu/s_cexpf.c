/* Copyright (c) 2016-2018 ZAO "MCST". All rights reserved. */

/* Return value of complex exponential function for float complex value.
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
%     ðòïçòáííá  ÷ùþéóìñåô  úîáþåîéå  æõîëãéé            %
%  F(Z) = åèò (Z)  ëïíðìåëóîïçï  áòçõíåîôá.              %
%  îåäïðõóôéíùí     úîáþåîéåí     áòçõíåîôá   ñ÷ìñåôóñ   %
%  RE > 709.7827128933839731 éìé                         %
%  RE < -745.1332191019412221 .                          %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define KA8 9.05183071609456021563e-04
#define KA6 -2.08608471166298886810e-02
#define KA4 2.53669325182559742429e-01
#define KA2 -1.23370054671140197041e+00
#define KA9 1.58206303556477327304e-04
#define KA7 -4.68126625672624547281e-03
#define KA5 7.96925827256306198148e-02
#define KA3 -6.45964096146795350000e-01
#define KA1 1.57079632678809990359e+00
#define LKA8 0x3f4da939d840136bLL
#define LKA6 0xbf955c8bc087f7d0LL
#define LKA4 0x3fd03c1e43ea1435LL
#define LKA2 0xbff3bd3cc8d2ecb7LL
#define LKA9 0x3f24bc85ccb213adLL
#define LKA7 0xbf732ca9d79fc665LL
#define LKA5 0x3fb466bbac8a54e8LL
#define LKA3 0xbfe4abbce56ae6e6LL
#define LKA1 0x3ff921fb5443b587LL

#define KE2 3.66556671660783833261e-06
#define KE1 2.70760782821392980564e-03
#define K256ONLN2 369.3299304675746271

__complex__ float
__cexpf (__complex__ float x)
{
    _type_double_bits ex;
    _type_float_bits arg, absre, absim;
    FL re, im;
    int signre;

    _type_double_bits psi, pco;
    DB y, yv2, yv4;
    long ind;
    int k;
#if __iset__ >= 6
    _type_double_bits lyv2, lyv3, lyv4, lres, lres1;
#endif /* __iset__ < 6 */

    __complex__ float res;

    re = __real__ x;
    im = __imag__ x;

    absre.value = ffabs (re);
    absim.value = ffabs (im);

    if (__glibc_unlikely (absre.int0 >= 0x7f800000)) { /* |re| == Inf or Nan */
        if (absre.int0 == 0x7f800000) { /* |re| = Inf */
            /* Real part is infinite.  */
            arg.value = re;
            signre = arg.int0 & 0x80000000;

            if (absim.int0 < 0x7f800000) {
                /* Imaginary part is finite.  */
                FL value = (signre) ? 0 : Inf_F;

                if (absim.int0 == 0) {
                    /* Imaginary part is 0.0.  */
                    __real__ res = value;
                    __imag__ res = im;
                }
                else {
                    FL sinix, cosix;

                    __sincosf (im, &sinix, &cosix);
                    __real__ res = __copysignf (value, cosix);
                    __imag__ res = __copysignf (value, sinix);
                }
            }
            else if (signre == 0) {
                __real__ res = Inf_F;
                __imag__ res = NaN_F;
            }
            else {
                __real__ res = 0;
                __imag__ res = __copysignf (0, im);
            }
        }
        else {
            /* If the real part is NaN the result is NaN + iNaN unless the
               imaginary part is zero.  */
            __real__ res = NaN_F;
            __imag__ res = (absim.int0 == 0) ? im : NaN_F;
        }
        return res;
    }
    if (__glibc_unlikely (absim.int0 >= 0x7f800000)) { /* |im| == Inf or Nan */
        /* If the imaginary part is +-inf or NaN and the real part
           is not +-inf the result is NaN + iNaN.  */
        __real__ res = __imag__ res = NaN_F;
        return res;
    }

    /* ×ÙÞÉÓÌÅÎÉÅ si=sin(im) É co=cos(im) */
    arg.value = absim.value;
    if (unlikely (arg.int0 > 0x43c90fdb)) { /* 2^7*pi */
        y = __rem_pio2f (arg.value, &ind);
        k = ind;
    }
    else {
        y = K2ONPI * (DB) arg.value;
#if __iset__ <= 2
        psi.value = y + DVAIN52;
        y -= (DB) (k = psi.llong);
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
        pco = psi;
        psi.value = y;
    }
    else pco.value = y;
    arg.value = im;
    psi.llong += (LL) ((((arg.int0 >> 31) << 1) + k) & 2) << 62;
    pco.llong += (LL) (k + (k & 1)) << 62;

    /* ×ÙÞÉÓÌÅÎÉÅ re=exp(re) */
    y = K256ONLN2 * (DB) re;

    if (re > (3 * FL_MAX_LOG) || re < (3 * FL_MIN_LOG)) {
        /* Overflow (original real part of x > 3max).  */
        ex.value = re > (3 * FL_MAX_LOG) ? DB_MAX : 0;
    }
    else {
        k = (int) y;
#if __iset__ <= 2
        y -= (DB) k;
#else /*  __iset__ <= 2 */
#pragma asm_inline
        __asm ("fdtoifd 0x3,%1,%0" : "=r" (yv2): "r" (y));
        y -= yv2;
#endif /*  __iset__ <= 2 */
        ex.value = constnexp[k & 255];
        ex.llong = __builtin_e2k_paddd (ex.llong, ((LL) (k >> 8)) << 52);
#if __iset__ >= 6
        lres.value = KE2 * y * (y + KE1 / KE2);
        ex.llong = __builtin_e2k_fmad (lres.llong, ex.llong, ex.llong);
#else /* __iset__ < 6 */
        ex.value = KE2 * y * (y + KE1/KE2) * ex.value + ex.value;
#endif /* __iset__ < 6 */
    }
    __real__ res = (FL) (ex.value * pco.value);  /* exp(re)*cos(im) */
    __imag__ res = (FL) (ex.value * psi.value);  /* exp(re)*sin(im) */
    return res;
}

#ifndef __cexpf
weak_alias (__cexpf, cexpf)
#endif
