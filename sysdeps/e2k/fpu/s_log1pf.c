/* Copyright (c) 2016-2018 ZAO "MCST". All rights reserved. */

#include <errno.h>
#include "f2c.h"
#include <e2kintrin.h>

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%  πςοηςαννα  χωώισμρετ  ξατυςαμψξωκ μοηαςιζν ώισμα θ+1. %
%  ξεδοπυστινων  ϊξαώεξιεν  αςηυνεξτα  ρχμρετσρ θ <= -1. %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define KB5 4.00297520051000810194e-01f
#define KB3 6.66666627963472760429e-01f
#define IKB5 0x3eccf3cc
#define IKB3 0x3f2aaaaa

#define LN2 0.6931471805599453094
#define KA3 6.66690704586487441045e-01

FL __log1pf (FL X)
{
    _type_double_bits y0;
    _type_float_bits arg;
    FL yd, ydv2, c;
    DB y, yp;
#if __iset__ >= 6
    _type_double_bits ly, lyv2, lres, lres1;
    _type_float_bits iy, iyv2, iyv3, ires, ires1;
#endif /* __iset__ < 6 */

    arg.value = ffabs (X);

    if (arg.int0 >= 0x7f800000)
        return X + Inf_F;         /* X == NaN ΙΜΙ Inf */
    if (__glibc_unlikely (X <= -1)) {
        __set_errno (X < -1 ? ERANGE : EDOM);
        return ((X < -1) ? X - X : X) / (X - X);
    }
    if (arg.int0 <= 0x3d800000) {        /* |X| <= 0.0625 */
        yd = X / (X + 2);
        ydv2 = yd * yd;
        c = 0.5f * X * X;
#if __iset__ >= 6
        iy.value = yd;
        iyv2.value = ydv2;
        iyv3.value = yd * ydv2;
        ires.value = c;
        ires.int0 = __builtin_e2k_fmss (iy.int0, ires.int0, ires.int0);
        ires1.int0 = __builtin_e2k_fmas (IKB5, iyv2.int0, IKB3);
        ires.int0 = __builtin_e2k_fmas (ires1.int0, iyv3.int0, ires.int0);
        return ires.value + X;
#else /* __iset__ < 6 */
        return KB5 * yd * ydv2 * (KB3 / KB5 + ydv2) + (yd * c - c) + X;
#endif /* __iset__ < 6 */
    }
    yp = y0.value = 1.0 + (DB) X;
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

weak_alias (__log1pf, log1pf)
