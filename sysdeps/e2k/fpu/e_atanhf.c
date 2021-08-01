/* Copyright (c) 2016-2018 ZAO "MCST". All rights reserved. */

#include "f2c.h"
#include <e2kintrin.h>

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ðòïçòáííá  ÷ùþéóìñåô  úîáþåîéå  æõîëãéé  'áòëôáî-  %
%  çåîó  çéðåòâïìéþåóëéê'.  îåäïðõóôéíùí   úîáþåîéåí     %
%  áòçõíåîôá  ñ÷ìñåôóñ  |X| >= 1.                        %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define KA4 1.2335735537725661670e-01
#define KA3 1.4236779719745809580e-01
#define KA2 2.0000617202708831366e-01
#define KA1 3.3333332125858511352e-01
#define LKA4 0x3fbf9458ff10fd6cLL
#define LKA3 0x3fc2391ba47bbaf5LL
#define LKA2 0x3fc999cd5fed6012LL
#define LKA1 0x3fd55555485e4077LL

#define LN2 0.6931471805599453094
#define K3  6.66690704586487441045e-01

FL __ieee754_atanhf (FL X)
{
    _type_double_bits y0;
    _type_float_bits arg;
    DB x, y, yp, yv2, yv4;
#if __iset__ >= 6
    _type_double_bits ly, lyv2, lyv4, lyv3, lres, lres1, lres2;
#endif /* __iset__ < 6 */

    x = (DB) X;
    arg.value = ffabs (X);
    if (arg.int0 >= 0x3f800000)        /* |X| >= 1.0 */
        return SVID_libm_err (X, X, arg.int0 > 0x3f800000 ? 130 : 131);   /* atanhf(x>=1) */
    if (arg.int0 <= 0x3e800000) {       /* |X| <= 0.25 */
        yv2 = x * x;
        yv4 = yv2 * yv2;
#if __iset__ >= 6
        ly.value = x;
        lyv2.value = yv2;
        lyv4.value = yv4;
        lyv3.value = yv2 * x;
        lres1.llong = __builtin_e2k_fmad (LKA2, lyv2.llong, LKA1);
        lres2.llong = __builtin_e2k_fmad (lres1.llong, lyv3.llong, ly.llong);
        lyv3.value *= yv4; /* x^7 */
        lres1.llong = __builtin_e2k_fmad (LKA4, lyv2.llong, LKA3);
        lres.llong = __builtin_e2k_fmad (lres1.llong, lyv3.llong, lres2.llong);
        return (FL) lres.value;
#else /* __iset__ < 6 */
        return (FL) (x + KA2 * x * yv2 * (KA1 / KA2 + yv2) + x * yv2 * yv4 * (KA3 + KA4 * yv2));
#endif /* __iset__ < 6 */
    }
    /* 0.5*log((1+x)/(1-x)) */
    y = (DB) arg.value;
    yp = y0.value = (1.0 + y) / (1.0 - y);
    y0.llong = __builtin_e2k_paddd (y0.llong, 0x400000000000LL);
    y0.llong = __builtin_e2k_pandd (y0.llong, 0x7fff800000000000LL);
    y = (yp - y0.value) / (yp + y0.value);
#if __iset__ >= 6
    ly.value = y;
    lyv2.value = y * y;
    lres.value = LN2 * (DB) ((y0.llong >> 52) - 1023) + constnln[(y0.llong >> 47) & 0x1f];
    lres.llong = __builtin_e2k_fmad (ly.llong, 0x4000000000000000LL, lres.llong);
    lres1.value = K3 * y;
    lres.llong = __builtin_e2k_fmad (lres1.llong, lyv2.llong, lres.llong);
    y = lres.value;
#else /* __iset__ < 6 */
    y = y + y + (LN2 * (DB) ((y0.llong >> 52) - 1023) + constnln[(y0.llong >> 47) & 0x1f]) +
        K3 * y * (y * y);
#endif /* __iset__ < 6 */
    arg.value = X;
    arg.int0 = (arg.int0 & 0x80000000) | 0x3f000000;
    return arg.value * (FL) y;
}

strong_alias (__ieee754_atanhf, __atanhf_finite)
strong_alias (__ieee754_atanhf, __atanhf)
weak_alias (__ieee754_atanhf, atanhf)
