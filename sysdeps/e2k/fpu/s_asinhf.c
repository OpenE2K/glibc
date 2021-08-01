/* Copyright (c) 2016-2018 ZAO "MCST". All rights reserved. */

#include "f2c.h"
#include <e2kintrin.h>

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ÚÔÁÚ·ÌÌ·  ˜˘˛ÈÛÏÒÂÙ  ˙Ó·˛ÂÓÈÂ  ÊıÓÎ„ÈÈ  '·ÚÎÛÈ-   %
%  ÓıÛ  ÁÈÂÚ‚ÔÏÈ˛ÂÛÎÈÍ'.                                %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define KB7 -5.9410177953627928138e-03
#define KB6  4.1972093585195796583e-02
#define KB5 -1.2043309485631575884e-01
#define KB4  1.5966002224888212191e-01
#define KB3 -3.3840552438602108931e-02
#define KB2 -1.5943863964676613376e-01
#define KB1 -5.8795924806167372718e-04
#define KB0  9.9998273878471699572e-01
#define LKB7 0xbf78559bd22d355cLL
#define LKB6 0x3fa57d5dc2960cecLL
#define LKB5 0xbfbed4b40bc391b4LL
#define LKB4 0x3fc46fbd5704cf75LL
#define LKB3 0xbfa1538c84013686LL
#define LKB2 0xbfc4687c3f803416LL
#define LKB1 0xbf434428def37cbaLL
#define LKB0 0x3fefffdbccf4a2c7LL

#define KA2 3.32807466760169641890e-02
#define KA1 2.21012300181659504808e-01
#define KA0 2.49999998982016857723e-01
#define KE3 2.31940318931796337809e-02
#define KE2 3.96942784568516090335e-01
#define KE1 1.25904870826282843417e+00
#define LKA2 0x3fa10a2c8d1eb23cLL
#define LKA1 0x3fcc4a218ca5a090LL
#define LKA0 0x3fcffffffdd05b9cLL
#define LKE3 0x3f97c02d21c63bcdLL
#define LKE2 0x3fd96782b586b781LL
#define LKE1 0x3ff425104220f552LL

#define KP4 -1.4288508088872718324e-02
#define KP3  2.8783606219822368416e-02
#define KP2 -4.4506510993737653792e-02
#define KP1  7.4995826302391266349e-02
#define KP0 -1.6666664588074717179e-01
#define LKP4 0xbf8d434b179a4bbfLL
#define LKP3 0x3f9d79731d8014c7LL
#define LKP2 0xbfa6c98eb25ab5b9LL
#define LKP1 0x3fb332ed2d4e25fcLL
#define LKP0 0xbfc5555528b2273bLL

#define LN2 0.6931471805599453094
#define K3  6.66690704586487441045e-01

FL __asinhf (FL X)
{
    _type_double_bits y0;
    _type_float_bits arg;
    DB x, y, yp, yv2, yv4, dob;
#if __iset__ >= 6
    _type_double_bits ly, lyv2, lyv4, lyv5, lres, lres1, lres2;
#endif /* __iset__ < 6 */

    x = (DB) X;
    arg.value = ffabs (X);

    if (arg.int0 <= 0x3f99999a) {       /* |X| <= 1.2 */
        yv2 = x * x;
        yv4 = yv2 * yv2;
        if (arg.int0 <= 0x3f000000) {    /* |X| <= 0.5 */
            if (arg.int0 < 0x38000000)     /* |X| <= 2**-14 */
                return X + X * (X * X);
#if __iset__ >= 6
            ly.value = x;
            lyv2.value = yv2;
            lyv4.value = yv4;
            lyv5.value = yv4 * x;
            lres1.value = KP0 * x;
            lres2.llong = __builtin_e2k_fmad (lres1.llong, lyv2.llong, ly.llong);
            lres.llong = __builtin_e2k_fmad (LKP2, lyv2.llong, LKP1);
            lres1.llong = __builtin_e2k_fmad (LKP4, lyv2.llong, LKP3);
            lres1.llong = __builtin_e2k_fmad (lres1.llong, lyv4.llong, lres.llong);
            lres.llong = __builtin_e2k_fmad (lres1.llong, lyv5.llong, lres2.llong);
            return (FL) lres.value;
#else /* __iset__ < 6 */
            return (FL) (x + KP1 * x * yv2 * (KP0 / KP1 + yv2) + KP4 * x * yv2 *
                         yv4 * (KP2 / KP4 + yv2 * (yv2 + KP3 / KP4)));
#endif /* __iset__ < 6 */
        }
        else {
            y = (DB) arg.value;
#if __iset__ >= 6
            y0.value = x;
            ly.value = y;
            lyv2.value = yv2;
            lyv4.value = yv4;
            lres.llong = __builtin_e2k_fmad (LKB1, ly.llong, LKB0);
            lres1.llong = __builtin_e2k_fmad (LKB3, ly.llong, LKB2);
            lres2.llong = __builtin_e2k_fmad (lres1.llong, lyv2.llong, lres.llong);
            lres.llong = __builtin_e2k_fmad (LKB5, ly.llong, LKB4);
            lres1.llong = __builtin_e2k_fmad (LKB7, ly.llong, LKB6);
            lres1.llong = __builtin_e2k_fmad (lres1.llong, lyv2.llong, lres.llong);
            lres.llong = __builtin_e2k_fmad (lres1.llong, lyv4.llong, lres2.llong);
            return (FL) (x * lres.value);
#else /* __iset__ < 6 */
            return (FL) (x * (KB0 + KB1 * y) + KB3 * x * yv2 * (KB2 / KB3 + y) +
                         KB7 * x * yv4 * (KB4 / KB7 + KB5 / KB7 * y + yv2 * (KB6 / KB7 + y)));
#endif /* __iset__ < 6 */
        }
    }                           /*  log(2|x|)+P2(x^2)/Q3(x^2) */
    if (arg.int0 >= 0x7f800000) /* |X| == NaN …Ã… Inf */
        return X + X;
    if (arg.int0 < 0x46000000) {        /* |X| < 2**12 */
        y = x * x;
        yv2 = y * y;
#if __iset__ >= 6
        ly.value = y;
        lyv2.value = yv2;
        lres.llong = __builtin_e2k_fmad (LKA1, ly.llong, LKA2);
        lres2.llong = __builtin_e2k_fmad (LKA0, lyv2.llong, lres.llong);
        lres1.value = y + KE1;
        lres.llong = __builtin_e2k_fmad (LKE2, ly.llong, LKE3);
        lres1.llong = __builtin_e2k_fmad (lres1.llong, lyv2.llong, lres.llong);
        dob = lres2.value / lres1.value;
#else /* __iset__ < 6 */
        dob = (KA2 + KA1 * y + KA0 * yv2) / (KE3 + KE2 * y + yv2 * (KE1 + y));
#endif /* __iset__ < 6 */
    }
    else {
        y = 0.5 / x;
        dob = y * y;
    }
    yp = y0.value = (DB) arg.value;
    y0.llong = __builtin_e2k_paddd (y0.llong, 0x400000000000LL);
    y0.llong = __builtin_e2k_pandd (y0.llong, 0x7fff800000000000LL);
    y = (yp - y0.value) / (yp + y0.value);
#if __iset__ >= 6
    ly.value = y;
    lyv2.value = y * y;
    lres.value = LN2 * (DB) ((y0.llong >> 52) - 1022) + constnln[(y0.llong >> 47) & 0x1f];
    lres.llong = __builtin_e2k_fmad (ly.llong, 0x4000000000000000LL, lres.llong);
    yp = lres.value;
    lres.value = dob;
    lres1.value = -K3 * y;
    lres.llong = __builtin_e2k_fmsd (lres1.llong, lyv2.llong, lres.llong);
    y = lres.value;
#else /* __iset__ < 6 */
    yp = y + y + (LN2 * (DB) ((y0.llong >> 52) - 1022) + constnln[(y0.llong >> 47) & 0x1f]);
    y = -K3 * y * (y * y) - dob;
#endif /* __iset__ < 6 */
    if (X > 0)
        return (FL) (yp - y);
    else
        return (FL) (y - yp);
}

weak_alias (__asinhf, asinhf)
