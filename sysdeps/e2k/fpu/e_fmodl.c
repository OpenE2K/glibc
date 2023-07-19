/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%    ПРОГРАММА  ВЫЧИСЛЯЕТ  ВЕЩЕСТВЕННЫЙ  ОСТАТОК  ЧИСЛА  %
%  X  ПО  МОДУЛЮ  Y. НЕДОПУСТИМЫМ  ЗНАЧЕНИЕМ  АРГУМЕНТА  %
%  ЯВЛЯЕТСЯ  Y = 0  ИЛИ  |X| = Inf.                      %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

//#define TRUNC 6.77626357803440271255e-20L /* 1.25 / 2 ^ 64 */

LD __fmodl (LD X, LD Y)
{
    _type_long_double_bits absx, absy, ldz;
    LD hy, ly, recipy;
    LI ix, iy, sign, need_correct = 0;

    ldz.value = X;
    sign = ldz.twolong.short0 & 0x8000;
    ix = ldz.twolong.short0 & 0x7fff;

    absx.value = ldfabs (X);
    absy.value = ldfabs (Y);
    iy = absy.twolong.short0;

    if (absx.value < absy.value)
        return X;

    if (unlikely (X != X || Y != Y))  /* X = NaN или Y = NaN */
        return (X * Y) / (X * Y);
    if (unlikely (Y == 0 || ix == 0x7fff)) /* Y = 0 или X = Inf */
        return SVID_libm_err (X, Y, 227);

    if (iy == 0) { /* Y - денормализованное */
        absy.value *= DVAIN63;
        iy = absy.twolong.short0;
        absx.value = __fmodl (absx.value, absy.value);
        absx.value *= DVAIN63;
        ix = absx.twolong.short0;
        need_correct = 1;
    }
    else {
        if (ix > (16382 + iy)) { /* частное X/Y может переполниться */
            absx.value = __fmodl (absx.value, absy.value * DVAIN16382); /* 2^16382 */
            ix = absx.twolong.short0;
        }
    }
    recipy = 1 / absy.value;

    ldz.value = absy.value;
    ldz.twolong.long1 &= 0xffffffff00000000LL; /* делим на 2 половины по 32 разряда */
    ly = absy.value - ldz.value;
    hy = ldz.value;

#pragma loop count (2)
    while (ix > (31 + iy)) {
        ldz.value = absx.value * recipy; /* получаем частное */
//        ldz.value -= ldz.value * TRUNC; /* уменьшаем на 1 ulp */
        ldz.twolong.long1 &= 0xffffffff00000000LL; /* 32 разряда оставляем */
        absx.value -= ldz.value * hy; /* вычитаем частное, умноженное на старшую половину делителя */
        absx.value -= ldz.value * ly; /* вычитаем частное, умноженное на младшую половину делителя */
        ix = absx.twolong.short0;
    }

    ldz.value = (LD) (LL) (absx.value * recipy); /* получаем частное с обрубанием при преобразовании в целое */
    absx.value -= ldz.value * hy; /* вычитаем частное, умноженное на старшую половину делителя */
    absx.value -= ldz.value * ly; /* вычитаем частное, умноженное на младшую половину делителя */
    if (absx.value < 0) { /* возможно нужна коррекция */
        absx.value += absy.value;
    }
    else if (absx.value == absy.value) {
        absx.value = 0;
    }
    if (need_correct) { /* был коррекция X и Y */
        absx.value *= (1 / DVAIN63);
    }
    absx.twolong.short0 += sign; /* формируем знак результата */
    return absx.value;
}

strong_alias (__fmodl, __fmodl_finite)
weak_alias (__fmodl, fmodl)
weak_alias (__fmodl, __ieee754_fmodl)
