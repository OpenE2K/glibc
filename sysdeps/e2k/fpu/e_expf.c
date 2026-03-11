/* Copyright (c) 2016-2019 AO MCST. All rights reserved.
 * Distributed under the terms of MIT License.
 */

#include "f2c.h"
#include <e2kintrin.h>

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ПРОГРАММА  ВЫЧИСЛЯЕТ  ЗНАЧЕНИЕ  ФУНКЦИИ            %
%  F(Х) = ЕХР (Х).  НЕДОПУСТИМЫМ   ЗНАЧЕНИЕМ   АРГУМЕН-  %
%  ТА  ЯВЛЯЕТСЯ  Х > 88.7228394 ИЛИ X < -103.972076 .    %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define KA2 3.66556671660783833261e-06
#define KA1 2.70760782821392980564e-03
#define K256ONLN2 369.3299304675746271

FL __ieee754_expf (FL X)
{
    _type_float_bits arg;
    _type_double_bits rez;
    DB y;
    long k;

    arg.value = ffabs (X);
    if (arg.int0 >= 0x7f800000) {       /* X == NaN или Inf */
        if (X < 0)
            X = 0;
        return X;
    }
    y = K256ONLN2 * (DB) X;
    if ((X > FL_MAX_LOG) || (X < FL_MIN_LOG)) {
        return (FL) SVID_libm_err(X, 0, (X < FL_MIN_LOG) ? 107 : 106);
    }
    k = (int) y;
#if __iset__ <= 2
    y -= (DB) k;
#else /*  __iset__ <= 2 */
    DB ym;
#pragma asm_inline
    __asm ("fdtoifd 0x3,%1,%0" : "=r" (ym): "r" (y));
    y -= ym;
#endif /*  __iset__ <= 2 */
    rez.value = constnexp[k & 255];
    rez.llong = __builtin_e2k_paddd (rez.llong, ((LL) (k >> 8)) << 52);
#if __iset__ >= 6
    _type_double_bits lres;
    lres.value = KA2 * y * (y + KA1 / KA2);
    lres.llong = __builtin_e2k_fmad (lres.llong, rez.llong, rez.llong);
    return (FL) lres.value;
#else /* __iset__ < 6 */
    return (FL) (KA2 * y * (y + KA1 / KA2) * rez.value + rez.value);
#endif /* __iset__ < 6 */
}

strong_alias (__ieee754_expf, __expf_finite)
weak_alias (__ieee754_expf, expf)
