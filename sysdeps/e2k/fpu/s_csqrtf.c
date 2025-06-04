/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

#include <complex.h>
#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ПРОГРАММА  ВЫЧИСЛЯЕТ  КОРЕНЬ  КВАДРАТНЫЙ  ИЗ       %
%  КОМПЛЕКСНОГО  ЧИСЛА.                                  %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

__complex__ float
__csqrtf (__complex__ float x)
{
    _type_float_bits max, min, absre, absim;
    DB X, Y, hyp;
    FL re, im;
    int sign;
    __complex__ float res;

    re = __real__ x;
    im = __imag__ x;
    X = (DB) re;
    Y = (DB) im;

    max.value = im;
    sign = max.int0 & 0x80000000;
    absre.value = ffabs (re);
    absim.value = ffabs (im);

    if (__glibc_unlikely (absre.int0 >= 0x7f800000 ||
                          absim.int0 >= 0x7f800000)) { /* |re| или |im| == Inf or Nan */
        if (absim.int0 == 0x7f800000) {
            __real__ res = Inf_F;
            __imag__ res = im;
        }
        else if (absre.int0 == 0x7f800000) {
            if (re < 0) {
                __real__ res = absim.int0 > 0x7f800000 ? NaN_F : 0;
                max.value = Inf_F;
                max.int0 += sign;
                __imag__ res = max.value;
            }
            else {
                __real__ res = re;
                max.int0 = sign;
                __imag__ res = absim.int0 > 0x7f800000 ? NaN_F : max.value;
            }
        }
        else {
            __real__ res = __imag__ res = NaN_F;
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
    if (__glibc_unlikely (min.int0 == 0)) { /* re = 0 или im = 0 */
        if (max.int0 == 0) { /* re = 0 и im = 0 */
            __real__ res = 0;
            __imag__ res = im;
            return res;
        }
        if (absim.int0 == 0) { /* re != 0, im = 0 */
            min.value = fsqrt (absre.value);
            if (re < 0) {
                __real__ res = 0;
                min.int0 += sign;
            }
            else {
                __real__ res = min.value;
                min.int0 = sign;
            }
            __imag__ res = min.value;
            return res;
        }
        else { /* re = 0, im != 0 */
            min.value = (max.int0 < 0x00100000) ?
                0.5f * fsqrt (2 * absim.value) : fsqrt (0.5f * absim.value);
            __real__ res = min.value;
            min.int0 += sign;
            __imag__ res = min.value;
            return res;
        }
    }
    min.value = max.value - min.value;
    if (min.value == max.value)
        hyp = (DB) max.value;
    else {
        hyp = dsqrt (X * X + Y * Y);
    }
    /* конец вычисления 0.5*hypot(re,im) */
    X = dsqrt (0.5 * (dfabs (X) + hyp));
    Y = 0.5 * dfabs (Y) / X;
    min.value = (FL) X;
    max.value = (FL) Y;
    if (re > 0) {
        __real__ res = min.value;
        max.int0 += sign;
        __imag__ res = max.value;
        return res;
    }
    __real__ res = max.value;
    min.int0 += sign;
    __imag__ res = min.value;
    return res;
}

#ifndef __csqrtf
weak_alias (__csqrtf, csqrtf)
#endif
