/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     πςοηςαννα  χωώισμρετ  δχοιώξωκ  μοηαςιζν  ώισμα θ. %
%  ξεδοπυστινων  ϊξαώεξιεν  αςηυνεξτα  ρχμρετσρ  θ <= 0. %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define KA11 2.6300224413953993748621e-01L
#define KA9  3.2059808503155084101674e-01L
#define KA7  4.1219858354987791583887e-01L
#define KA5  5.7707801635547834445479e-01L
#define KA3  9.6179669392597561445251e-01L
#define KA1_lo 1.67517131648865110689e-10L

#define K7  4.1220829118722033061699e-01L
#define K5  5.7707801626372054289775e-01L
#define K3  9.6179669392597588307892e-01L
#define KINVLN2_hi 1.44269504072144627571 /* b8aa3b2900000000 3fff */
//#define K1 (KINVLN2_hi+KA1_lo) /* bug 70148 */
#define K1  1.4426950408889634073877L

LD __log2l (LD X)
{
    _type_long_double_bits arg, y0;
    _type_double_bits con;
    LD y, yd, ydv2, ydv4;
    LI exp, i0;

    arg.value = X;
    exp = arg.twolong.short0 & 0x7fff;
    if (exp == 0x7fff)
        return X + Inf;         /* X == NaN ΙΜΙ Inf */
    if (X <= 0)
        return SVID_libm_err (X, X, 248 + (X < 0.0));
    if (X <= 1.0625 && X >= 0.9375) {        /* |x - 1| <= 0.0625 */
        y0.value = y = X - 1;
        y0.twolong.long1 &= 0xffffffff00000000ULL;
        yd = y / (X + 1);
        ydv2 = yd * yd;
        ydv4 = ydv2 * ydv2;
        return K1 * (y - y0.value - y * yd) + KA1_lo * y0.value + KA5 * yd *
            ydv2 * (KA3 / KA5 + ydv2) + KA11 * yd * ydv2 * ydv4 * (KA7 / KA11 +
            KA9 / KA11 * ydv2 + ydv4) + y0.value * KINVLN2_hi;
    }
    if (exp)
        exp -= 16383;
    else {
        arg.value = X * DVAIN63;
        exp = (arg.twolong.short0 & 0x7fff) - (16383 + 63);
    }
    arg.twolong.short0 = 0x3fff;
    y0.twolong.short0 = 0x3fff;
    y0.twolong.long1 = arg.twolong.long1 & 0xff00000000000000LL;
    i0 = (arg.twolong.long1 >> 56) & 0x7f;

    yd = (arg.value - y0.value) / (arg.value + y0.value);
    ydv2 = yd * yd;
    con.llong = ((LL) constln[i0]) << 32;
    return K1 * (yd + yd + constln_lo[i0]) + KA1_lo * con.value +
       K7 * yd * ydv2 * (ydv2 * ydv2 + K5 / K7 * ydv2 + K3 / K7) +
       (con.value * KINVLN2_hi + (LD) exp);
}

strong_alias (__log2l, __log2l_finite)
weak_alias (__log2l, log2l)
weak_alias (__log2l, __ieee754_log2l)
