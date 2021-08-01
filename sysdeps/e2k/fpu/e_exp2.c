/* Copyright (c) 2016-2018 ZAO "MCST". All rights reserved. */

#include "f2c.h"
#include <e2kintrin.h>

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ÚÔÁÚ·ÌÌ·  ˜˘˛ÈÛÏÒÂÙ  ˙Ó·˛ÂÓÈÂ  ÊıÓÎ„ÈÈ            %
%  F(Ë) = 2 ** Ë.  ÓÂ‰ÔıÛÙÈÌ˘Ì   ˙Ó·˛ÂÓÈÂÌ   ·ÚÁıÌÂÓÙ·  %
%  Ò˜ÏÒÂÙÛÒ  Ë >= 1024 ÈÏÈ X <= -1075.                   %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

DB __exp2 (DB X)

#define KA5 1.21268036650713469551e-15
#define KA4 2.23939601174027490360e-12
#define KA3 3.30830268054125203755e-09
#define KA2 3.66556559690979825976e-06
#define KA1 2.70760617406228636492e-03
#define LKA5 0x3cd5d8804780a73aLL
#define LKA4 0x3d83b2abe8ddb6c0LL
#define LKA3 0x3e2c6b08d7049fa0LL
#define LKA2 0x3ecebfbdff82c2b8LL
#define LKA1 0x3f662e42fefa39efLL
{
    _type_double_bits arg;
    DB y, yv2, hi;
    long k, ind = 0;
#if __iset__ >= 6
    _type_double_bits ly, lyv2, lyv4, lres, lres1;
#endif /* __iset__ < 6 */

    arg.value = X;
    arg.llong &= 0x7fffffffffffffffLL;

    if (arg.llong >= 0x7ff0000000000000LL) {    /* X == NaN …Ã… Inf */
        if (X != X)
            X = X - X;
        else if (X < 0.0)
            X = 0.0;            /* NaN */
        return X;
    }
    y = X * 256.0;
    if (X >= 1024 || X <= -1075)
        return SVID_libm_err (X, X, X >= 1024 ? 44 : 45);
    k = (int) y;
#if __iset__ <= 2
    hi = (DB) k;
#else /*  __iset__ <= 2 */
#pragma asm_inline
    __asm ("fdtoifd 0x3,%1,%0" : "=r" (hi): "r" (y));
#endif /*  __iset__ <= 2 */
    y -= hi;
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
    ind = k & 255;
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

strong_alias (__exp2, __exp2_finite)
weak_alias (__exp2, exp2)
weak_alias (__exp2, __ieee754_exp2)
#ifdef NO_LONG_DOUBLE
strong_alias (__exp2, __exp2l)
weak_alias (__exp2, exp2l)
#endif
