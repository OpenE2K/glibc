/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     πςοηςαννα  χωώισμρετ  ϊξαώεξιε  ζυξλγιι            %
%  F(θ) = 2 ** θ.  ξεδοπυστινων   ϊξαώεξιεν   αςηυνεξTA  %
%  ρχμρετσρ  θ >= 16384 ιμι X <= -16446.                 %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define KA6  3.5023605298761446389230e-17L
#define KA5  3.8805808982911052890212e-14L
#define KA4  3.5830323053986465190648e-11L
#define KA3  2.6466421444307261308017e-08L
#define KA2  1.4662262387640424338771e-05L
#define KA1  5.4152123481245727327856e-03L

LD __exp2l (LD X)
{
    _type_long_double_bits arg;
    LD y, yv2, hi;
    LI k, ind;

    arg.value = ldfabs (X);
    if ((arg.twolong.short0 & 0x7fff) == 0x7fff) {    /* X == NaN ΙΜΙ Inf */
        if (X != X)
            X = X - X;     /* NaN */
        else if (X < 0)
            X = 0;            /* Inf */
        return X;
    }
    if (X >= 16384)
        return SVID_libm_err (X, 0.0, 244);
    if (X <= -16446)
        return SVID_libm_err (X, 0.0, 245);
    y = X * 128;
    k = (int) y;
    y = y - (LD) k;
    yv2 = y * y;
    y = KA2 * y * (y + KA1 / KA2) + KA4 * yv2 * (yv2 + KA3 / KA4 * y) + yv2 * yv2 * (KA5 * y + KA6 * yv2);
    ind = k & 127;
    hi = constexpl[ind];
    arg.value = hi * y + constexpl_lo[ind] + hi;
    k = k >> 7;
#if __iset__ < 4
    if (k < -16382)
        ind = k + 110;
    else
        ind = k;
    arg.twolong.short0 += ind;
    if (k < -16382)
        return arg.value * K2IN110;
    else
        return arg.value;
#else /* __iset__ < 4 */
#pragma asm_inline
    __asm ("fxscalesx %0,%1,%0" : "+r" (arg.value) : "r" (k));
    return arg.value;
#endif /* __iset__ < 4 */
}

strong_alias (__exp2l, __exp2l_finite)
weak_alias (__exp2l, exp2l)
weak_alias (__exp2l, __ieee754_exp2l)
