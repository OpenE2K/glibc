/* Copyright (c) 2016-2018 ZAO "MCST". All rights reserved. */

#include "f2c.h"
#include <e2kintrin.h>

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ÚÔÁÚ·ÌÌ·  ˜˘˛ÈÛÏÒÂÙ  ˙Ó·˛ÂÓÈÂ  ÊıÓÎ„ÈÈ            %
%  F(Ë) = 10 ** Ë.  ÓÂ‰ÔıÛÙÈÌ˘Ì   ˙Ó·˛ÂÓÈÂÌ   ·ÚÁıÌÂÓÙ· %
%  Ò˜ÏÒÂÙÛÒ  Ë >= 308.2547155599167468    ÈÏÈ            %
%  X < -323.6072453387797623 .                           %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

DB __ieee754_exp10 (DB X)

#define KA5 5.39383070420133929351e-01
#define KA4 1.17125557824358809954e+00
#define KA3 2.03467859229340296817e+00
#define KA2 2.65094905523897638564e+00
#define KA1 2.30258509299404568402e+00
#define LKA5 0x3fe142a048ef0eaaLL
#define LKA4 0x3ff2bd767d3d053eLL
#define LKA3 0x4000470591de2bffLL
#define LKA2 0x40053524c73ce873LL
#define LKA1 0x40026bb1bbb55516LL

#define K256ONLG2 850.4135922911647185
#define KLG2ON256_hi 1.175898420342491590e-03
#define KLG2ON256_lo 2.199349533857460038e-13
#define LK256ONLG2 0x408a934f0979a371LL
#define LKLG2ON256_hi 0x3f53441350900000LL
#define LKLG2ON256_lo 0x3d4ef3fde623e256LL
{
    _type_double_bits arg;
    DB y, yv2, hi;
    long k, ind;
#if __iset__ >= 6
    _type_double_bits ly, lyv2, lyv4, lres, lres1;
#endif /* __iset__ < 6 */

    arg.value = X;
    arg.llong &= 0x7fffffffffffffffLL;

    if (arg.twolong.int0 >= 0x7ff00000) {       /* X == NaN …Ã… Inf */
        if (X != X)
            X = X - X;
        else if (X < 0.0)
            X = 0.0;            /* NaN */
        return X;
    }
    y = X * K256ONLG2;
    if (X >= 308.2547155599167468 || X < -323.6072453387797623)
        return SVID_libm_err (X, X, X >= 308.2547155599167468 ? 46 : 47);
    k = (int) y;
    ind = k & 255;
#if __iset__ <= 2
    hi = (DB) k;
#else /*  __iset__ <= 2 */
#pragma asm_inline
    __asm ("fdtoifd 0x3,%1,%0" : "=r" (hi): "r" (y));
#endif /*  __iset__ <= 2 */
#if __iset__ >= 6
    lres.value = X;
    ly.value = hi;
    lres.llong = __builtin_e2k_fnmad (LKLG2ON256_hi, ly.llong, lres.llong);
    y = lres.value - hi * KLG2ON256_lo;
#else /* __iset__ < 6 */
    y = X - hi * KLG2ON256_hi - hi * KLG2ON256_lo;
#endif /* __iset__ < 6 */
    yv2 = y * y;
#if __iset__ >= 6
    ly.value = y;
    lyv2.value = yv2;
    lyv4.value = yv2 * yv2;
    lres1.value = y * KA1;
    lres.llong = __builtin_e2k_fmad (LKA3, ly.llong, LKA2);
    lres1.llong = __builtin_e2k_fmad (lres.llong, lyv2.llong, lres1.llong);
    lres.llong = __builtin_e2k_fmad (LKA5, ly.llong, LKA4);
    lres.llong = __builtin_e2k_fmad (lres.llong, lyv4.llong, lres1.llong);
    y = lres.value;
#else /* __iset__ < 6 */
    y = KA2 * y * (y + KA1 / KA2) + KA3 * y * yv2 + yv2 * yv2 * (KA5 * y + KA4);
#endif /* __iset__ < 6 */
    hi = constexp[ind];
#if __iset__ >= 6
    ly.value = y;
    lres.value = constexp_lo[ind];
    arg.value = hi;
    lres.llong = __builtin_e2k_fmad (arg.llong, ly.llong, lres.llong);
    arg.value = lres.value + hi;
#else /* __iset__ < 6 */
    arg.value = hi * y + constexp_lo[ind] + hi;
#endif /* __iset__ < 6 */
    k = k >> 8;
    if (k < -1021)
        ind = k + 54;
    else
        ind = k;
    arg.llong = __builtin_e2k_paddd (arg.llong, ((LL) ind) << 52);
    if (k < -1021)
        return arg.value * K2IN54;
    else
        return arg.value;
}

strong_alias (__ieee754_exp10, __exp10_finite)
weak_alias (__ieee754_exp10, __exp10)
weak_alias (__ieee754_exp10, exp10)
#if SHLIB_COMPAT (libm, GLIBC_2_1, GLIBC_2_27)
strong_alias (__ieee754_exp10, __pow10)
compat_symbol (libm, __pow10, pow10, GLIBC_2_1);
#endif
#ifdef NO_LONG_DOUBLE
strong_alias (__exp10, __exp10l)
weak_alias (__exp10, exp10l)
# if SHLIB_COMPAT (libm, GLIBC_2_1, GLIBC_2_27)
strong_alias (__exp10l, __pow10l)
compat_symbol (libm, __pow10l, pow10l, GLIBC_2_1);
# endif
#endif
