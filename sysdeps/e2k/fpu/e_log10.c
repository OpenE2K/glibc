/* Copyright (c) 2016-2018 ZAO "MCST". All rights reserved. */

#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%  πςοηςαννα  χωώισμρετ  δεσρτιώξωκ  μοηαςιζν  ώισμα  θ. %
%  ξεδοπυστινων  ϊξαώεξιεν  αςηυνεξτα  ρχμρετσρ  θ <= 0. %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

DB __ieee754_log10 (DB X)

#define KA9 9.66989590060015292639e-02
#define KA7 1.24083979371931111739e-01
#define KA5 1.73717792815394082883e-01
#define KA3 2.89529654602161416805e-01
#define LKA9 0x3fb8c143527ceef6LL
#define LKA7 0x3fbfc3f7b95c27d4LL
#define LKA5 0x3fc63c6277700dcfLL
#define LKA3 0x3fd287a7636f42eaLL

#define K2 4.00001628508120317429e-01
#define K1 6.66666666664346138331e-01
#define KINVLN10_hi 4.34295654296875e-01        /* 3fdbcb8...0 */
#define KINVLN10_lo -1.172393623172348803e-06
#define KINVLN10 4.34294481903251816668e-1
#define LKINVLN10 0x3fdbcb7b1526e50eLL
#define KLG2_hi 3.01029995663611771306e-1
#define KLG2_lo 3.69423907715893078616e-13
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
        return X + X;         /* X == NaN ΙΜΙ +Inf */
    if (X <= 0.0)
        return SVID_libm_err (X, X, 18 + (X < 0.0));
    y = X - 1.0;
    y0.value = y;
    if ((y0.llong & ~0x8000000000000000LL) <= 0x3faeb85100000000LL) { /* |x-1|<=0.06 */
        y0.llong &= ~0xffffffffLL;
        yd = y / (X + 1.0);
        ydv2 = yd * yd;
#if __iset__ >= 6
        lyv2.value = ydv2;
        lyv4.value = ydv2 * ydv2;
        lyv8.value = yd * ydv2;
        lres.value = KINVLN10 * (y - y0.value) + KINVLN10_lo * y0.value - KINVLN10 * y * yd;
        lres1.llong = __builtin_e2k_fmad (LKA5, lyv2.llong, LKA3);
        lres.llong = __builtin_e2k_fmad (lres1.llong, lyv8.llong, lres.llong);
        lyv8.value *= lyv4.value;
        lres1.llong = __builtin_e2k_fmad (LKA9, lyv2.llong, LKA7);
        lres.llong = __builtin_e2k_fmad (lres1.llong, lyv8.llong, lres.llong);
        return lres.value + y0.value * KINVLN10_hi;
#else /* __iset__ < 6 */
//        return KINVLN10 * (y - y0.value - y * yd) + KINVLN10_lo * y0.value
        return KINVLN10 * (y - y0.value) + KINVLN10_lo * y0.value - KINVLN10 * y * yd
            + KA5 * yd * ydv2 * (KA3 / KA5 + ydv2) + KA9 * yd * ydv2 * (ydv2
            * ydv2 * (KA7 / KA9 + ydv2)) + y0.value * KINVLN10_hi;
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
    ly.value = KINVLN10 * constln_lo[i0];
    lres.llong = __builtin_e2k_fmad (/* 2*KINVLN10 */ 0x3febcb7b1526e50eLL, lres.llong, ly.llong);
    ly.value = KINVLN10_lo * y0.value + exp * KLG2_lo + lres.value;
    arg.value = K2 * KINVLN10 * yd * ydv2;
    lres.value = ydv2 + K1 / K2;
    lres.llong = __builtin_e2k_fmad (arg.llong, lres.llong, ly.llong);
    return lres.value + (y0.value * KINVLN10_hi + exp * KLG2_hi);
#else /* __iset__ < 6 */
    return KINVLN10_lo * y0.value + exp * KLG2_lo + KINVLN10 * (yd + yd + constln_lo[i0]) +
        K2 * KINVLN10 * yd * ydv2 * (ydv2 + K1 / K2) + (y0.value * KINVLN10_hi + exp * KLG2_hi);
#endif /* __iset__ < 6 */
}

strong_alias (__ieee754_log10, __log10_finite)
weak_alias (__ieee754_log10, __log10)
weak_alias (__ieee754_log10, log10)
#ifdef NO_LONG_DOUBLE
strong_alias (__ieee754_log10, __log10l)
weak_alias (__ieee754_log10, log10l)
#endif
