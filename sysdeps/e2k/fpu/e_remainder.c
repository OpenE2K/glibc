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
#define THRESH (31ULL << 52)
#else /* __iset__ < 6 */
#define THRESH (26ULL << 52)
#endif /* __iset__ < 6 */

DB __remainder (DB X, DB Y)
{
    _type_double_bits absx, absy, ldz, ret;
    DB recipy, tmp, half_y;
    ULL ix, iy;

    absx.value = dfabs (X);
    absy.value = dfabs (Y);
    ret.value = X;
    ret.llong &= 0x8000000000000000LL;

    if (absx.value < absy.value * 0.5)
        return X;

    absx.value *= 0.5; /* уменьшение 2 раза делимого */
    half_y = 0.5 * absy.value;
    iy = absy.llong;
    ix = absx.llong;
    recipy = 1 / absy.value;
#if __iset__ < 6
    ldz.value = absy.value;
    ldz.llong = __builtin_e2k_pandd (ldz.llong, 0xfffffffffc000000LL); /* делим на 2 половины: 27 и 26 разрядов */
    DB hy = ldz.value;
    DB ly = absy.value - hy;
#endif /*  __iset__ < 6 */

    if (unlikely (X != X || Y != Y))  /* X = NaN или Y = NaN */
        return (X * Y) / (X * Y);
    if (unlikely (Y == 0 || absx.llong == 0x7ff0000000000000LL)) /* Y = 0 или X = Inf*/
        return SVID_libm_err (X, Y, 28);

    if (iy >= (1ULL << 52) && ix <= (THRESH + iy)) { /* отдельная ветка для не очень больших x/y */
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
        absx.value -= ldz.value * hy; /* вычитаем частное, умноженное на старшую половину делителя */
        absx.value -= ldz.value * ly; /* вычитаем частное, умноженное на младшую половину делителя */
#endif /* __iset__ < 6 */
        absx.value *= 2; /* восстановление делимого */
        if (absx.value > half_y) { /* возможно нужна коррекция */
            absx.value -= absy.value;
            if (absx.value >= half_y)
                absx.value -= absy.value;
        }
        absx.llong += ret.llong; /* формируем знак результата */
        return absx.value;
    }

    if (unlikely (!(iy >= (1ULL << 52)))) { /* Y - денормализованное */
        absy.value *= DVAIN52;
        absx.value = __fmod (X, absy.value);
        return __remainder (absx.value * DVAIN52, absy.value) * (1 / DVAIN52);
    }
    if (unlikely (ix > ((1022ULL << 52) + iy))) { /* частное X/Y может переполниться */
        absx.value = __fmod (absx.value, absy.value * DVAIN1022); /* 2^1022 */
        ix = absx.llong;
    }

#pragma loop count (10)
    while (ix > (THRESH + iy)) {
        ldz.value = absx.value * recipy; /* получаем частное */
        ldz.llong = __builtin_e2k_pandd (ldz.llong, 0xfffffffff8000000LL); /* 26 разрядов оставляем */
#if __iset__ >= 6
        absx.llong = __builtin_e2k_fnmad (ldz.llong, absy.llong, absx.llong);
#else /* __iset__ < 6 */
        absx.value -= ldz.value * hy; /* вычитаем частное, умноженное на старшую половину делителя */
        absx.value -= ldz.value * ly; /* вычитаем частное, умноженное на младшую половину делителя */
#endif /* __iset__ < 6 */
        ix = absx.llong;
    }

    tmp = absx.value * recipy; /* получаем частное */
#if __iset__ <= 2
    ldz.value = (DB) (int) (tmp); /* получаем частное с обрубанием при преобразовании в целое */
#else /*  __iset__ <= 2 */
#pragma asm_inline
    __asm ("fdtoifd 0x3,%1,%0" : "=r" (ldz.value): "r" (tmp));
#endif /*  __iset__ <= 2 */
#if __iset__ >= 6
    absx.llong = __builtin_e2k_fnmad (ldz.llong, absy.llong, absx.llong);
#else /* __iset__ < 6 */
    absx.value -= ldz.value * hy; /* вычитаем частное, умноженное на старшую половину делителя */
    absx.value -= ldz.value * ly; /* вычитаем частное, умноженное на младшую половину делителя */
#endif /* __iset__ < 6 */
    absx.value *= 2; /* восстановление делимого */
    if (absx.value > half_y) { /* возможно нужна коррекция */
        absx.value -= absy.value;
        if (absx.value >= half_y)
            absx.value -= absy.value;
    }
    absx.llong += ret.llong; /* формируем знак результата */
    return absx.value;
}

strong_alias (__remainder, __remainder_finite)
weak_alias (__remainder, remainder)
weak_alias (__remainder, __ieee754_remainder)
strong_alias (__remainder, __drem)
weak_alias (__remainder, drem)
#ifdef NO_LONG_DOUBLE
strong_alias (__remainder, __remainderl)
weak_alias (__remainder, remainderl)
strong_alias (__remainder, __dreml)
weak_alias (__remainder, dreml)
#endif
