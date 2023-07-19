/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

#include <errno.h>
#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%  πςοηςαννα  χωώισμρετ  ξατυςαμψξωκ μοηαςιζν ώισμα θ+1. %
%  ξεδοπυστινων  ϊξαώεξιεν  αςηυνεξτα  ρχμρετσρ θ <= -1. %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define KA11 1.8229926400626044328988e-01L
#define KA9  2.2222165873253707057082e-01L
#define KA7  2.8571428601840093011501e-01L
#define KA5  3.9999999999992582043596e-01L
#define KA3  6.6666666666666667328338e-01L

#define K3  2.8572101483985472322395e-01L
#define K2  3.9999999993632415899233e-01L
#define K1  6.6666666666666685948101e-01L

LD __log1pl (LD X)
{
    _type_long_double_bits arg, y0;
    _type_double_bits con;
    LD c, yd, ydv2, ydv4;
    LI exp, i0;

    arg.value = X;
    exp = arg.twolong.short0 & 0x7fff;
    if (exp == 0x7fff)
        return X + Inf;         /* X == NaN ΙΜΙ Inf */
    if (__glibc_unlikely (X <= -1)) {
        if (X < -1)
            __set_errno (ERANGE);
        else
            __set_errno (EDOM);
        return ((X < -1) ? X - X : X) / (X - X);
    }
    if (X <= 0.0625 && X >= -0.0625) {        /* |x| <= 0.0625 */
        yd = X / (X + 2);
	c = 0.5 * X * X;
        ydv2 = yd * yd;
        ydv4 = ydv2 * ydv2;
        return (KA5 * yd * ydv2 * (KA3 / KA5 + ydv2) + yd * c + KA11 * yd *
                ydv2 * ydv4 * (KA7 / KA11 + KA9 / KA11 * ydv2 + ydv4) - c + X);
    }
    if (exp < 0x403f) { /* X < 2^64 */
        arg.value = X + 1;
        exp = arg.twolong.short0 & 0x7fff;
    }
    c = (exp > 16383) ? 1 - (arg.value - X) : X - (arg.value - 1);
    c /= arg.value;
    exp -= 16383;

    arg.twolong.short0 = 0x3fff;
    y0.twolong.short0 = 0x3fff;
    y0.twolong.long1 = arg.twolong.long1 & 0xff00000000000000LL;
    i0 = (arg.twolong.long1 >> 56) & 0x7f;

    yd = (arg.value - y0.value) / (arg.value + y0.value);
    ydv2 = yd * yd;
    con.llong = ((LL) constln[i0]) << 32;
    return (yd + yd + (exp * KLN2_lo + c) + constln_lo[i0] + K3 * yd * ydv2 *
        (ydv2 * ydv2 + K2 / K3 * ydv2 + K1 / K3) + (con.value + (LD)exp * KLN2_hi));
}

weak_alias (__log1pl, log1pl)
