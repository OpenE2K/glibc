/* Copyright (c) 2016-2018 ZAO "MCST". All rights reserved. */

#include "f2c.h"
#include <e2kintrin.h>

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ÚÔÁÚ·ÌÌ·  ˜˘˛ÈÛÏÒÂÙ  ˙Ó·˛ÂÓÈÂ  ÊıÓÎ„ÈÈ  'Ù·ÓÁÂÓÛ'.%
%  ·ÚÁıÌÂÓÙ  ˜  Ú·‰È·Ó·Ë.                                %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define KP2 -1.12840047793643231033e+01
#define KP0 -1.02840747639676895258e+01
#define KQ4  1.05713068418147687393e-01
#define KQ2 -4.43858717177940829375e+00
#define KQ0  1.61541868640304182743e+01
#define LKP2 0xc0269169130e8e10LL
#define LKP0 0xc02491723f59b865LL
#define LKQ4 0x3fbb1002fb9da21bLL
#define LKQ2 0xc011c11cfedcf366LL
#define LKQ0 0x40302778ca527bc3LL

#define KA17 1.7892511715456732319e-03
#define KA15 3.3644070550128140655e-11
#define KA13 4.4253157734259361208e-03
#define KA11 8.6092496627051106478e-03
#define KA9  2.1910677211362767497e-02
#define KA7  5.3965015806884189531e-02
#define KA5  1.3333342940622636141e-01
#define KA3  3.3333333286943828133e-01
#define LKA17 0x3f5d50a9d1081545LL
#define LKA15 0x3dc27efb63849d09LL
#define LKA13 0x3f722047a85765d3LL
#define LKA11 0x3f81a1b9edf49127LL
#define LKA9  0x3f966fc0a8395c25LL
#define LKA7  0x3faba14d740991d2LL
#define LKA5  0x3fc11111df61b2bbLL
#define LKA3  0x3fd5555554d5d19fLL

FL __tanf (FL X)
{
    _type_float_bits arg;
    _type_double_bits darg;
    DB y, yv2, yv4;
    long ind;
    int k;
#if __iset__ >= 6
    _type_double_bits ly, lyv2, lyv3, lyv4, lres, lres1, lres2;
#endif /* __iset__ < 6 */

    arg.value = ffabs (X);

    if (arg.int0 <= 0x3f490fdb) {    /* pi / 4 */
        y = (DB) X;
        yv2 = y * y;
        yv4 = yv2 * yv2;
#if __iset__ >= 6
	/* ◊Ÿ…«“Ÿ€¡ Œ≈‘, ‘≈ ÷≈ 24 ‘¡À‘¡ */
	ly.value = y;
        lyv2.value = yv2;
        lyv3.value = y * yv2;
        lyv4.value = yv4;
        lres1.llong = __builtin_e2k_fmad (LKA5, lyv2.llong, LKA3);
        lres2.llong = __builtin_e2k_fmad (lres1.llong, lyv3.llong, ly.llong);
        lyv3.value *= yv4; /* x^7 */
        lres1.llong = __builtin_e2k_fmad (LKA9, lyv2.llong, LKA7);
        lres2.llong = __builtin_e2k_fmad (lres1.llong, lyv3.llong, lres2.llong);
        lres.llong = __builtin_e2k_fmad (LKA13, lyv2.llong, LKA11);
        lres1.llong = __builtin_e2k_fmad (LKA17, lyv2.llong, LKA15);
        lres1.llong = __builtin_e2k_fmad (lres1.llong, lyv4.llong, lres.llong);
        lyv3.value *= yv4; /* x^11 */
        lres.llong = __builtin_e2k_fmad (lres1.llong, lyv3.llong, lres2.llong);
        return (FL) lres.value;
#else /* __iset__ < 6 */
        return (FL) (y + KA5 * y * yv2 * (KA3/KA5 + yv2) + KA9 * y * yv2 * yv4 * (KA7/KA9 + yv2)
            + KA17 * y * yv2 * (yv4 * yv4) * (KA11/KA17 + KA13/KA17 * yv2 + yv4 * (KA15/KA17 + yv2)));
#endif /* __iset__ < 6 */
    }

    if (unlikely (arg.int0 > 0x43c90fdb)) {    /* 2^7*pi */
        y = __rem_pio2f (arg.value, &ind);
        k = ind;
        /* if (k & 8) {yv2 = y; goto label;} */ /* X = Inf or NaN */
    }
    else {
        y = K2ONPI * (DB) arg.value;
#if __iset__ <= 2
        darg.value = y + DVAIN52;
        y -= darg.value - DVAIN52;
        k = darg.llong;
#else /*  __iset__ <= 2 */
#pragma asm_inline
        __asm ("fdtoifd 0x0,%1,%0" : "=r" (darg.value): "r" (y)); /* rc=0 - to nearest */
        y -= darg.value;
        k = (int) darg.value;
#endif /*  __iset__ <= 2 */
    }
    arg.value = X;
    yv2 = y * y;
#if __iset__ >= 6
    lyv2.value = yv2;
    lyv3.value = y * yv2;
    lres.value = KQ0 * y;
    lres1.llong = __builtin_e2k_fmad (LKQ4, lyv2.llong, LKQ2);
    lres.llong = __builtin_e2k_fmad (lres1.llong, lyv3.llong, lres.llong);
    y = lres.value;
    lres.llong = __builtin_e2k_fmsd (LKP2, lyv2.llong, LKP0);
    darg.llong = __builtin_e2k_fmad (lyv2.llong, lyv2.llong, lres.llong);
#else /* __iset__ < 6 */
    y = KQ4 * y * yv2 * (KQ2/KQ4 + yv2) + KQ0 * y;
    darg.value = yv2 * (yv2 + KP2) - KP0;
#endif /* __iset__ < 6 */
    darg.llong = __builtin_e2k_pxord (darg.llong, (LL) ((arg.int0 >> 31) + k) << 63);
    if (k & 1)
        return (FL) (darg.value / y);
    else
        return (FL) (y / darg.value);
}

weak_alias (__tanf, tanf)
