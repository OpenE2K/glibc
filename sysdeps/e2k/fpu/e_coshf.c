/* Copyright (c) 2016-2018 ZAO "MCST". All rights reserved. */

#include "f2c.h"
#include <e2kintrin.h>

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ðòïçòáííá  ÷ùþéóìñåô  úîáþåîéå  æõîëãéé  'ëïóéîõó  %
%  çéðåòâïìéþåóëéê'.  îåäïðõóôéíùí  úîáþåîéåí  áòçõ-     %
%  íåîôá  ñ÷ìñåôóñ  |è| > 89.4159851 .                   %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define KA2 3.66556671660783833261e-06
#define KA1 2.70760782821392980564e-03

FL __ieee754_coshf (FL X)
{
    _type_float_bits arg;
    _type_double_bits exp, exm;
    DB y, yp, ym;
    LL k, km;
#if __iset__ >= 6
    _type_double_bits ly, lres;
#endif /* __iset__ < 6 */

    arg.value = ffabs (X);
    y = (DB) arg.value;

    if (arg.int0 >= 0x7f800000) {       /* X == NaN ÉÌÉ Inf */
        return X * X;
    }
    if (arg.value > MAX_SINH_F) {
        return SVID_libm_err (X, X, 105);    /* coshf overflow */
    }
    y *= K256ONLN2;
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
    exp.llong = __builtin_e2k_paddd (exp.llong, ((k >> 8) - 1) << 52);
    exm.value = constnexp[km & 255];
    exm.llong = __builtin_e2k_paddd (exm.llong, ((km >> 8) - 1) << 52);
#if 0
//#if __iset__ >= 6
    ly.value = ym;
    lres.value = exp.value * yp;
    lres.llong = __builtin_e2k_fmad (exm.llong, ly.llong, lres.llong);
    return (FL) lres.value;
#else /* __iset__ < 6 */
    return (FL) (exp.value * yp + exm.value * ym);
#endif /* __iset__ < 6 */
}

strong_alias (__ieee754_coshf, __coshf_finite)
strong_alias (__ieee754_coshf, __coshf)
weak_alias (__ieee754_coshf, coshf)
