/* Copyright (c) 2016-2019 ZAO "MCST". All rights reserved. */

#include "f2c.h"
#include <e2kintrin.h>

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%  ПРОГРАММА  ВЫЧИСЛЯЕТ  НАТУРАЛЬНЫЙ  ЛОГАРИФМ  ЧИСЛА Х. %
%  НЕДОПУСТИМЫМ  ЗНАЧЕНИЕМ  АРГУМЕНТА  ЯВЛЯЕТСЯ  Х <= 0. %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define LN2 0.6931471805599453094
#define KA3 6.66690704586487441045e-01

FL __logf (FL X)
{
    _type_float_bits arg;
    _type_double_bits y0;
    DB y, yp;
#if __iset__ >= 6
    _type_double_bits ly, lyv2, lres, lres1;
#endif /* __iset__ < 6 */

    arg.value = X;
    yp = y0.value = (DB) X;
    if (X != X || arg.int0 >= 0x7f800000)    /* X = NaN или +Inf */
        return X + X;
    if (X <= 0)
        return (FL) SVID_libm_err (yp, yp, 116 + (X < 0));

    y0.llong = __builtin_e2k_paddd (y0.llong, 0x400000000000LL);
    y0.llong = __builtin_e2k_pandd (y0.llong, 0x7fff800000000000LL);
    y = (yp - y0.value) / (yp + y0.value);
#if __iset__ >= 6
    ly.value = y;
    lyv2.value = y * y;
    lres.value = LN2 * (DB) ((y0.llong >> 52) - 1023) + constnln[(y0.llong >> 47) & 0x1f];
    lres.llong = __builtin_e2k_fmad (ly.llong, 0x4000000000000000LL, lres.llong);
    lres1.value = KA3 * y;
    lres.llong = __builtin_e2k_fmad (lres1.llong, lyv2.llong, lres.llong);
    return (FL) lres.value;
#else /* __iset__ < 6 */
    return (FL) (y + y + (LN2 * (DB) ((y0.llong >> 52) - 1023)
        + constnln[(y0.llong >> 47) & 0x1f]) +  KA3 * y * (y * y));
#endif /* __iset__ < 6 */
}

strong_alias (__logf, __logf_finite)
weak_alias (__logf, logf)
weak_alias (__logf, __ieee754_logf)
