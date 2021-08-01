/* Copyright (c) 2016-2018 ZAO "MCST". All rights reserved. */

#include <errno.h>
#include "f2c.h"
#include <e2kintrin.h>

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ÚÔÁÚ·ÌÌ·  ˜˘˛ÈÛÏÒÂÙ  ˙Ó·˛ÂÓÈÂ  ÊıÓÎ„ÈÈ            %
%  F(Ë) = ÂËÚ (Ë) - 1.  ÓÂ‰ÔıÛÙÈÌ˘Ì   ˙Ó·˛ÂÓÈÂÌ   ·ÚÁı- %
%  ÌÂÓÙ·  Ò˜ÏÒÂÙÛÒ  Ë > 709.7827128933839731.            %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

DB __expm1 (DB X)

#define KB7 2.48032020420177499210e-05
#define KB6 1.98428845924519582148e-04
#define KB5 1.38888888652352410134e-03
#define KB4 8.33333330967948324107e-03
#define KB3 4.16666666666671799845e-02
#define KB2 1.66666666666671799891e-01
#define LKB7 0x3efa020f10d206b9LL
#define LKB6 0x3f2a022acec2a72bLL
#define LKB5 0x3f56c16c161af977LL
#define LKB4 0x3f81111110410155LL
#define LKB3 0x3fa555555555559fLL
#define LKB2 0x3fc555555555560eLL

#define KA5 8.33333478792298551112e-03
#define KA4 4.16666768487944911020e-02
#define KA3 1.66666666666665333693e-01
#define KA2 4.99999999999990669185e-01
#define LKA5 0x3f811111430bc5c6LL
#define LKA4 0x3fa55555accc11b8LL
#define LKA3 0x3fc5555555555525LL
#define LKA2 0x3fdfffffffffff58LL
{
    _type_double_bits arg, hi;
    DB y, yv2;
    long k, ind;
#if __iset__ >= 6
    _type_double_bits ly, lyv2, lyv4, lres, lres1, lres2;
#endif /* __iset__ < 6 */

    arg.value = X;
    arg.llong &= 0x7fffffffffffffffLL;
    y = X * K256ONLN2;

    if (arg.llong >= 0x7ff0000000000000LL || arg.llong == 0)  /* |X| == 0, NaN …Ã… Inf */
        return (X < -1) ? -1 : X + X;
    if (arg.llong < 0x3fb0000000000000LL) {        /* |X| < 0.0625 */
        y = X * X;
        yv2 = y * y;
#if __iset__ >= 6
        ly.value = X;
        lyv2.value = y;
        lyv4.value = yv2;
        lres1.llong = __builtin_e2k_fmad (LKB2, ly.llong, 0x3fe0000000000000LL);
        lres.llong = __builtin_e2k_fmad (LKB4, ly.llong, LKB3);
        lres2.llong = __builtin_e2k_fmad (lres.llong, lyv2.llong, lres1.llong);
        lres1.llong = __builtin_e2k_fmad (LKB7, ly.llong, LKB6);
        lres1.llong = __builtin_e2k_fmad (lres1.llong, ly.llong, LKB5);
        lres.llong = __builtin_e2k_fmad (lres1.llong, lyv4.llong, lres2.llong);
        lres.llong = __builtin_e2k_fmad (lres.llong, lyv2.llong, ly.llong);
        return lres.value;
#else /* __iset__ < 6 */
        return (0.5 + KB2 * X + y * (KB3 + KB4 * X) + yv2 * (KB5 + KB6 * X + KB7 * y)) * y + X;
#endif /* __iset__ < 6 */
    }
    if (X > DB_MAX_LOG) {
        __set_errno (ERANGE);
        return DB_MAX * DB_MAX;   /* overflow */
    }
    if (X <= -38)
        return DB_MIN_UN - 1.0;
    k = (int) y;
    ind = k & 255;
#if __iset__ <= 2
    y = (DB) k;
#else /*  __iset__ <= 2 */
#pragma asm_inline
    __asm ("fdtoifd 0x3,%1,%0" : "=r" (y): "r" (y));
#endif /*  __iset__ <= 2 */
#if __iset__ >= 6
    lres.value = X;
    ly.value = y;
    lres.llong = __builtin_e2k_fnmad (LKLN2ON256_hi, ly.llong, lres.llong);
    y = lres.value - y * KLN2ON256_lo;
#else /* __iset__ < 6 */
    y = X - y * KLN2ON256_hi - y * KLN2ON256_lo;
#endif /* __iset__ < 6 */
    yv2 = y * y;
#if __iset__ >= 6
    ly.value = y;
    lyv2.value = yv2;
    lyv4.value = yv2 * yv2;
    lres.llong = __builtin_e2k_fmad (LKA3, ly.llong, LKA2);
    lres1.llong = __builtin_e2k_fmad (lres.llong, lyv2.llong, ly.llong);
    lres.llong = __builtin_e2k_fmad (LKA5, ly.llong, LKA4);
    lres.llong = __builtin_e2k_fmad (lres.llong, lyv4.llong, lres1.llong);
    y = lres.value;
#else /* __iset__ < 6 */
    y = KA2 * y * (y + 1 / KA2) + KA3 * y * yv2 + yv2 * yv2 * (KA5 * y + KA4);
#endif /* __iset__ < 6 */
    hi.value = constexp[ind];
#if __iset__ >= 6
    ly.value = y;
    lres.value = constexp_lo[ind];
    arg.llong = __builtin_e2k_fmad (hi.llong, ly.llong, lres.llong);
#else /* __iset__ < 6 */
    arg.value = hi.value * y + constexp_lo[ind];
#endif /* __iset__ < 6 */
    k >>= 8;
    arg.llong = __builtin_e2k_paddd (arg.llong, ((LL) k) << 52);
    hi.llong = __builtin_e2k_paddd (hi.llong, ((LL) k) << 52);
    if (k > 50)
        X = arg.value - 1.0 + hi.value; /* X >= 51*log(2) */
    else if (k < -13)
        X = arg.value + hi.value - 1.0; /* X < -9.0136 */
    else {
        y = hi.value;
        hi.llong &= ~0xffffffffLL;
        X = hi.value - 1.0 + (y - hi.value + arg.value);
    }
    return X;
}

weak_alias (__expm1, expm1)
#ifdef NO_LONG_DOUBLE
strong_alias (__expm1, __expm1l)
weak_alias (__expm1, expm1l)
#endif
