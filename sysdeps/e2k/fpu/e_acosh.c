/* Copyright (c) 2016-2018 ZAO "MCST". All rights reserved. */

#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     πςοηςαννα  χωώισμρετ  ϊξαώεξιε  ζυξλγιι  'αςλλο-   %
%  σιξυσ  ηιπεςβομιώεσλικ'.  ξεδοπυστινων   ϊξαώεξιεν    %
%  αςηυνεξτα  ρχμρετσρ  θ < 1.                           %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

DB __ieee754_acosh (DB X)

#define KA5 -7.25717260606618343970e-04
#define KA4 6.82096672305299459161e-02
#define KA3 -6.24444324574107935833e-01
#define KA2 1.90758595937067854277e+00
#define KA1 -2.34323780096859411145e+00
#define KE5 5.37747703686733154555e-02
#define KE4 -9.21634985001381204840e-01
#define KE3 4.85734183288500847349e+00
#define KE2 -1.08743672056013785866e+01
#define KE1 1.08729512038743684012e+01
#define LKA5 0xbf47c7c1f341e26eLL
#define LKA4 0x3fb176305206b303LL
#define LKA3 0xbfe3fb72aa06fecbLL
#define LKA2 0x3ffe8578dadce4c2LL
#define LKA1 0xc002bef375cf49deLL
#define LKE5 0x3fab885de02aae64LL
#define LKE4 0xbfed7e08a6edc5b2LL
#define LKE3 0x40136deb0476ef6cLL
#define LKE2 0xc025bfad0ef18173LL
#define LKE1 0x4025bef375cf49daLL

#define KP4 -7.25156777376967427495e-01
#define KP3 -5.40048701038245640330e+01
#define KP2 -1.00476881303038304715e+03
#define KP1 -6.59613774948346034902e+03
#define KP0 -1.39590972186814507010e+04
#define KQ4 1.18364821531370943950e+02
#define KQ3 3.53165197272157492365e+03
#define KQ2 4.05555333697523893945e+04
#define KQ1 1.95996868478041511257e+05
#define KQ0 3.35018333248354826801e+05
#define LKP4 0xbfe7347bfc69d188LL
#define LKP3 0xc04b009f956453cbLL
#define LKP2 0xc08f6626877231dfLL
#define LKP1 0xc0b9c423438cd681LL
#define LKP0 0xc0cb438c71a968b2LL
#define LKQ4 0x405d97593c68875aLL
#define LKQ3 0x40ab974dcf5e5a19LL
#define LKQ2 0x40e3cd71115d7166LL
#define LKQ1 0x4107ece6f2a49d8dLL
#define LKQ0 0x411472a9553f0e86LL

#define K2 4.00006488775094945494e-01
#define K1 6.66666666629825839191e-01
{
    _type_double_bits arg, y0;
    DB y, yv2, yv4, dob;
    LL exp, i0;
#if __iset__ >= 6
    _type_double_bits ly, lyv2, lyv4, ldob, lres, lres1, lres2, lres3;
#endif /* __iset__ < 6 */

    arg.value = X;
    if (arg.llong < 0x3ff0000000000000LL && arg.llong <= 0xfff0000000000000LL) /* X < 1.0 && X >= -inf */
        return SVID_libm_err (X, X, 29);   /* acosh(x<1) */
    if ((arg.llong & 0x7fffffffffffffffLL) >= 0x7ff0000000000000LL) /* X == +-NaN ΙΜΙ +Inf */
        return X + X;
    if (arg.llong <= 0x3ffc000000000000LL) {       /* X <= 1.75 */
        arg.value = X - 1;
        if (arg.llong == 0)
            return arg.value;
        arg.llong += 0x10000000000000LL;   /*  2*arg  */
        y = arg.value;
        y0.value = dsqrt (arg.value);
        yv2 = arg.value * arg.value;
        yv4 = yv2 * yv2;
        arg.value = y0.value;
        y0.llong = __builtin_e2k_pandnd (y0.llong, 0x00000000ffffffffLL);
#if __iset__ >= 6
        ldob.value = y;
        lyv2.value = yv2;
        lyv4.value = yv4;
        lres.value = KP0 * y;
        lres1.llong = __builtin_e2k_fmad (LKP2, ldob.llong, LKP1);
        lres.llong = __builtin_e2k_fmad (lres1.llong, lyv2.llong, lres.llong);
        lres1.llong = __builtin_e2k_fmad (LKP4, ldob.llong, LKP3);
        lres2.llong = __builtin_e2k_fmad (lres1.llong, lyv4.llong, lres.llong);

        lres.llong = __builtin_e2k_fmad (LKQ1, ldob.llong, LKQ0);
        lres1.llong = __builtin_e2k_fmad (LKQ3, ldob.llong, LKQ2);
        lres.llong = __builtin_e2k_fmad (lres1.llong, lyv2.llong, lres.llong);
        lres1.value = y + KQ4;
        lres1.llong = __builtin_e2k_fmad (lres1.llong, lyv4.llong, lres.llong);
        lres.value = lres2.value / lres1.value;

        ldob.llong = __builtin_e2k_fnmad (y0.llong, y0.llong, ldob.llong);
        ldob.value = ldob.value / (arg.value + y0.value);
        ldob.llong = __builtin_e2k_fmad (lres.llong, arg.llong, ldob.llong);
        return ldob.value + y0.value;
#else /* __iset__ < 6 */
        dob = (y - y0.value * y0.value) / (arg.value + y0.value);
        return (KP0 * y + yv2 * (KP1 + KP2 * y) + (KP3 + KP4 * y) * yv4) / (KQ0 + KQ1 * y
               + yv2 * (KQ2 + KQ3 * y) + (y + KQ4) * yv4) * arg.value + dob + y0.value;
#endif /* __iset__ < 6 */
    }                           /*  log(2|x|)+P5(x^2)/Q5(x^2) */
    if (arg.llong < 0x4197d78400000000LL) {        /* X < 1e8 */
        y = X * X;
        yv2 = y * y;
        yv4 = yv2 * yv2;
#if __iset__ >= 6
        ly.value = y;
        lyv2.value = yv2;
        lyv4.value = yv4;
        lres.llong = __builtin_e2k_fmad (LKA4, ly.llong, LKA5);
        lres1.llong = __builtin_e2k_fmad (LKA2, ly.llong, LKA3);
        lres2.llong = __builtin_e2k_fmad (lres1.llong, lyv2.llong, lres.llong);
        lres.value = y + KA1;
        lres2.llong = __builtin_e2k_fmad (lres.llong, lyv4.llong, lres2.llong);

        lres.value = KE5 * y;
        lres1.llong = __builtin_e2k_fmad (LKE3, ly.llong, LKE4);
        lres3.llong = __builtin_e2k_fmad (lres1.llong, lyv2.llong, lres.llong);
        lres.llong = __builtin_e2k_fmad (0xc010000000000000LL, ly.llong, LKE1);
        lres1.llong = __builtin_e2k_fmad (lres.llong, ly.llong, LKE2);
        lres1.llong = __builtin_e2k_fmad (lres1.llong, lyv4.llong, lres3.llong);
        lres.value = lres2.value / lres1.value;
        dob = lres.value;
#else /* __iset__ < 6 */
        dob = (KA5 + KA4 * y + KA2 * yv2 * (KA3 / KA2 + y) + yv4 * (KA1 + y)) / (KE5 * y
            + KE3 * yv2 * (KE4 / KE3 + y) + yv4 * (KE2 + KE1 * y - 4.0 * yv2));
#endif /* __iset__ < 6 */
    }
    else {
        y = 0.5 / X;
        dob = -y * y;
    }
    exp = exponent (arg) - 1022;
    i0 = arg.llong & 0xfe00000000000LL;
    y0.llong = i0 | 0x3ff0000000000000LL;
    arg.llong &= 0xfffffffffffffLL;
    arg.llong |= 0x3ff0000000000000LL;
    y = (arg.value - y0.value) / (arg.value + y0.value);
    yv2 = y * y;
    y0.llong = ((LL) ((unsigned *) constln)[i0 >>= 45]) << 32;
#if __iset__ >= 6
    arg.value = K2 * y * yv2;
    lres.value = yv2 + K1 / K2;
    ly.value = y + y + exp * KLN2_lo + constln_lo[i0];
    lres.llong = __builtin_e2k_fmad (arg.llong, lres.llong, ly.llong);
    return lres.value + dob + (y0.value + exp * KLN2_hi);
#else /* __iset__ < 6 */
    return y + y + exp * KLN2_lo + constln_lo[i0] + K2 * y * yv2 * (yv2 + K1 / K2) + dob + (y0.value + exp * KLN2_hi);
#endif /* __iset__ < 6 */
}

strong_alias (__ieee754_acosh, __acosh_finite)
strong_alias (__ieee754_acosh, __acosh)
weak_alias (__ieee754_acosh, acosh)
#ifdef NO_LONG_DOUBLE
strong_alias (__ieee754_acosh, __acoshl)
weak_alias (__ieee754_acosh, acoshl)
#endif
