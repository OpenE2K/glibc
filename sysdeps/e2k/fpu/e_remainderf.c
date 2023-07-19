/* Copyright (c) 2016-2018 ZAO "MCST". All rights reserved. */

#include <math.h>
#include <math_private.h>

#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%    ПРОГРАММА  ВЫЧИСЛЯЕТ  ВЕЩЕСТВЕННЫЙ  ОСТАТОК  ЧИСЛА  %
%  X  ПО  МОДУЛЮ  Y.  РЕЗУЛЬТАТ  НАХОДИТСЯ  В ДИАПАЗОНЕ  %
%  ОТ -Y/2 ДО +Y/2.  НЕДОПУСТИМЫМ  ЗНАЧЕНИЕМ  АРГУМЕНТА  %
%  ЯВЛЯЕТСЯ  Y = 0  ИЛИ  |X| = Inf.                      %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#if __iset__ >= 6
#define THRESH (31LL << 52)
#else /* __iset__ < 6 */
#define THRESH (28LL << 52)
#endif /* __iset__ < 6 */

FL __remainderf (FL X, FL Y)
{
    _type_double_bits absx, absy, ldz;
    _type_float_bits ax, ay, ret;
    DB recipy, tmp, one_y, half_y;
    LL ix, iy;

    ax.value = ffabs (X);
    ay.value = ffabs (Y);
    ret.value = X;
    ret.int0 &= 0x80000000;

    if (ax.value < ay.value * 0.5f)
        return X;

    absx.value = (DB) ax.value;
    ix = absx.llong;
    one_y = (DB) ay.value;
    half_y = 0.5 * one_y;
    absy.value = 2 * one_y;
    iy = absy.llong;
    recipy = 1 / absy.value;

    if (unlikely (X != X || Y != Y))  /* X = NaN или Y = NaN */
        return (X * Y) / (X * Y);
    if (unlikely (Y == 0 || ax.int0 == 0x7f800000)) /* Y = 0 или X = Inf */
        return SVID_libm_err (X, Y, 128);

    if (ix <= (THRESH + iy)) { /* отдельная ветка для не очень больших x/y */
        tmp = absx.value / absy.value; /* для предотвращения бага gcc */
#if __iset__ <= 2
        ldz.value = (DB) (int) (tmp); /* получаем частное с обрубанием при преобразовании в целое */
#else /*  __iset__ <= 2 */
#pragma asm_inline
        __asm ("fdtoifd 0x3,%1,%0" : "=r" (ldz.value): "r" (tmp));
#endif /*  __iset__ <= 2 */
#if __iset__ >= 6
        absx.llong = __builtin_e2k_fnmad (ldz.llong, absy.llong, absx.llong);
#else /* __iset__ < 6 */
        absx.value -= ldz.value * absy.value; /* вычитаем частное, умноженное на делитель */
#endif /* __iset__ < 6 */

	if (absx.value > half_y) { /* возможно нужна коррекция */
            absx.value -= one_y;
            if (absx.value >= half_y)
                absx.value -= one_y;
        }
        ax.value = (FL) absx.value;
        ret.int0 += ax.int0; /* формируем знак результата */
        return ret.value;
    }

#pragma loop count (10)
    while (ix > (THRESH + iy)) {
        ldz.value = absx.value * recipy; /* получаем частное */
        ldz.llong = __builtin_e2k_pandd (ldz.llong, 0xffffffffff000000LL); /* 29 разрядов оставляем */
#if __iset__ >= 6
        absx.llong = __builtin_e2k_fnmad (ldz.llong, absy.llong, absx.llong);
#else /* __iset__ < 6 */
        absx.value -= ldz.value * absy.value; /* вычитаем частное, умноженное на делитель */
#endif /* __iset__ < 6 */
        ix = absx.llong;
    }

    tmp = absx.value * recipy; /* для предотвращения бага gcc */
#if __iset__ <= 2
    ldz.value = (DB) (int) (tmp); /* получаем частное с обрубанием при преобразовании в целое */
#else /*  __iset__ <= 2 */
#pragma asm_inline
    __asm ("fdtoifd 0x3,%1,%0" : "=r" (ldz.value): "r" (tmp));
#endif /*  __iset__ <= 2 */
#if __iset__ >= 6
    absx.llong = __builtin_e2k_fnmad (ldz.llong, absy.llong, absx.llong);
#else /* __iset__ < 6 */
    absx.value -= ldz.value * absy.value; /* вычитаем частное, умноженное на делитель */
#endif /* __iset__ < 6 */

    if (absx.value > half_y) { /* возможно нужна коррекция */
        absx.value -= one_y;
        if (absx.value >= half_y)
            absx.value -= one_y;
    }
    ax.value = (FL) absx.value;
    ret.int0 ^= ax.int0; /* формируем знак результата */
    return ret.value;
}

strong_alias (__remainderf, __remainderf_finite)
weak_alias (__remainderf, remainderf)
weak_alias (__remainderf, __ieee754_remainderf)
strong_alias (__remainderf, __dremf)
weak_alias (__remainderf, dremf)
