/* Copyright (c) 2016-2018 ZAO "MCST". All rights reserved. */

#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ðòïçòáííá  ÷ùþéóìñåô  úîáþåîéå  æõîëãéé  'áòëôáî-  %
%  çåîó  çéðåòâïìéþåóëéê'.  îåäïðõóôéíùí   úîáþåîéåí     %
%  áòçõíåîôá  ñ÷ìñåôóñ  |X| >= 1.                        %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

DB __ieee754_atanh (DB X)

#define KA9 7.02947397340087677502e-02
#define KA8 5.63478806993429477345e-02
#define KA7 6.68535556128915833244e-02
#define KA6 7.69146442340385597500e-02
#define KA5 9.09093250897611022948e-02
#define KA4 1.11111107165099093537e-01
#define KA3 1.42857142895249832675e-01
#define KA2 1.99999999999813420962e-01
#define KA1 3.33333333333333687215e-01
#define K2 4.00006488775094945494e-01
#define K1 6.66666666629825839191e-01
#define LKA9 0x3fb1fed6083d07beLL
#define LKA8 0x3facd9a1219aecc2LL
#define LKA7 0x3fb11d508afa8b34LL
#define LKA6 0x3fb3b0ad99919465LL
#define LKA5 0x3fb745d5622975f6LL
#define LKA4 0x3fbc71c70b7f1778LL
#define LKA3 0x3fc24924925e17a7LL
#define LKA2 0x3fc9999999997f57LL
#define LKA1 0x3fd555555555555cLL

{
    _type_double_bits arg, y0, yd;
    DB y, yv2, yv4;
    LL exp, i0;
#if __iset__ >= 6
    _type_double_bits ly, lyv2, lyv4, lyv8, lres, lres1, lres2;
#endif /* __iset__ < 6 */

    arg.value = dfabs (X);
    if (arg.llong >= 0x3ff0000000000000LL)        /* |X| >= 1.0 */
        return SVID_libm_err (X, X, arg.llong > 0x3ff0000000000000LL ? 30 : 31);   /* atanh(x>=1) */
    if (arg.llong <= 0x3fd0000000000000LL) {       /* |X| <= 0.25 */
        yv2 = X * X;
        yv4 = yv2 * yv2;
#if __iset__ >= 6
        lyv2.value = yv2;
        lyv4.value = yv4;
        lyv8.value = yv4 * yv4;
        lres.value = KA1 * yv2;
        lres1.llong = __builtin_e2k_fmad (LKA3, lyv2.llong, LKA2);
        lres2.llong = __builtin_e2k_fmad (lres1.llong, lyv4.llong, lres.llong);
        lres.llong = __builtin_e2k_fmad (LKA7, lyv2.llong, LKA6);
        lres1.llong = __builtin_e2k_fmad (LKA9, lyv2.llong, LKA8);
        lres1.llong = __builtin_e2k_fmad (lres1.llong, lyv4.llong, lres.llong);
        lres.llong = __builtin_e2k_fmad (LKA5, lyv2.llong, LKA4);
        lres1.llong = __builtin_e2k_fmad (lres1.llong, lyv4.llong, lres.llong);
        arg.llong = __builtin_e2k_fmad (lres1.llong, lyv8.llong, lres2.llong);
        y0.value = X;
        y0.llong = __builtin_e2k_fmad (arg.llong, y0.llong, y0.llong);
        return y0.value;
#else /* __iset__ < 6 */
        return (KA2 * yv2 * (KA1 / KA2 + yv2) + KA5 * yv2 * yv4 * (KA3
            / KA5 + yv2 * (yv2 + KA4 / KA5)) + KA9 * yv4 * (yv4 * yv4)
            * (KA6 / KA9 + KA7 / KA9 * yv2 + yv4 * (yv2 + KA8 / KA9))) * X + X;
#endif /* __iset__ < 6 */
    }
    yv2 = arg.value;            /*  0.5*log((1+x)/(1-x)) */
    y0.value = 1.0 - arg.value;
    arg.value = (1.0 + arg.value) / y0.value;
    arg.llong &= ~0xffffffffLL;
    y = 1.0 / y0.value;
    y0.llong &= ~0xffffffffLL;
    yv4 = 1.0 - y0.value - yv2;
    yd.value = 1.0 - arg.value * y0.value + yv2 - arg.value * yv4;
    exp = exponentabs (arg) - 1023;
    if (yd.value != 0) {
        yd.value *= y;
        yd.llong -= (exp << 52);
    }
    i0 = arg.llong & 0xfe00000000000LL;
    y0.llong = i0 | 0x3ff0000000000000LL;
    arg.llong &= 0xfffffffffffffLL;
    arg.llong |= 0x3ff0000000000000LL;
    y = (arg.value - y0.value + yd.value) / (arg.value + y0.value + yd.value);
    yv2 = y * y;
    y0.llong = ((LL) ((unsigned *) constln)[i0 >>= 45]) << 32;
#if __iset__ >= 6
    arg.value = K2 * y * yv2;
    lres.value = yv2 + K1 / K2;
    ly.value = y + y + exp * KLN2_lo + constln_lo[i0];
    lres.llong = __builtin_e2k_fmad (arg.llong, lres.llong, ly.llong);
    y = lres.value + (y0.value + exp * KLN2_hi);
#else /* __iset__ < 6 */
    y = y + y + exp * KLN2_lo + constln_lo[i0] + K2 * y * yv2 * (yv2 + K1 / K2) + (y0.value + exp * KLN2_hi);
#endif /* __iset__ < 6 */
    y0.value = X;
    y0.llong = (y0.llong & 0x8000000000000000LL) | 0x3fe0000000000000LL;
    return y * y0.value;
}

strong_alias (__ieee754_atanh, __atanh_finite)
strong_alias (__ieee754_atanh, __atanh)
weak_alias (__ieee754_atanh, atanh)
#ifdef NO_LONG_DOUBLE
strong_alias (__ieee754_atanh, __atanhl)
weak_alias (__ieee754_atanh, atanhl)
#endif
