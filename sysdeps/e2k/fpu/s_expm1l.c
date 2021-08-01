/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

#include <math.h>
#include <math_private.h>

#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ÚÔÁÚ·ÌÌ·  ˜˘˛ÈÛÏÒÂÙ  ˙Ó·˛ÂÓÈÂ  ÊıÓÎ„ÈÈ            %
%  F(Ë) = ÂËÚ (Ë) - 1.  ÓÂ‰ÔıÛÙÈÌ˘Ì   ˙Ó·˛ÂÓÈÂÌ   ·ÚÁı- %
%  ÌÂÓÙ·  Ò˜ÏÒÂÙÛÒ  Ë > 709.7827128933839731.            %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define KB9  2.7558950268759900506595e-07L
#define KB8  2.7559276487341414795375e-06L
#define KB7  2.4801587261766509290874e-05L
#define KB6  1.9841269793484582853716e-04L
#define KB5  1.3888888888889199989956e-03L
#define KB4  8.3333333333337066572422e-03L
#define KB3  4.1666666666666666662869e-02L
#define KB2  1.6666666666666666662109e-01L

#define KA6  1.3888402125928659474269e-03L
#define KA5  8.3333407361571271053310e-03L
#define KA4  4.1666666667770658180590e-02L
#define KA3  1.6666666666661140796939e-01L
#define KA2  4.9999999999999999723523e-01L

LD __expm1l (LD X)
{
    _type_long_double_bits arg, hi;
    LD y, yv2;
    LI k, ind;

    arg.value = ldfabs (X);
    if ((arg.twolong.short0 & 0x7fff) == 0x7fff || X == 0) { /* X == NaN, Inf …Ã… 0 */
         return (X < -1) ? -1 : X + X;
    }
    if ((arg.twolong.short0 & 0x7fff) < 0x3ffb) { /* |X| < 0.0625 */
        y = X * X;
        yv2 = y * y;
        return y * (KB2 * X + 0.5) + yv2 * (KB3 + KB4 * X) + KB9 * y * yv2 *
        (KB5/KB9 + KB6/KB9 * X + y * (KB7/KB9 + KB8/KB9 * X + y)) + X;
    }
    if (X > LD_MAX_LOG)
        return LD_MAX * LD_MAX; /* overflow */
    if (X <= -46)
        return LD_MIN_UN - 1; /* inexact */

    k = (int) (X * LD128ONLN2);
    ind = k & 127;
    y = (LD) k;
    y = X - y * LDLN2ON128_hi - y * LDLN2ON128_lo;
    yv2 = y * y;
    y = KA2 * y * (y + 1 / KA2) + KA4 * yv2 * (yv2 + KA3 / KA4 * y) + yv2 * yv2 * (KA5 * y + KA6 * yv2);
    hi.value = constexpl[ind];
    arg.value = hi.value * y + constexpl_lo[ind];
    k = k >> 7;
#if __iset__ < 4
    arg.twolong.short0 += k;
    hi.twolong.short0 += k;
#else /* __iset__ < 4 */
#pragma asm_inline
    __asm ("fxscalesx %0,%1,%0" : "+r" (arg.value) : "r" (k));
#pragma asm_inline
    __asm ("fxscalesx %0,%1,%0" : "+r" (hi.value) : "r" (k));
#endif /* __iset__ < 4 */
    if (k > 50) {
        X = arg.value - 1 + hi.value;    /* X >= 51*log(2) */
    }
    else {
        if (k < -13) {
            X = arg.value + hi.value - 1; /* X < -9.0136 */
        }
        else {
            y = hi.value; hi.twolong.long1 &= 0xffffffff00000000ULL;
            X = hi.value - 1 + (y - hi.value + arg.value);
        }
    }
    return X;
}

libm_hidden_def (__expm1l)
weak_alias (__expm1l, expm1l)
