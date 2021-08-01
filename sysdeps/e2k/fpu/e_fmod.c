/* Copyright (c) 2016-2018 ZAO "MCST". All rights reserved. */

#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%    ПРОГРАММА  ВЫЧИСЛЯЕТ  ВЕЩЕСТВЕННЫЙ  ОСТАТОК  ЧИСЛА  %
%  X  ПО  МОДУЛЮ  Y. НЕДОПУСТИМЫМ  ЗНАЧЕНИЕМ  АРГУМЕНТА  %
%  ЯВЛЯЕТСЯ  Y = 0  ИЛИ  |X| = Inf.                      %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define TRUNC (1.38777878078144567553e-16) /* 1.25 / 2 ^ 53 */

#if __iset__ >= 6
#define THRESH (31ULL << 52)
#else /* __iset__ < 6 */
#define THRESH (26ULL << 52)
#endif /* __iset__ < 6 */

DB __fmod (DB X, DB Y)
{
    _type_double_bits absx, absy, ldz, ret;
    DB recipy, tmp;
    ULL ix, iy;

    absx.value = dfabs (X);
    absy.value = dfabs (Y);
    ret.value = X;
    ret.llong &= 0x8000000000000000LL;

    if (absx.value < absy.value)
        return X;

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
        return SVID_libm_err (X, Y, 27);

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
        if (absx.value < 0) { /* возможно нужна коррекция */
            absx.value += absy.value;
        }
        absx.llong += ret.llong; /* формируем знак результата */
        return absx.value;
    }

    if (unlikely (!(iy >= (1ULL << 52)))) { /* Y - денормализованное */
        absy.value *= DVAIN52;
        absx.value = __fmod (X, absy.value);
        return __fmod (absx.value * DVAIN52, absy.value) * (1 / DVAIN52);
    }
    if (unlikely (ix > ((1022ULL << 52) + iy))) { /* частное X/Y может переполниться */
        absx.value = __fmod (absx.value, absy.value * DVAIN1022); /* 2^1022 */
        ix = absx.llong;
    }

#pragma loop count (10)
    while (ix > (THRESH + iy)) {
        ldz.value = absx.value * recipy; /* получаем частное */
// похоже, что это не нужно делать, мы и так обрезаем мантиссу
//        ldz.value -= ldz.value * TRUNC; /* уменьшаем на 1 ulp */
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
    if (absx.value < 0) { /* возможно нужна коррекция */
        absx.value += absy.value;
    }
    else if (absx.value == absy.value) {
        absx.value = 0;
    }
    absx.llong += ret.llong; /* формируем знак результата */
    return absx.value;
}

strong_alias (__fmod, __fmod_finite)
weak_alias (__fmod, fmod)
weak_alias (__fmod, __ieee754_fmod)
