/* Copyright (c) 2016-2018 ZAO "MCST". All rights reserved. */

#include "f2c.h"
#include <e2kintrin.h>

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ðòïçòáííá  ÷ùþéóìñåô  çìá÷îïå  úîáþåîéå  æõîëãéé   %
%  'áòëëïóéîõó'.  îåäïðõóôéíùí   úîáþåîéåí   áòçõíåîôá   %
%  ñ÷ìñåôóñ  |è| > 1.  òåúõìøôáô  ÷  òáäéáîáè.           %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define KA13 3.36908472028301195240e-02
#define KA11 1.71492383576772531896e-02
#define KA9 3.11006627354945679275e-02
#define KA7 4.45994015285121837185e-02
#define KA5 7.50009454349742347622e-02
#define KA3 1.66666663374309070536e-01
#define LKA13 0x3fa13fed3dd245c1LL
#define LKA11 0x3f918f91e79712c6LL
#define LKA9  0x3f9fd8da2554dd34LL
#define LKA7  0x3fa6d5bb95f8e653LL
#define LKA5  0x3fb333430fcff28eLL
#define LKA3  0x3fc555554e435732LL

FL __acosf (FL X)
{
    _type_double_bits arg, arg1;
    DB y, z, yv2, yv4;
#if __iset__ >= 6
    _type_double_bits lz, lzv2, lzv4, lzv8, lres, lres1, lres2;
#endif /* __iset__ < 6 */

    arg.value = z = (DB) X;
    arg.llong = __builtin_e2k_pandd (arg.llong, 0x7fffffffffffffffLL);

    if (arg.twolong.int0 <= 0x3fe00000) {
        yv2 = z * z;
        yv4 = yv2 * yv2;
#if __iset__ >= 6
        lz.value = yv2;
        lzv2.value = yv2 * z;
        lzv4.value = yv4;
        lzv8.value = yv4 * lzv2.value;
        lres.value = PION2 - z;
        lres1.llong = __builtin_e2k_fmad (LKA5, lz.llong, LKA3);
        lres2.llong = __builtin_e2k_fnmad (lres1.llong, lzv2.llong, lres.llong);
        lres.llong = __builtin_e2k_fmad (LKA9, lz.llong, LKA7);
        lres1.llong = __builtin_e2k_fmad (LKA13, lz.llong, LKA11);
        lres1.llong = __builtin_e2k_fmad (lres1.llong, lzv4.llong, lres.llong);
        lres.llong = __builtin_e2k_fnmad (lres1.llong, lzv8.llong, lres2.llong);
        return (FL) lres.value;
#else /* __iset__ < 6 */
        return (FL) (PION2 - z - KA5 * z * yv2 * (KA3/KA5 + yv2) - KA13 * z *
            yv2 * yv4 * (KA7/KA13 + KA9/KA13 * yv2 + yv4 * (yv2 + KA11/KA13)));
#endif /* __iset__ < 6 */
    }
    if (X != X) { /* X=NaN */
        return X - X;
    }
    if (arg.value > 1) {    /* |X|>1 */
        return (FL) SVID_libm_err (z, 0, 101);
    }

    arg1.llong = __builtin_e2k_psubd (arg.llong, 0x10000000000000LL);
    yv2 = 0.5 - arg1.value;
    z = dsqrt (yv2);
    yv4 = yv2 * yv2;
#if __iset__ >= 6
    lz.value = yv2;
    lzv2.value = yv4;
    lzv4.value = yv4 * yv4;
    lres.llong = __builtin_e2k_fmad (LKA3 + 0x0010000000000000LL, lz.llong, 0x4000000000000000LL);
    lres1.llong = __builtin_e2k_fmad (LKA7 + 0x0010000000000000LL, lz.llong, LKA5 + 0x0010000000000000LL);
    lres2.llong = __builtin_e2k_fmad (lres1.llong, lzv2.llong, lres.llong);
    lres1.llong = __builtin_e2k_fmad (LKA13 + 0x0010000000000000LL, lz.llong, LKA11 + 0x0010000000000000LL);
    lres1.llong = __builtin_e2k_fmad (lres1.llong, lz.llong, LKA9 + 0x0010000000000000LL);
    lres.llong = __builtin_e2k_fmad (lres1.llong, lzv4.llong, lres2.llong);
    y = lres.value;
#else /* __iset__ < 6 */
    y = 2.0 + (2 * KA5) * yv2 * (KA3/KA5 + yv2) + (2 * KA13) * yv2 * yv4 *
        (KA7/KA13 + KA9/KA13 * yv2 + yv4 * (yv2 + KA11/KA13));
#endif /* __iset__ < 6 */
    y *= z;
    if (X < 0) {
        return (FL) (PI - y);
    }
    return (FL) y;
}

strong_alias (__acosf, __acosf_finite)
weak_alias (__acosf, acosf)
weak_alias (__acosf, __ieee754_acosf)
