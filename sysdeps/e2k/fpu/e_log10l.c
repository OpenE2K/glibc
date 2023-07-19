/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%  πςοηςαννα  χωώισμρετ  δεσρτιώξωκ  μοηαςιζν  ώισμα  θ. %
%  ξεδοπυστινων  ϊξαώεξιεν  αςηυνεξτα  ρχμρετσρ  θ <= 0. %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define KA11 7.9171564412943002119859e-02L
#define KA9  9.6509640146928424280950e-02L
#define KA7  1.2408413781871893942635e-01L
#define KA5  1.7371779276126851528512e-01L
#define KA3  2.8952965460216788797435e-01L

#define K3  2.8572101483985472322395e-01L
#define K2  3.9999999993632415899233e-01L
#define K1  6.6666666666666685948101e-01L
#define KINVLN10_hi 4.34294481878168880939e-1 /* de5bd8a900000000 3ffd */
#define KINVLN10_lo 2.50829467116452763385e-11L
//#define KINVLN10 (KINVLN10_hi+KINVLN10_lo) /* bug 70148 */
#define KINVLN10 4.34294481903251827651e-1L
#define KLG2_hi 3.01029995665885508060e-01 /* 9a209a8500000000 3ffd */
#define KLG2_lo -1.90431284671642754113e-12L

LD __log10l (LD X)
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
        return SVID_libm_err (X, X, 218 + (X < 0.0));
    if (X <= 1.0625 && X >= 0.9375) {        /* |x - 1| <= 0.0625 */
        y0.value = y = X - 1;
        y0.twolong.long1 &= 0xffffffff00000000ULL;
        yd = y / (X + 1);
        ydv2 = yd * yd;
        ydv4 = ydv2 * ydv2;
        return KINVLN10 * (y - y0.value - y * yd) + KINVLN10_lo * y0.value + KA5 * yd *
            ydv2 * (KA3 / KA5 + ydv2) + KA11 * yd * ydv2 * ydv4 * (KA7 / KA11 +
            KA9 / KA11 * ydv2 + ydv4) + y0.value * KINVLN10_hi;
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
    return KINVLN10_lo * con.value + (DB) exp * KLG2_lo + KINVLN10 *
        (yd + yd + constln_lo[i0]) + KINVLN10 * K3 * yd * ydv2 * (ydv2 * ydv2 +
        K2 / K3 * ydv2 + K1 / K3) + (con.value * KINVLN10_hi + (LD) ((DB) exp * KLG2_hi));
}

strong_alias (__log10l, __log10l_finite)
weak_alias (__log10l, log10l)
weak_alias (__log10l, __ieee754_log10l)
