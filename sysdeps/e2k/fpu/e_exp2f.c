/* Copyright (c) 2016-2018 ZAO "MCST". All rights reserved. */

#include "f2c.h"
#include <e2kintrin.h>

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     πςοηςαννα  χωώισμρετ  ϊξαώεξιε  ζυξλγιι            %
%  F(θ) = 2 ** θ.  ξεδοπυστινων   ϊξαώεξιεν   αςηυνεξTA  %
%  ρχμρετσρ  θ >= 128 ιμι X <= -150.                     %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define KA2 3.66556671660783833261e-06
#define KA1 2.70760782821392980564e-03

FL __exp2f (FL X)
{
    _type_float_bits arg;
    _type_double_bits rez;
    DB y;
    long k;

    arg.value = ffabs (X);
    if (arg.int0 >= 0x7f800000) {       /* X == NaN ΙΜΙ Inf */
        if (X < 0)
            X = 0;
        return X;
    }
    y = (DB) X;
    if ((X >= 128) || (X <= -150)) {
        return (FL) SVID_libm_err (y, y, (X <= -150) ? 145 : 144);
    }
    y *= 256;
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

strong_alias (__exp2f, __exp2f_finite)
weak_alias (__exp2f, exp2f)
weak_alias (__exp2f, __ieee754_exp2f)
