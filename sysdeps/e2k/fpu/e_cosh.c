/* Copyright (c) 2016-2018 ZAO "MCST". All rights reserved. */

#include "f2c.h"
#include <e2kintrin.h>

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ÚÔÁÚ·ÌÌ·  ˜˘˛ÈÛÏÒÂÙ  ˙Ó·˛ÂÓÈÂ  ÊıÓÎ„ÈÈ  'ÎÔÛÈÓıÛ  %
%  ÁÈÂÚ‚ÔÏÈ˛ÂÛÎÈÍ'.  ÓÂ‰ÔıÛÙÈÌ˘Ì  ˙Ó·˛ÂÓÈÂÌ  ·ÚÁı-     %
%  ÌÂÓÙ·  Ò˜ÏÒÂÙÛÒ  |Ë| >= 710.4758600739439771 .        %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

DB __ieee754_cosh (DB X)

#define KB14 1.15588856334052064971e-11
#define KB12 2.08761083951577682032e-09
#define KB10 2.75573216530826621342e-07
#define KB8 2.48015872967165158214e-05
#define KB6 1.38888888888939018688e-03
#define KB4 4.16666666666666439202e-02
#define LKB14 0x3da96b12fa1d084dLL
#define LKB12 0x3e21eeb46cc41955LL
#define LKB10 0x3e927e4fd2d2046cLL
#define LKB8  0x3efa01a019ebb078LL
#define LKB6  0x3f56c16c16c1751fLL
#define LKB4  0x3fa5555555555552LL

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

    if (arg.llong >= 0x7ff0000000000000LL) /* X == NaN …Ã… Inf */
        return X * X;
    y = arg.value * K256ONLN2;
    k = (LL) y;
#if __iset__ <= 2
    y = (DB) k;
#else /*  __iset__ <= 2 */
#pragma asm_inline
    __asm ("fdtoifd 0x3,%1,%0" : "=r" (y): "r" (y));
#endif /*  __iset__ <= 2 */
    if (arg.llong < 0x3fe6666666666666LL) {        /* |X| < 0.7 */
        yv2 = X * X;
        yp = yv2 * yv2;
#if __iset__ >= 6
        lyv2.value = yv2;
        lyv4.value = yp;
        lyv8.value = yp * yp;
        lres.llong = __builtin_e2k_fmad (LKB6, lyv2.llong, LKB4);
        lres1.llong = __builtin_e2k_fmad (LKB10, lyv2.llong, LKB8);
        lres1.llong = __builtin_e2k_fmad (lres1.llong, lyv4.llong, lres.llong);
        lres.llong = __builtin_e2k_fmad (LKB14, lyv2.llong, LKB12);
        lres1.llong = __builtin_e2k_fmad (lres.llong, lyv8.llong, lres1.llong);
        lres.value = 0.5 * yv2;
        lres.llong = __builtin_e2k_fmad (lres1.llong, lyv4.llong, lres.llong);
        return lres.value + 1.0;
#else /* __iset__ < 6 */
        return yv2 * (KB4 * yv2) + yv2 * yp * (KB8 * yv2 + KB6) + yp * yp * (KB14
            * yv2 * yp + KB12 * yv2 * (yv2 + KB10 / KB12)) + 0.5 * yv2 + 1.0;
#endif /* __iset__ < 6 */
    }
    if (arg.value > MAX_SINH)
        return SVID_libm_err (X, X, 5);    /* cosh overflow */
    km = -k;
#if __iset__ >= 6
    ly.value = y;
    lres.llong = __builtin_e2k_fnmad (LKLN2ON256_hi, ly.llong, arg.llong);
    y = lres.value - y * KLN2ON256_lo;
#else /* __iset__ < 6 */
    y = arg.value - y * KLN2ON256_hi - y * KLN2ON256_lo;
#endif /* __iset__ < 6 */
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
    k = ((k >> 8) - 1) << 52;
    indm = km & 255;
    km = ((km >> 8) + 2) << 52;
    arg.value = constexp[ind];
    exp.value = constexp_lo[ind];
    exm.value = constexp[indm];
    arg.llong = __builtin_e2k_paddd (arg.llong, k);
    exp.llong = __builtin_e2k_paddd (exp.llong, k);
    exm.llong = __builtin_e2k_paddd (exm.llong, km);
#if __iset__ >= 6
    ly.value = yp;
    exp.llong = __builtin_e2k_fmad (arg.llong, ly.llong, exp.llong);
    ly.value = ym;
    exm.llong = __builtin_e2k_fmad (exm.llong, ly.llong, exm.llong);
    exm.llong = __builtin_e2k_fmad (exm.llong, 0x3fc0000000000000LL, exp.llong);
    return exm.value + arg.value;
#else /* __iset__ < 6 */
    exp.value = arg.value * yp + exp.value;
    exm.value = (exm.value * ym + exm.value) * 0.125;
    return exp.value + exm.value + arg.value;
#endif /* __iset__ < 6 */
}

strong_alias (__ieee754_cosh, __cosh_finite)
strong_alias (__ieee754_cosh, __cosh)
weak_alias (__ieee754_cosh, cosh)
#ifdef NO_LONG_DOUBLE
strong_alias (__ieee754_cosh, __coshl)
weak_alias (__ieee754_cosh, coshl)
#endif
