/* Copyright (c) 2016-2018 ZAO "MCST". All rights reserved. */

#include "f2c.h"
#include <e2kintrin.h>

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ðòïçòáííá  ÷ùþéóìñåô  úîáþåîéå  æõîëãéé  'óéîõó    %
%  çéðåòâïìéþåóëéê'.  îåäïðõóôéíùí  úîáþåîéåí  áòçõ-     %
%  íåîôá  ñ÷ìñåôóñ  |è| >= 710.4758600739439771 .        %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

DB __ieee754_sinh (DB X)

#define KB15 7.72727099093074677037e-13
#define KB13 1.60581501778014887911e-10
#define KB11 2.50521133247516736418e-08
#define KB9 2.75573192099768740862e-06
#define KB7 1.98412698412887581019e-04
#define KB5 8.33333333333332375550e-03
#define KB3 1.66666666666666666746e-01
#define LKB15 0x3d6b301b612530f6LL
#define LKB13 0x3de611f59548abd6LL
#define LKB11 0x3e5ae645c0efd0d5LL
#define LKB9  0x3ec71de3a5244e2aLL
#define LKB7  0x3f2a01a01a01bb5dLL
#define LKB5  0x3f8111111111110cLL
#define LKB3  0x3fc5555555555555LL

#define KA5 8.33333478792298551112e-03
#define KA4 4.16666768487944911020e-02
#define KA3 1.66666666666665333693e-01
#define KA2 4.99999999999990669185e-01
#define LKA5 0x3f811111430bc5c6LL
#define LKA4 0x3fa55555accc11b8LL
#define LKA3 0x3fc5555555555525LL
#define LKA2 0x3fdfffffffffff58LL
{
    _type_double_bits arg, exp, exm;
    DB y, yv2, yp, ym;
    LL k, ind, km, indm;
#if __iset__ >= 6
    _type_double_bits ly, lyv2, lyv4, lyv8, lres, lres1;
#endif /* __iset__ < 6 */

    arg.value = X;
    arg.llong &= 0x7fffffffffffffffLL;

    if (arg.llong >= 0x7ff0000000000000LL) /* X == NaN ÉÌÉ Inf */
        return X + X;
    if (arg.value > MAX_SINH)
        return SVID_libm_err (X, X, 25);   /* sinh overflow */
    y = arg.value * K256ONLN2;
    k = (LL) y;
#if __iset__ <= 2
    y = (DB) k;
#else /*  __iset__ <= 2 */
#pragma asm_inline
    __asm ("fdtoifd 0x3,%1,%0" : "=r" (y): "r" (y));
#endif /*  __iset__ <= 2 */
    if (arg.llong < 0x3fecccccccccccccLL) {        /* |X| < 0.9 */
        yv2 = X * X;
        yp = yv2 * yv2;
#if __iset__ >= 6
        arg.value = X;
        lyv2.value = yv2;
        lyv4.value = yp;
        lyv8.value = yp * yp;
        lres.llong = __builtin_e2k_fmad (LKB5, lyv2.llong, LKB3);
        lres1.llong = __builtin_e2k_fmad (LKB9, lyv2.llong, LKB7);
        lres1.llong = __builtin_e2k_fmad (lres1.llong, lyv4.llong, lres.llong);
        lres.llong = __builtin_e2k_fmad (LKB15, lyv2.llong, LKB13);
        lres.llong = __builtin_e2k_fmad (lres.llong, lyv2.llong, LKB11);
        lres1.llong = __builtin_e2k_fmad (lres.llong, lyv8.llong, lres1.llong);
        lres.value = X * yv2;
        lres.llong = __builtin_e2k_fmad (lres1.llong, lres.llong, arg.llong);
        return lres.value;
#else /* __iset__ < 6 */
        return KB5 * X * yv2 * (KB3 / KB5 + yv2) + KB11 * X * yv2 * yp * (yv2 * (KB9 / KB11
            + yv2) + KB7 / KB11) + X * yp * (yp * yp) * (KB15 * yv2 + KB13) + X;
#endif /* __iset__ < 6 */
    }
    km = -k;
#if __iset__ >= 6
    ly.value = y;
    lres.llong = __builtin_e2k_fnmad (LKLN2ON256_hi, ly.llong, arg.llong);
    y = lres.value - y * KLN2ON256_lo;
#else /* __iset__ < 6 */
    y = arg.value - y * KLN2ON256_hi - y * KLN2ON256_lo;
#endif /* __iset__ < 6 */
    arg.value = X;
    arg.llong &= 0x8000000000000000LL;
    yv2 = y * y;
#if __iset__ >= 6
    ly.value = y;
    lyv2.value = yv2;
    lyv4.value = yv2 * y;
    lres.llong = __builtin_e2k_fmad (LKA4, lyv2.llong, LKA2);
    ym = lres.value * yv2;
    lres.llong = __builtin_e2k_fmad (LKA5, lyv2.llong, LKA3);
    lres.llong = __builtin_e2k_fmad (lres.llong, lyv4.llong, ly.llong);
    yv2 = lres.value;
#else /* __iset__ < 6 */
    ym = KA4 * yv2 * (KA2 / KA4 + yv2);
    yv2 = y + KA5 * y * yv2 * (KA3 / KA5 + yv2);
#endif /* __iset__ < 6 */
    yp = ym + yv2;
    ym = ym - yv2;
    ind = k & 255;
    k = __builtin_e2k_paddd (arg.llong, ((k >> 8) - 1) << 52);
    indm = km & 255;
    km = __builtin_e2k_paddd (arg.llong, ((km >> 8) + 2) << 52);
    arg.value = constexp[ind];
    exp.value = constexp_lo[ind];
    exm.value = constexp[indm];
    arg.llong += k;
    exp.llong += k;
    exm.llong += km;
#if __iset__ >= 6
    ly.value = yp;
    exp.llong = __builtin_e2k_fmad (arg.llong, ly.llong, exp.llong);
    ly.value = ym;
    exm.llong = __builtin_e2k_fmad (exm.llong, ly.llong, exm.llong);
    exm.llong = __builtin_e2k_fmad (exm.llong, 0xbfc0000000000000LL, exp.llong);
    return exm.value + arg.value;
#else /* __iset__ < 6 */
    exp.value = arg.value * yp + exp.value;
    exm.value = (exm.value * ym + exm.value) * 0.125;
    return (exp.value - exm.value + arg.value);
#endif /* __iset__ < 6 */
}

strong_alias (__ieee754_sinh, __sinh_finite)
strong_alias (__ieee754_sinh, __sinh)
weak_alias (__ieee754_sinh, sinh)
#ifdef NO_LONG_DOUBLE
strong_alias (__ieee754_sinh, __sinhl)
weak_alias (__ieee754_sinh, sinhl)
#endif
