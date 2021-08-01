/* Copyright (c) 2016-2018 ZAO "MCST". All rights reserved. */

#include "f2c.h"
#include <e2kintrin.h>

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     πςοηςαννα  χωώισμρετ  ϊξαώεξιε  ζυξλγιι            %
%  F(θ) = 10 ** θ.  ξεδοπυστινων   ϊξαώεξιεν   αςηυνεξ-  %
%  τα ρχμρετσρ  θ >= 38.5318413 ιμι X <= -45.1544991 .   %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define KA2 3.66556671660783833261e-06
#define KA1 2.70760782821392980564e-03
#define FL_MAX_LG 38.5318413F
#define FL_MIN_LG -45.1544991F
#define K256ONLG2 850.4135922911647185

FL __ieee754_exp10f (FL X)
{
    _type_float_bits arg;
    _type_double_bits rez;
    DB y;
    long k;

    arg.value = ffabs (X);
    if (arg.int0 >= 0x7f800000) {       /* X == NaN ΙΜΙ Inf */
        if (X < 0) X = 0;
        return X;
    }
    y = K256ONLG2 * (DB) X;
    if ((X >= FL_MAX_LG) || (X < FL_MIN_LG)) {
        return (FL) SVID_libm_err (X, X, (X < FL_MIN_LG) ? 147 : 146);
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

strong_alias (__ieee754_exp10f, __exp10f_finite)
weak_alias (__ieee754_exp10f, __exp10f)
weak_alias (__ieee754_exp10f, exp10f)

#if SHLIB_COMPAT (libm, GLIBC_2_1, GLIBC_2_27)
strong_alias (__ieee754_exp10f, __pow10f)
compat_symbol (libm, __pow10f, pow10f, GLIBC_2_1);
#endif
