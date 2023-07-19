/* Copyright (c) 2016-2018 ZAO "MCST". All rights reserved. */

#include "f2c.h"
#include <e2kintrin.h>

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     πςοηςαννα  χωώισμρετ  ϊξαώεξιε  ζυξλγιι  'αςλλο-   %
%  σιξυσ  ηιπεςβομιώεσλικ'.  ξεδοπυστινων   ϊξαώεξιεν    %
%  αςηυνεξτα  ρχμρετσρ  θ < 1.                           %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define KA2 -1.2195659587005627735642e-02
#define KA1  1.8857512797277079965641e-01
#define KA0 -2.5000000468047535430249e-01
#define KE2  2.6387974535515515832589e-01
#define KE1 -1.1292979199187320804116e+00
#define LKA2 0xbf88fa09b8a27bfbLL
#define LKA1 0x3fc8233ad3bdb451LL
#define LKA0 0xbfd0000005068f2cLL
#define LKE2 0x3fd0e367df182409LL
#define LKE1 0xbff2119ab217de56LL

#define KP2 -6.28900333356577654061e-01
#define KP1 -1.51725060893557945190e+01
#define KP0 -6.51127311169724174299e+01
#define KQ2  4.96761154196152803444e+01
#define KQ1  5.39944506851321373010e+02
#define KQ0  1.56270554706000457551e+03
#define LKP2 0xbfe41ff39786b850LL
#define LKP1 0xc02e5852b7d849b9LL
#define LKP0 0xc0504736fc9328d6LL
#define LKQ2 0x4048d68af337c8d3LL
#define LKQ1 0x4080df8e599baa30LL
#define LKQ0 0x40986ad27aedb209LL

#define LN2 0.6931471805599453094
#define K3  6.66690704586487441045e-01

FL __ieee754_acoshf (FL X)
{
    _type_double_bits y0;
    _type_float_bits arg;
    DB x, y, yp, yv2, dob;
#if __iset__ >= 6
    _type_double_bits ly, lyv2, ldob, lres, lres1, lres2;
#endif /* __iset__ < 6 */

    x = (DB) X;
    arg.value = X;
    if (arg.int0 < 0x3f800000 && arg.int0 <= 0xff800000) /* X < 1.0 && X >= -inf */
        return SVID_libm_err (X, X, 129);   /* acoshf(x<1) */
    if ((arg.int0 & 0x7fffffff) >= 0x7f800000) /* X == +-NaN ΙΜΙ +Inf */
        return X + X;
    if (arg.int0 <= 0x3fe00000) {       /* X <= 1.75 */
        arg.value = X - 1;
        if (arg.int0 == 0)
            return arg.value;
        arg.int0 += 0x800000;   /*  2*arg  */
        y = (DB) arg.value;
        yp = dsqrt (y);
        yv2 = y * y;
#if __iset__ >= 6
        ldob.value = y;
        lyv2.value = yv2;
        lres.value = KP0 * y;
        lres1.llong = __builtin_e2k_fmad (LKP2, ldob.llong, LKP1);
        lres2.llong = __builtin_e2k_fmad (lres1.llong, lyv2.llong, lres.llong);
        lres1.value = y + KQ2;
        lres.llong = __builtin_e2k_fmad (LKQ1, ldob.llong, LKQ0);
        lres1.llong = __builtin_e2k_fmad (lres1.llong, lyv2.llong, lres.llong);
        lres.value = lres2.value / lres1.value;
        ly.value = yp;
        ly.llong = __builtin_e2k_fmad (lres.llong, ly.llong, ly.llong);
        return (FL) ly.value;
#else /* __iset__ < 6 */
        return (FL) ((KP0 * y + yv2 * (KP1 + KP2 * y)) / (KQ0 + KQ1 * y + yv2 * (KQ2 + y)) * yp + yp);
#endif /* __iset__ < 6 */
    }                           /*  log(2|x|)+P2(x^2)/(x^2*Q2(x^2)) */
    if (arg.int0 < 0x47000000) {        /* |X| < 2**14 */
        y = x * x;
        yv2 = y * y;
#if __iset__ >= 6
        ly.value = y;
        lyv2.value = yv2;
        lres.llong = __builtin_e2k_fmad (LKA1, ly.llong, LKA2);
        lres2.llong = __builtin_e2k_fmad (LKA0, lyv2.llong, lres.llong);
        lres1.value = y + KE1;
        lres.value = y * KE2;
        lres1.llong = __builtin_e2k_fmad (lres1.llong, lyv2.llong, lres.llong);
        dob = lres2.value / lres1.value;
#else /* __iset__ < 6 */
        dob = (KA2 + KA1 * y + KA0 * yv2) / (KE2 * y + yv2 * (KE1 + y));
#endif /* __iset__ < 6 */
    }
    else {
        y = 0.5 / x;
        dob = -y * y;
    }
    yp = y0.value = (DB) arg.value;
    y0.llong = __builtin_e2k_paddd (y0.llong, 0x400000000000LL);
    y0.llong = __builtin_e2k_pandd (y0.llong, 0x7fff800000000000LL);
    y = (yp - y0.value) / (yp + y0.value);
#if __iset__ >= 6
    ly.value = y;
    lyv2.value = y * y;
    lres.value = LN2 * (DB) ((y0.llong >> 52) - 1022) + constnln[(y0.llong >> 47) & 0x1f];
    lres.llong = __builtin_e2k_fmad (ly.llong, 0x4000000000000000LL, lres.llong);
    lres1.value = K3 * y;
    lres.llong = __builtin_e2k_fmad (lres1.llong, lyv2.llong, lres.llong);
    return (FL) (lres.value + dob);
#else /* __iset__ < 6 */
    return (FL) (y + y + (LN2 * (DB) ((y0.llong >> 52) - 1022)
        + constnln[(y0.llong >> 47) & 0x1f]) + K3 * y * (y * y) + dob);
#endif /* __iset__ < 6 */
}

strong_alias (__ieee754_acoshf, __acoshf_finite)
strong_alias (__ieee754_acoshf, __acoshf)
weak_alias (__ieee754_acoshf, acoshf)
