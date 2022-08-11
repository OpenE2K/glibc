/* Copyright (c) 2016-2018 ZAO "MCST". All rights reserved. */

#include <errno.h>
#include "f2c.h"
#include <e2kintrin.h>

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ÚÔÁÚ·ÌÌ·  ˜˘˛ÈÛÏÒÂÙ  ˙Ó·˛ÂÓÈÂ  ÊıÓÎ„ÈÈ            %
%  F(Ë) = ÂËÚ (Ë) - 1.  ÓÂ‰ÔıÛÙÈÌ˘Ì   ˙Ó·˛ÂÓÈÂÌ   ·ÚÁı- %
%  ÌÂÓÙ·  Ò˜ÏÒÂÙÛÒ  Ë > 88.7228394.                      %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define KB4 8.33287762035138557882e-03f
#define KB3 4.16707679571888335950e-02f
#define KB2 1.66666667691996078656e-01f
#define IKB4 0x3c08869f
#define IKB3 0x3d2aaef8
#define IKB2 0x3e2aaaab

#define KA2 3.66556671660783833261e-06
#define KA1 2.70760782821392980564e-03
#define K256ONLN2 369.3299304675746271

FL __expm1f (FL X)
{
    _type_float_bits arg;
    _type_double_bits rez;
    DB y;
    long k;

    arg.value = ffabs (X);

    if (arg.int0 >= 0x7f800000 || arg.int0 == 0)  /* X == 0, NaN …Ã… Inf */
        return (X < -1) ? -1 : X + X;
    if (arg.int0 < 0x3d800000) {        /* |X| < 0.0625 */
        FL yv2 = X * X;
#if __iset__ >= 6
        _type_float_bits iy, iyv2, ires, ires1;
        iy.value = X;
        iyv2.value = yv2;
        ires.int0 = __builtin_e2k_fmas (IKB2, iy.int0, 0x3f000000);
        ires1.int0 = __builtin_e2k_fmas (IKB4, iy.int0, IKB3);
        ires.int0 = __builtin_e2k_fmas (ires1.int0, iyv2.int0, ires.int0);
        ires.int0 = __builtin_e2k_fmas (ires.int0, iyv2.int0, iy.int0);
        return ires.value;
#else /* __iset__ < 6 */
        return yv2 * (KB2 * X + 0.5f) + (yv2 * yv2) * (KB3 + KB4 * X) + X;
#endif /* __iset__ < 6 */
    }
    if (X > FL_MAX_LOG) {
        __set_errno (ERANGE);
        return FL_MAX * FL_MAX;   /* overflow */
    }
    if (X <= -28)
        return FL_MIN_UN - 1.0f;
    y = K256ONLN2 * (DB) X;
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
    _type_double_bits lres, lres1;
    lres.value = KA2 * y * (y + KA1 / KA2);
    lres1.value = rez.value - 1.0;
    lres.llong = __builtin_e2k_fmad (lres.llong, rez.llong, lres1.llong);
    return (FL) lres.value;
#else /* __iset__ < 6 */
    return (FL) (KA2 * y * (y + KA1 / KA2) * rez.value + (rez.value - 1.0));
#endif /* __iset__ < 6 */
}
weak_alias (__expm1f, expm1f)
