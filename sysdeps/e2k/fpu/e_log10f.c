/* Copyright (c) 2016-2018 ZAO "MCST". All rights reserved. */

#include "f2c.h"
#include <e2kintrin.h>

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%  πςοηςαννα  χωώισμρετ  δεσρτιώξωκ  μοηαςιζν  ώισμα  θ. %
%  ξεδοπυστινων  ϊξαώεξιεν  αςηυνεξτα  ρχμρετσρ  θ <= 0. %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define KA3 2.89540094138102480217e-01
#define KA1 8.68588963728085952434e-01
#define LKA3 0x3fd287d32ccd0ce3LL
#define LKA1 0x3febcb7b151c1dfaLL

#define KINVLN10 4.342944819032518167e-01
#define KLG2 3.010299956639811980e-01

FL __ieee754_log10f (FL X)
{
    _type_float_bits arg;
    _type_double_bits y0;
    DB y, yp;
#if __iset__ >= 6
    _type_double_bits ly, lyv2, lres, lres1;
#endif /* __iset__ < 6 */

    arg.value = X;
    yp = y0.value = (DB) X;
    if (X != X || arg.int0 >= 0x7f800000)    /* X = NaN ΙΜΙ +Inf */
        return X + X;
    if (X <= 0)
        return (FL) SVID_libm_err (yp, yp, 118 + (X < 0));

    y0.llong = __builtin_e2k_paddd (y0.llong, 0x400000000000LL);
    y0.llong = __builtin_e2k_pandd (y0.llong, 0x7fff800000000000LL);
    y = (yp - y0.value) / (yp + y0.value);
#if __iset__ >= 6
    ly.value = y;
    lyv2.value = y * y;
    lres.value = KLG2 * (DB) ((y0.llong >> 52) - 1023) + constnln[(y0.llong >> 47) & 0x1f] * KINVLN10;
    lres.llong = __builtin_e2k_fmad (ly.llong, LKA1, lres.llong);
    lres1.value = KA3 * y;
    lres.llong = __builtin_e2k_fmad (lres1.llong, lyv2.llong, lres.llong);
    return (FL) lres.value;
#else /* __iset__ < 6 */
    return (FL) (KA1 * y + (KLG2 * (DB) ((y0.llong >> 52) - 1023)
        + constnln[(y0.llong >> 47) & 0x1f] * KINVLN10) + KA3 * y * (y * y));
#endif /* __iset__ < 6 */
}

strong_alias (__ieee754_log10f, __log10f_finite)
weak_alias (__ieee754_log10f, __log10f)
weak_alias (__ieee754_log10f, log10f)
