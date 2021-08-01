/* Copyright (c) 2016-2018 ZAO "MCST". All rights reserved. */

#include "f2c.h"
#include <e2kintrin.h>

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ðòïçòáííá  ÷ùþéóìñåô  úîáþåîéå  æõîëãéé  'óéîõó    %
%  çéðåòâïìéþåóëéê'.  îåäïðõóôéíùí  úîáþåîéåí  áòçõ-     %
%  íåîôá  ñ÷ìñåôóñ  |è| > 89.4159851 .                   %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define KB9 2.82880520977637142049e-06
#define KB7 1.98346674682791327720e-04
#define KB5 8.33335237769337037738e-03
#define KB3 1.66666665809424354159e-01
#define LKB9 0x3ec7bad01c45387eLL
#define LKB7 0x3f29ff68f649f313LL
#define LKB5 0x3f8111139f6d0773LL
#define LKB3 0x3fc55555537e0f34LL

#define KA2 3.66556671660783833261e-06
#define KA1 2.70760782821392980564e-03

FL __ieee754_sinhf (FL X)
{
    _type_float_bits arg;
    _type_double_bits exp, exm;
    DB y, yp, ym;
    LL k, km;
#if __iset__ >= 6
    _type_double_bits ly, lyv2, lyv3, lres, lres1, lres2;
#endif /* __iset__ < 6 */

    arg.value = ffabs (X);

    if (arg.int0 >= 0x7f800000) {       /* X == NaN ÉÌÉ Inf */
        return X + X;
    }
    if (arg.value > MAX_SINH_F) {
        return SVID_libm_err (X, X, 125);    /* sinhf overflow */
    }
    if (arg.int0 < 0x3f99999a) {        /* |X| < 1.2 */
        y = (DB) X;
        yp = y * y;
#if __iset__ >= 6
        ly.value = y;
        lyv2.value = yp;
        lyv3.value = y * yp;
        lres1.llong = __builtin_e2k_fmad (LKB5, lyv2.llong, LKB3);
        lres2.llong = __builtin_e2k_fmad (lres1.llong, lyv3.llong, ly.llong);
        lyv3.value *= yp * yp; /* x^7 */
        lres1.llong = __builtin_e2k_fmad (LKB9, lyv2.llong, LKB7);
        lres.llong = __builtin_e2k_fmad (lres1.llong, lyv3.llong, lres2.llong);
        return (FL) lres.value;
#else /* __iset__ < 6 */
        return (FL) (y + KB5 * y * yp * (yp + KB3 / KB5) + y * yp * (yp * yp) * (KB7 + KB9 * yp));
#endif /* __iset__ < 6 */
    }
    y = K256ONLN2 * (DB) arg.value;
    arg.value = X;
    arg.int0 &= 0x80000000;
    k = (LL) y;
    km = -k;
#if __iset__ <= 2
    ym = (DB) k;
#else /*  __iset__ <= 2 */
#pragma asm_inline
    __asm ("fdtoifd 0x3,%1,%0" : "=r" (ym): "r" (y));
#endif /*  __iset__ <= 2 */
    y -= ym;
#if __iset__ >= 6
    ly.value = y;
    lres.value = KA2 * y;
    lres.llong = __builtin_e2k_fmad (lres.llong, ly.llong, 0x3ff0000000000000LL);
    ym = lres.value;
#else /* __iset__ < 6 */
    ym = KA2 * y * y + 1.0;
#endif /* __iset__ < 6 */
    y = KA1 * y;
    yp = ym + y;
    ym -= y;
    exp.value = constnexp[k & 255];
    exp.llong = __builtin_e2k_paddd (exp.llong, (((k >> 8) - 1) << 52) + ((LL) arg.int0 << 32));
    exm.value = constnexp[km & 255];
    exm.llong = __builtin_e2k_paddd (exm.llong, (((km >> 8) - 1) << 52) + ((LL) arg.int0 << 32));
#if 0
//#if __iset__ >= 6
    ly.value = ym;
    lres.value = exp.value * yp;
    lres.llong = __builtin_e2k_fnmad (exm.llong, ly.llong, lres.llong);
    return (FL) lres.value;
#else /* __iset__ < 6 */
    return (FL) (exp.value * yp - exm.value * ym);
#endif /* __iset__ < 6 */
}

strong_alias (__ieee754_sinhf, __sinhf_finite)
strong_alias (__ieee754_sinhf, __sinhf)
weak_alias (__ieee754_sinhf, sinhf)
