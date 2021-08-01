/* Copyright (c) 2016-2018 ZAO "MCST". All rights reserved. */

#include <errno.h>
#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%  πςοηςαννα  χωώισμρετ  ξατυςαμψξωκ μοηαςιζν ώισμα θ+1. %
%  ξεδοπυστινων  ϊξαώεξιεν  αςηυνεξτα  ρχμρετσρ θ <= -1. %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

DB __log1p (DB X)

#define KA9 2.22570439144317389386e-01
#define KA7 2.85714077467483184054e-01
#define KA5 4.00000000039844345850e-01
#define KA3 6.66666666666665475626e-01
#define LKA9 0x3fcc7d302a97319cLL
#define LKA7 0x3fd24923b2aeb299LL
#define LKA5 0x3fd9999999a48d66LL
#define LKA3 0x3fe555555555554bLL

#define K2 4.00006488775094945494e-01
#define K1 6.66666666629825839191e-01
{
    _type_double_bits arg, y0;
    DB yd, ydv2, c;
    LL exp, i0;
#if __iset__ >= 6
    _type_double_bits ly, lyv2, lyv4, lyv8, lres, lres1;
#endif /* __iset__ < 6 */

    arg.value = dfabs (X);
    if (arg.llong >= 0x7ff0000000000000LL)
        return X + Inf;         /* X == NaN ΙΜΙ Inf */
    if (__glibc_unlikely (X <= -1)) {
        __set_errno ((X < -1) ? ERANGE : EDOM);
        return ((X < -1) ? X - X : X) / (X - X);
    }
    if (arg.llong <= 0x3faeb85100000000LL) {         /* |X| <= 0.06 */
        yd = X / (X + 2.0);
        ydv2 = yd * yd;
        c = 0.5 * (X * X);
#if __iset__ >= 6
        lyv2.value = ydv2;
        lyv4.value = ydv2 * ydv2;
        lyv8.value = yd * ydv2;
//        lres.value = yd;
//        lres1.value = c;
//        lres.llong = __builtin_e2k_fmsd (lres.llong, lres1.llong, lres1.llong);
        lres.value = yd * c - c;
        lres1.llong = __builtin_e2k_fmad (LKA5, lyv2.llong, LKA3);
        lres.llong = __builtin_e2k_fmad (lres1.llong, lyv8.llong, lres.llong);
        lyv8.value *= lyv4.value;
        lres1.llong = __builtin_e2k_fmad (LKA9, lyv2.llong, LKA7);
        lres.llong = __builtin_e2k_fmad (lres1.llong, lyv8.llong, lres.llong);
        return lres.value + X;
#else /* __iset__ < 6 */
        return (KA5 * yd * ydv2 * (KA3 / KA5 + ydv2) + (yd * c - c) +
                KA9 * yd * ydv2 * (ydv2 * ydv2 * (KA7 / KA9 + ydv2)) + X);
#endif /* __iset__ < 6 */
    }
    arg.value = X + 1;
    exp = arg.llong >> 52;
    c = (exp > 1023) ? 1.0 - (arg.value - X) : X - (arg.value - 1.0);
    c /= arg.value;
    exp -= 1023;
    i0 = arg.llong & 0xfe00000000000LL;
    y0.llong = i0 | 0x3ff0000000000000LL;
    arg.llong &= 0xfffffffffffffLL;
    arg.llong |= 0x3ff0000000000000LL;
    yd = (arg.value - y0.value) / (arg.value + y0.value);
    ydv2 = yd * yd;
    y0.llong = ((LL) ((unsigned *) constln)[i0 >>= 45]) << 32;
#if __iset__ >= 6
    lres.value = yd;
    ly.value = exp * KLN2_lo + constln_lo[i0] + c;
    ly.llong = __builtin_e2k_fmad (/* 2.0 */ 0x4000000000000000LL, lres.llong, ly.llong);
    arg.value = K2 * yd * ydv2;
    lres.value = ydv2 + K1 / K2;
    lres.llong = __builtin_e2k_fmad (arg.llong, lres.llong, ly.llong);
    return lres.value + (y0.value + exp * KLN2_hi);
#else /* __iset__ < 6 */
    return (yd + yd + (exp * KLN2_lo + constln_lo[i0] + c) +
            K2 * yd * ydv2 * (ydv2 + K1 / K2) + (y0.value + exp * KLN2_hi));
#endif /* __iset__ < 6 */
}

weak_alias (__log1p, log1p)
#ifdef NO_LONG_DOUBLE
strong_alias (__log1p, __log1pl)
weak_alias (__log1p, log1pl)
#endif
