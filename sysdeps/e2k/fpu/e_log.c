/* Copyright (c) 2016-2019 AO MCST. All rights reserved.
 * Distributed under the terms of MIT License.
 */

#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%  ПРОГРАММА  ВЫЧИСЛЯЕТ  НАТУРАЛЬНЫЙ  ЛОГАРИФМ  ЧИСЛА Х. %
%  НЕДОПУСТИМЫМ  ЗНАЧЕНИЕМ  АРГУМЕНТА  ЯВЛЯЕТСЯ  Х <= 0. %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

DB __log (DB X)

#define KA9 2.226555769734402668e-01
#define KA7 2.857139299728113646e-01
#define KA5 4.000000001150663675e-01
#define KA3 6.666666666666550833e-01
#define LKA9 0x3fcc7ffa5aafbc63LL
#define LKA7 0x3fd24923144fabc5LL
#define LKA5 0x3fd9999999b93aadLL
#define LKA3 0x3fe55555555554edLL

#define K2 4.00006488775094945494e-01
#define K1 6.66666666629825839191e-01
{
    _type_double_bits arg, y0;
    DB y, yd, ydv2;
    LL exp, i0;
#if __iset__ >= 6
    _type_double_bits ly, lyv2, lyv4, lyv8, lres, lres1;
#endif /* __iset__ < 6 */

    arg.value = X;
    exp = exponentabs (arg);
    if (X != X || exp == 0x7ff)
        return X + X;         /* X == NaN или +Inf */
    if (X <= 0.0)
        return SVID_libm_err (X, X, 16 + (X < 0.0));
    y = X - 1.0;
    y0.value = y;
    if ((y0.llong & ~0x8000000000000000LL) <= 0x3faeb85100000000LL) { /* |x-1|<=0.06 */
        yd = y / (X + 1.0);
        ydv2 = yd * yd;
#if __iset__ >= 6
        lyv2.value = ydv2;
        lyv4.value = ydv2 * ydv2;
        lyv8.value = yd * ydv2;
        lres.value = yd * y;
        lres1.llong = __builtin_e2k_fmad (LKA5, lyv2.llong, LKA3);
        lres.llong = __builtin_e2k_fmsd (lres1.llong, lyv8.llong, lres.llong);
        lyv8.value *= lyv4.value;
        lres1.llong = __builtin_e2k_fmad (LKA9, lyv2.llong, LKA7);
        lres.llong = __builtin_e2k_fmad (lres1.llong, lyv8.llong, lres.llong);
        return lres.value + y;
#else /* __iset__ < 6 */
        return (KA5 * yd * ydv2 * (KA3 / KA5 + ydv2) - yd * y +
                KA9 * yd * ydv2 * (ydv2 * ydv2 * (KA7 / KA9 + ydv2)) + y);
#endif /* __iset__ < 6 */
    }
    if (exp)
        exp -= 1023;
    else {
        arg.value = X * DVAIN52;
        exp = exponentabs (arg) - (1023 + 52);
    }
    i0 = arg.llong & 0xfe00000000000LL;
    y0.llong = i0 | 0x3ff0000000000000LL;
    arg.llong &= 0xfffffffffffffLL;
    arg.llong |= 0x3ff0000000000000LL;
    yd = (arg.value - y0.value) / (arg.value + y0.value);
    ydv2 = yd * yd;
    y0.llong = ((LL) ((unsigned *) constln)[i0 >>= 45]) << 32;
#if __iset__ >= 6
    lres.value = yd;
    ly.value = exp * KLN2_lo + constln_lo[i0];
    ly.llong = __builtin_e2k_fmad (/* 2.0 */ 0x4000000000000000LL, lres.llong, ly.llong);
    arg.value = K2 * yd * ydv2;
    lres.value = ydv2 + K1 / K2;
    lres.llong = __builtin_e2k_fmad (arg.llong, lres.llong, ly.llong);
    return lres.value + (y0.value + exp * KLN2_hi);
#else /* __iset__ < 6 */
    return (yd + yd + exp * KLN2_lo + constln_lo[i0] + K2 * yd * ydv2 * (ydv2 + K1 / K2) + (y0.value + exp * KLN2_hi));
#endif /* __iset__ < 6 */
}

strong_alias (__log, __log_finite)
weak_alias (__log, log)
weak_alias (__log, __ieee754_log)
