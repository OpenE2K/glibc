/* Copyright (c) 2016-2019 ZAO "MCST". All rights reserved. */

#include "f2c.h"
#include <e2kintrin.h>

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ПРОГРАММА  ВЫЧИСЛЯЕТ  ЗНАЧЕНИЕ  ФУНКЦИИ            %
%  F(Х) = ЕХР (Х).  НЕДОПУСТИМЫМ   ЗНАЧЕНИЕМ   АРГУМЕН-  %
%  ТА  ЯВЛЯЕТСЯ  Х > 709.7827128933839731 ИЛИ            %
%  X < -745.1332191019412221 .                           %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

DB __ieee754_exp (DB X)

#define KA5 8.33333478792298551112e-03
#define KA4 4.16666768487944911020e-02
#define KA3 1.66666666666665333693e-01
#define KA2 4.99999999999990669185e-01
#define LKA5 0x3f811111430bc5c6LL
#define LKA4 0x3fa55555accc11b8LL
#define LKA3 0x3fc5555555555525LL
#define LKA2 0x3fdfffffffffff58LL
{
    _type_double_bits arg;
    DB y, yv2, hi;
    long k, ind;
#if __iset__ >= 6
    _type_double_bits ly, lyv2, lyv4, lres, lres1;
#endif /* __iset__ < 6 */

    arg.value = X;
    arg.llong &= 0x7fffffffffffffffLL;

    if (arg.llong >= 0x7ff0000000000000LL) {    /* X == NaN или Inf */
        if (X != X)
            X = X - X;
        else if (X < 0.0)
            X = 0.0;            /* NaN */
        return X;
    }
    y = X * K256ONLN2;
    if (X > DB_MAX_LOG || X < DB_MIN_LOG)
        return SVID_libm_err (X, 0.0, X > DB_MAX_LOG ? 6 : 7);
    k = (int) y;
    ind = k & 255;
#if __iset__ <= 2
    hi = (DB) k;
#else /*  __iset__ <= 2 */
#pragma asm_inline
    __asm ("fdtoifd 0x3,%1,%0" : "=r" (hi): "r" (y));
#endif /*  __iset__ <= 2 */
#if __iset__ >= 6
    lres.value = X;
    ly.value = hi;
    lres.llong = __builtin_e2k_fnmad (LKLN2ON256_hi, ly.llong, lres.llong);
    y = lres.value - hi * KLN2ON256_lo;
#else /* __iset__ < 6 */
    y = X - hi * KLN2ON256_hi - hi * KLN2ON256_lo;
#endif /* __iset__ < 6 */
    yv2 = y * y;
#if __iset__ >= 6
    ly.value = y;
    lyv2.value = yv2;
    lyv4.value = yv2 * yv2;
    lres.llong = __builtin_e2k_fmad (LKA3, ly.llong, LKA2);
    lres1.llong = __builtin_e2k_fmad (lres.llong, lyv2.llong, ly.llong);
    lres.llong = __builtin_e2k_fmad (LKA5, ly.llong, LKA4);
    lres.llong = __builtin_e2k_fmad (lres.llong, lyv4.llong, lres1.llong);
    y = lres.value;
#else /* __iset__ < 6 */
    y = KA2 * y * (y + 1 / KA2) + KA3 * y * yv2 + yv2 * yv2 * (KA5 * y + KA4);
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
    if (k < -1021)
        ind = k + 54;
    else
        ind = k;
    arg.llong = __builtin_e2k_paddd (arg.llong, ((LL) ind) << 52);
    if (k < -1021)
        return arg.value * K2IN54;
    else
        return arg.value; /* 55 такт */
}

strong_alias (__ieee754_exp, __exp_finite)
weak_alias (__ieee754_exp, exp)
