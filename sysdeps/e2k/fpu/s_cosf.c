/* Copyright (c) 2016-2018 ZAO "MCST". All rights reserved. */

#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ðòïçòáííá  ÷ùþéóìñåô  úîáþåîéå  æõîëãéé  'ëïóéîõó'.%
%  áòçõíåîô  ÷  òáäéáîáè.                                %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define KA8  9.05183071609456021563e-04
#define KA6 -2.08608471166298886810e-02
#define KA4  2.53669325182559742429e-01
#define KA2 -1.23370054671140197041e+00
#define KA9  1.58206303556477327304e-04
#define KA7 -4.68126625672624547281e-03
#define KA5  7.96925827256306198148e-02
#define KA3 -6.45964096146795350000e-01
#define KA1  1.57079632678809990359e+00
#define LKA8 0x3f4da939d840136bLL
#define LKA6 0xbf955c8bc087f7d0LL
#define LKA4 0x3fd03c1e43ea1435LL
#define LKA2 0xbff3bd3cc8d2ecb7LL
#define LKA9 0x3f24bc85ccb213adLL
#define LKA7 0xbf732ca9d79fc665LL
#define LKA5 0x3fb466bbac8a54e8LL
#define LKA3 0xbfe4abbce56ae6e6LL
#define LKA1 0x3ff921fb5443b587LL

#define KB8  2.4421785199368441433e-05
#define KB6 -1.3887135661461297298e-03
#define KB4  4.1666636654110536520e-02
#define KB2 -4.9999999861199384554e-01
#define LKB8 0x3ef99bac4cc39917LL
#define LKB6 0xbf56c0afd6683040LL
#define LKB4 0x3fa5555453870642LL
#define LKB2 0xbfdffffffe8277c0LL

FL __cosf (FL X)
{
    _type_float_bits arg;
    _type_double_bits darg;
    DB y, yv2, yv4;
    long k, ind;
#if __iset__ >= 6
    _type_double_bits lyv2, lyv3, lyv4, lres, lres1;
#endif /* __iset__ < 6 */

    arg.value = ffabs (X);
    y = K2ONPI * (DB) arg.value;

    if (arg.int0 <= 0x3f490fdb) {    /* pi / 4 */
        y = (DB) X;
        yv2 = y * y;
        yv4 = yv2 * yv2;
#if __iset__ >= 6
	lyv2.value = yv2;
        lyv4.value = yv4;
        lres.llong = __builtin_e2k_fmad (LKB2, lyv2.llong, 0x3ff0000000000000LL);
        lres.llong = __builtin_e2k_fmad (LKB4, lyv4.llong, lres.llong);
        lyv3.value = yv2 * yv4;
        lres1.llong = __builtin_e2k_fmad (LKB8, lyv2.llong, LKB6);
        lres.llong = __builtin_e2k_fmad (lres1.llong, lyv3.llong, lres.llong);
        return (FL) lres.value;
#else /* __iset__ < 6 */
        return (FL) (1.0 + KB2 * yv2 + KB4 * yv4 + yv2 * yv4 * (KB6 + KB8 * yv2));
#endif /* __iset__ < 6 */
    }

    if (unlikely (arg.int0 > 0x46c90fdb)) {    /* 2^13*pi */
        y = __rem_pio2f (arg.value, &ind);
        k = ind;
        /* if (k & 8) goto label; */   /* X = Inf or NaN */
    }
    else {
#if __iset__ <= 2
        darg.value = y + DVAIN52;
        y -= darg.value - DVAIN52;
        k = darg.twolong.int1;
#else /*  __iset__ <= 2 */
#pragma asm_inline
        __asm ("fdtoifd 0x0,%1,%0" : "=r" (darg.value): "r" (y)); /* rc=0 - to nearest */
        y -= darg.value;
        k = (int) darg.value;
#endif /*  __iset__ <= 2 */
    }
    yv2 = y * y;
    yv4 = yv2 * yv2;

#if __iset__ >= 6
    lyv2.value = yv2;
    if (k & 1) {
        lyv3.value = y * yv2;
        lres.value = KA1 * y;
        lres1.llong = __builtin_e2k_fmad (LKA5, lyv2.llong, LKA3);
        lres.llong = __builtin_e2k_fmad (lres1.llong, lyv3.llong, lres.llong);
	y = lres.value;
        lres.llong = __builtin_e2k_fmsd (LKA9 ^ 0x8000000000000000LL, lyv2.llong, LKA7);
	yv4 = lyv3.value * yv4 * lres.value;
    }
    else {
        lyv4.value = yv4;
        lres.llong = __builtin_e2k_fmad (LKA2, lyv2.llong, 0x3ff0000000000000LL);
        lres.llong = __builtin_e2k_fmad (LKA4, lyv4.llong, lres.llong);
	y = lres.value;
        lres.llong = __builtin_e2k_fmsd (LKA8 ^ 0x8000000000000000LL, lyv2.llong, LKA6);
	yv4 = yv2 * yv4 * lres.value;
    }
#else /* __iset__ < 6 */
    if (k & 1) {
        yv4 = -KA9 * y * yv2 * yv4 * (KA7/KA9 + yv2);
        y = KA1 * y + KA5 * y * yv2 * (KA3/KA5 + yv2);
    }
    else {
        y = 1.0 + KA2 * yv2 + KA4 * yv4;
        yv4 = yv2 * yv4 * (-KA6 - KA8 * yv2);
    }
#endif /* __iset__ < 6 */
    k++;
    /*label:*/
    return (FL) ((k & 2) ? yv4 - y : y - yv4);
}

weak_alias (__cosf, cosf)
