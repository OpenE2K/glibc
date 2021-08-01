/* Copyright (c) 2016-2018 ZAO "MCST". All rights reserved. */

#include "f2c.h"
#include <e2kintrin.h>

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ПРОГРАММА  ВЫЧИСЛЯЕТ  ГЛАВНОЕ  ЗНАЧЕНИЕ  ФУНКЦИИ   %
%  'АРКТАНГЕНС'.  РЕЗУЛЬТАТ  В  РАДИАНАХ .               %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

DB __atan (DB X)

#define KB5 1.99999455044662533610e-01
#define KB3 -3.33333333333073478475e-01
#define LKB5 0x3fc9999507513c24LL
#define LKB3 0xbfd555555555430cLL

#define KA15 -6.35431043675711160064e-02
#define KA13 7.68566158858773828350e-02
#define KA11 -9.09083861491049053210e-02
#define KA9 1.11111107266816104432e-01
#define KA7 -1.42857142847146228750e-01
#define KA5 1.99999999999990244796e-01
#define KA3 -3.33333333333333331778e-01
#define LKA15 0xbfb0445c632520ddLL
#define LKA13 0x3fb3ace00bb6092cLL
#define LKA11 0xbfb745c5a1714aa0LL
#define LKA9  0x3fbc71c70beeee43LL
#define LKA7  0xbfc249249243a5acLL
#define LKA5  0x3fc999999999983aLL
#define LKA3  0xbfd5555555555555LL

#define KP4 -6.40349001231194236343e-02
#define KP2 -3.37480023171378478027e-01
#define KP0 -3.33333333333333332699e-01
#define KQ6 8.08859755436776222789e-02
#define KQ4 7.30997313513022422820e-01
#define KQ2 1.61244006951412662122e+00
#define LKP4  0xbfb0649759d4d839LL
#define LKP2  0xbfd59945cfa4c2daLL
#define LKP0  0xbfd5555555555555LL
#define LKQ6  0x3fb4b4f17baa4759LL
#define LKQ4  0x3fe764547a6012c3LL
#define LKQ2  0x3ff9cc8df5552b95LL
{
    _type_double_bits arg, f;
    DB y, yv2, yv4, yv8, c, s;
    LL ind;
#if __iset__ >= 6
    _type_double_bits ly, lyv2, lyv4, lyv8, lres, lres1, lres2;
#endif /* __iset__ < 6 */

    if (X == 0) return X; /* for X = -0.0 only */

    arg.value = dfabs (X);
    y = 1.0 / arg.value;
    if (unlikely (arg.twolong.int0 > 0x41300000)) {  /* |X|>2^20 или NaN */
        y -= PION2_lo;
        return (X > 0.0) ? PION2 - y : y - PION2;
    }
    yv2 = X * X;
    yv4 = yv2 * yv2;
    if (arg.twolong.int0 < 0x3fc00000) {  /* |X|<0.125 */
        yv8 = yv4 * yv4;
#if __iset__ >= 6
        ly.value = X;
        f.value = X * yv2;
        lyv2.value = yv2;
        lyv4.value = yv4;
        lyv8.value = yv8;
        lres.llong = __builtin_e2k_fmad (LKA5, lyv2.llong, LKA3);
        lres1.llong = __builtin_e2k_fmad (LKA9, lyv2.llong, LKA7);
        lres.llong = __builtin_e2k_fmad (lres1.llong, lyv4.llong, lres.llong);
        lres1.llong = __builtin_e2k_fmad (LKA15, lyv2.llong, LKA13);
        lres1.llong = __builtin_e2k_fmad (lres1.llong, lyv2.llong, LKA11);
        lres.llong = __builtin_e2k_fmad (lres1.llong, lyv8.llong, lres.llong);
        lres.llong = __builtin_e2k_fmad (f.llong, lres.llong, ly.llong);
        return lres.value;
#else /* __iset__ < 6 */
        return KA5 * X * yv2 * (KA3 / KA5 + yv2) + KA7 * X * yv2 * yv4 + KA11 * X *
               (KA9 / KA11 + yv2) * yv8 + X * yv4 * yv8 * (KA13 + KA15 * yv2) + X;
#endif /* __iset__ < 6 */
    }
    c = arg.value;
    arg.llong = __builtin_e2k_pandnd (arg.llong, 0xffffffffLL);
    if (arg.twolong.int0 > 0x40200000) {  /* |X|>8 */
        c -= arg.value;
        f.value = y;
        f.llong = __builtin_e2k_pandnd (f.llong, 0xffffffffLL);

#if __iset__ >= 6
        lres.llong = __builtin_e2k_fnmad (arg.llong, f.llong, 0x3ff0000000000000LL);
        ly.value = c;
        lres.llong = __builtin_e2k_fnmad (ly.llong, f.llong, lres.llong);
        ly.value = y;
        lres.llong = __builtin_e2k_fnmad (lres.llong, ly.llong, LPION2_lo);
        s = lres.value;

        lyv2.value = yv2;
        lyv4.value = yv4;
        lres.llong = __builtin_e2k_fmad (LKP2, lyv2.llong, LKP4);
        lres2.llong = __builtin_e2k_fmad (LKP0, lyv4.llong, lres.llong);
        lres.llong = __builtin_e2k_fmad (LKQ4, lyv2.llong, LKQ6);
        lres1.value = yv2 + KQ2;
        lres1.llong = __builtin_e2k_fmad (lres1.llong, lyv4.llong, lres.llong);
        lres.value = lres2.value / lres1.value;
        lres1.value = s;
        lres.llong = __builtin_e2k_fmsd (lres.llong, ly.llong, lres1.llong);
        y = lres.value;
#else /* __iset__ < 6 */
        s = PION2_lo - (1.0 - arg.value * f.value - c * f.value) * y;
        y *= (KP4 + KP2 * yv2 + KP0 * yv4) / (KQ6 + KQ4 * yv2 + yv4 * (KQ2 + yv2));
        y -= s;
#endif /* __iset__ < 6 */
        arg.value = PION2 - f.value;
    }
    else {
        arg.llong = __builtin_e2k_paddd (arg.llong, 0x100000000000LL);
        ind = __builtin_e2k_psubd (arg.llong, 0x3fc0000000000000LL) >> 45;
        arg.llong = __builtin_e2k_pandd (arg.llong, 0x7fffe00000000000LL);
#if __iset__ >= 6
        lres.value = c;
        lres.llong = __builtin_e2k_fmad (lres.llong, arg.llong, 0x3ff0000000000000LL);
        y = (arg.value - c) / lres.value;
#else /* __iset__ < 6 */
        y = (arg.value - c) / (1.0 + c * arg.value);
#endif /* __iset__ < 6 */
        yv2 = y * y;
#if __iset__ >= 6
        ly.value = y - constarctg_lo[ind];
        lres1.value = KB5 * y * yv2;
        lres.value = KB3 / KB5 + yv2;
        lres.llong = __builtin_e2k_fmad (lres1.llong, lres.llong, ly.llong);
        y = lres.value;
#else /* __iset__ < 6 */
        y = y - constarctg_lo[ind] + KB5 * y * yv2 * (KB3 / KB5 + yv2);
#endif /* __iset__ < 6 */
        arg.llong = (LL) ((unsigned *) constarctg)[ind] << 32;
    }
    return (X > 0) ? arg.value - y : y - arg.value;
}

weak_alias (__atan, atan)

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*         КОНСТАНТЫ  ФУНКЦИЙ  ATAN  и  ATAN2           */
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
const int constarctg[] = {
    0x3fbfd5ba,     /* ATAN (0x3fc00...0) */
    0x3fc00a5e,     /* ATAN (0x3fc02...0) */
    0x3fc029dd,     /* ATAN (0x3fc04...0) */
    0x3fc0495a,     /* ATAN (0x3fc06...0) */
    0x3fc068d5,     /* ATAN (0x3fc08...0) */
    0x3fc0884e,     /* ATAN (0x3fc0a...0) */
    0x3fc0a7c5,     /* ATAN (0x3fc0c...0) */
    0x3fc0c73a,     /* ATAN (0x3fc0e...0) */
    0x3fc0e6ad,     /* ATAN (0x3fc10...0) */
    0x3fc1061e,     /* ATAN (0x3fc12...0) */
    0x3fc1258d,     /* ATAN (0x3fc14...0) */
    0x3fc144fa,     /* ATAN (0x3fc16...0) */
    0x3fc16465,     /* ATAN (0x3fc18...0) */
    0x3fc183cd,     /* ATAN (0x3fc1a...0) */
    0x3fc1a334,     /* ATAN (0x3fc1c...0) */
    0x3fc1c298,     /* ATAN (0x3fc1e...0) */
    0x3fc1e1fa,     /* ATAN (0x3fc20...0) */
    0x3fc2015b,     /* ATAN (0x3fc22...0) */
    0x3fc220b8,     /* ATAN (0x3fc24...0) */
    0x3fc24014,     /* ATAN (0x3fc26...0) */
    0x3fc25f6e,     /* ATAN (0x3fc28...0) */
    0x3fc27ec5,     /* ATAN (0x3fc2a...0) */
    0x3fc29e1a,     /* ATAN (0x3fc2c...0) */
    0x3fc2bd6d,     /* ATAN (0x3fc2e...0) */
    0x3fc2dcbd,     /* ATAN (0x3fc30...0) */
    0x3fc2fc0b,     /* ATAN (0x3fc32...0) */
    0x3fc31b57,     /* ATAN (0x3fc34...0) */
    0x3fc33aa1,     /* ATAN (0x3fc36...0) */
    0x3fc359e8,     /* ATAN (0x3fc38...0) */
    0x3fc3792d,     /* ATAN (0x3fc3a...0) */
    0x3fc39870,     /* ATAN (0x3fc3c...0) */
    0x3fc3b7b0,     /* ATAN (0x3fc3e...0) */
    0x3fc3d6ee,     /* ATAN (0x3fc40...0) */
    0x3fc3f62a,     /* ATAN (0x3fc42...0) */
    0x3fc41563,     /* ATAN (0x3fc44...0) */
    0x3fc4349a,     /* ATAN (0x3fc46...0) */
    0x3fc453ce,     /* ATAN (0x3fc48...0) */
    0x3fc47300,     /* ATAN (0x3fc4a...0) */
    0x3fc49230,     /* ATAN (0x3fc4c...0) */
    0x3fc4b15d,     /* ATAN (0x3fc4e...0) */
    0x3fc4d087,     /* ATAN (0x3fc50...0) */
    0x3fc4efaf,     /* ATAN (0x3fc52...0) */
    0x3fc50ed5,     /* ATAN (0x3fc54...0) */
    0x3fc52df8,     /* ATAN (0x3fc56...0) */
    0x3fc54d18,     /* ATAN (0x3fc58...0) */
    0x3fc56c36,     /* ATAN (0x3fc5a...0) */
    0x3fc58b52,     /* ATAN (0x3fc5c...0) */
    0x3fc5aa6a,     /* ATAN (0x3fc5e...0) */
    0x3fc5c981,     /* ATAN (0x3fc60...0) */
    0x3fc5e894,     /* ATAN (0x3fc62...0) */
    0x3fc607a5,     /* ATAN (0x3fc64...0) */
    0x3fc626b4,     /* ATAN (0x3fc66...0) */
    0x3fc645bf,     /* ATAN (0x3fc68...0) */
    0x3fc664c9,     /* ATAN (0x3fc6a...0) */
    0x3fc683cf,     /* ATAN (0x3fc6c...0) */
    0x3fc6a2d3,     /* ATAN (0x3fc6e...0) */
    0x3fc6c1d4,     /* ATAN (0x3fc70...0) */
    0x3fc6e0d2,     /* ATAN (0x3fc72...0) */
    0x3fc6ffce,     /* ATAN (0x3fc74...0) */
    0x3fc71ec7,     /* ATAN (0x3fc76...0) */
    0x3fc73dbd,     /* ATAN (0x3fc78...0) */
    0x3fc75cb1,     /* ATAN (0x3fc7a...0) */
    0x3fc77ba2,     /* ATAN (0x3fc7c...0) */
    0x3fc79a90,     /* ATAN (0x3fc7e...0) */
    0x3fc7b97b,     /* ATAN (0x3fc80...0) */
    0x3fc7d863,     /* ATAN (0x3fc82...0) */
    0x3fc7f749,     /* ATAN (0x3fc84...0) */
    0x3fc8162b,     /* ATAN (0x3fc86...0) */
    0x3fc8350b,     /* ATAN (0x3fc88...0) */
    0x3fc853e8,     /* ATAN (0x3fc8a...0) */
    0x3fc872c3,     /* ATAN (0x3fc8c...0) */
    0x3fc8919a,     /* ATAN (0x3fc8e...0) */
    0x3fc8b06e,     /* ATAN (0x3fc90...0) */
    0x3fc8cf40,     /* ATAN (0x3fc92...0) */
    0x3fc8ee0f,     /* ATAN (0x3fc94...0) */
    0x3fc90cda,     /* ATAN (0x3fc96...0) */
    0x3fc92ba3,     /* ATAN (0x3fc98...0) */
    0x3fc94a69,     /* ATAN (0x3fc9a...0) */
    0x3fc9692c,     /* ATAN (0x3fc9c...0) */
    0x3fc987ec,     /* ATAN (0x3fc9e...0) */
    0x3fc9a6a8,     /* ATAN (0x3fca0...0) */
    0x3fc9c562,     /* ATAN (0x3fca2...0) */
    0x3fc9e419,     /* ATAN (0x3fca4...0) */
    0x3fca02cd,     /* ATAN (0x3fca6...0) */
    0x3fca217e,     /* ATAN (0x3fca8...0) */
    0x3fca402c,     /* ATAN (0x3fcaa...0) */
    0x3fca5ed6,     /* ATAN (0x3fcac...0) */
    0x3fca7d7e,     /* ATAN (0x3fcae...0) */
    0x3fca9c23,     /* ATAN (0x3fcb0...0) */
    0x3fcabac4,     /* ATAN (0x3fcb2...0) */
    0x3fcad962,     /* ATAN (0x3fcb4...0) */
    0x3fcaf7fe,     /* ATAN (0x3fcb6...0) */
    0x3fcb1696,     /* ATAN (0x3fcb8...0) */
    0x3fcb352b,     /* ATAN (0x3fcba...0) */
    0x3fcb53bd,     /* ATAN (0x3fcbc...0) */
    0x3fcb724b,     /* ATAN (0x3fcbe...0) */
    0x3fcb90d7,     /* ATAN (0x3fcc0...0) */
    0x3fcbaf5f,     /* ATAN (0x3fcc2...0) */
    0x3fcbcde4,     /* ATAN (0x3fcc4...0) */
    0x3fcbec66,     /* ATAN (0x3fcc6...0) */
    0x3fcc0ae5,     /* ATAN (0x3fcc8...0) */
    0x3fcc2960,     /* ATAN (0x3fcca...0) */
    0x3fcc47d8,     /* ATAN (0x3fccc...0) */
    0x3fcc664d,     /* ATAN (0x3fcce...0) */
    0x3fcc84bf,     /* ATAN (0x3fcd0...0) */
    0x3fcca32d,     /* ATAN (0x3fcd2...0) */
    0x3fccc199,     /* ATAN (0x3fcd4...0) */
    0x3fcce000,     /* ATAN (0x3fcd6...0) */
    0x3fccfe65,     /* ATAN (0x3fcd8...0) */
    0x3fcd1cc6,     /* ATAN (0x3fcda...0) */
    0x3fcd3b24,     /* ATAN (0x3fcdc...0) */
    0x3fcd597e,     /* ATAN (0x3fcde...0) */
    0x3fcd77d5,     /* ATAN (0x3fce0...0) */
    0x3fcd9629,     /* ATAN (0x3fce2...0) */
    0x3fcdb479,     /* ATAN (0x3fce4...0) */
    0x3fcdd2c6,     /* ATAN (0x3fce6...0) */
    0x3fcdf110,     /* ATAN (0x3fce8...0) */
    0x3fce0f56,     /* ATAN (0x3fcea...0) */
    0x3fce2d99,     /* ATAN (0x3fcec...0) */
    0x3fce4bd8,     /* ATAN (0x3fcee...0) */
    0x3fce6a14,     /* ATAN (0x3fcf0...0) */
    0x3fce884c,     /* ATAN (0x3fcf2...0) */
    0x3fcea681,     /* ATAN (0x3fcf4...0) */
    0x3fcec4b3,     /* ATAN (0x3fcf6...0) */
    0x3fcee2e1,     /* ATAN (0x3fcf8...0) */
    0x3fcf010b,     /* ATAN (0x3fcfa...0) */
    0x3fcf1f32,     /* ATAN (0x3fcfc...0) */
    0x3fcf3d56,     /* ATAN (0x3fcfe...0) */
    0x3fcf5b75,     /* ATAN (0x3fd00...0) */
    0x3fcf97ab,     /* ATAN (0x3fd02...0) */
    0x3fcfd3d1,     /* ATAN (0x3fd04...0) */
    0x3fd007f5,     /* ATAN (0x3fd06...0) */
    0x3fd025fa,     /* ATAN (0x3fd08...0) */
    0x3fd043f8,     /* ATAN (0x3fd0a...0) */
    0x3fd061ee,     /* ATAN (0x3fd0c...0) */
    0x3fd07fdd,     /* ATAN (0x3fd0e...0) */
    0x3fd09dc5,     /* ATAN (0x3fd10...0) */
    0x3fd0bba5,     /* ATAN (0x3fd12...0) */
    0x3fd0d97e,     /* ATAN (0x3fd14...0) */
    0x3fd0f750,     /* ATAN (0x3fd16...0) */
    0x3fd1151a,     /* ATAN (0x3fd18...0) */
    0x3fd132dc,     /* ATAN (0x3fd1a...0) */
    0x3fd15097,     /* ATAN (0x3fd1c...0) */
    0x3fd16e4a,     /* ATAN (0x3fd1e...0) */
    0x3fd18bf5,     /* ATAN (0x3fd20...0) */
    0x3fd1a999,     /* ATAN (0x3fd22...0) */
    0x3fd1c735,     /* ATAN (0x3fd24...0) */
    0x3fd1e4c9,     /* ATAN (0x3fd26...0) */
    0x3fd20255,     /* ATAN (0x3fd28...0) */
    0x3fd21fd9,     /* ATAN (0x3fd2a...0) */
    0x3fd23d56,     /* ATAN (0x3fd2c...0) */
    0x3fd25aca,     /* ATAN (0x3fd2e...0) */
    0x3fd27837,     /* ATAN (0x3fd30...0) */
    0x3fd2959b,     /* ATAN (0x3fd32...0) */
    0x3fd2b2f7,     /* ATAN (0x3fd34...0) */
    0x3fd2d04c,     /* ATAN (0x3fd36...0) */
    0x3fd2ed98,     /* ATAN (0x3fd38...0) */
    0x3fd30adc,     /* ATAN (0x3fd3a...0) */
    0x3fd32818,     /* ATAN (0x3fd3c...0) */
    0x3fd3454b,     /* ATAN (0x3fd3e...0) */
    0x3fd36277,     /* ATAN (0x3fd40...0) */
    0x3fd37f9a,     /* ATAN (0x3fd42...0) */
    0x3fd39cb4,     /* ATAN (0x3fd44...0) */
    0x3fd3b9c7,     /* ATAN (0x3fd46...0) */
    0x3fd3d6d1,     /* ATAN (0x3fd48...0) */
    0x3fd3f3d2,     /* ATAN (0x3fd4a...0) */
    0x3fd410cb,     /* ATAN (0x3fd4c...0) */
    0x3fd42dbc,     /* ATAN (0x3fd4e...0) */
    0x3fd44aa4,     /* ATAN (0x3fd50...0) */
    0x3fd46783,     /* ATAN (0x3fd52...0) */
    0x3fd4845a,     /* ATAN (0x3fd54...0) */
    0x3fd4a128,     /* ATAN (0x3fd56...0) */
    0x3fd4bdee,     /* ATAN (0x3fd58...0) */
    0x3fd4daab,     /* ATAN (0x3fd5a...0) */
    0x3fd4f75f,     /* ATAN (0x3fd5c...0) */
    0x3fd5140a,     /* ATAN (0x3fd5e...0) */
    0x3fd530ad,     /* ATAN (0x3fd60...0) */
    0x3fd54d47,     /* ATAN (0x3fd62...0) */
    0x3fd569d8,     /* ATAN (0x3fd64...0) */
    0x3fd58660,     /* ATAN (0x3fd66...0) */
    0x3fd5a2e0,     /* ATAN (0x3fd68...0) */
    0x3fd5bf56,     /* ATAN (0x3fd6a...0) */
    0x3fd5dbc3,     /* ATAN (0x3fd6c...0) */
    0x3fd5f828,     /* ATAN (0x3fd6e...0) */
    0x3fd61484,     /* ATAN (0x3fd70...0) */
    0x3fd630d6,     /* ATAN (0x3fd72...0) */
    0x3fd64d1f,     /* ATAN (0x3fd74...0) */
    0x3fd66960,     /* ATAN (0x3fd76...0) */
    0x3fd68597,     /* ATAN (0x3fd78...0) */
    0x3fd6a1c5,     /* ATAN (0x3fd7a...0) */
    0x3fd6bdea,     /* ATAN (0x3fd7c...0) */
    0x3fd6da06,     /* ATAN (0x3fd7e...0) */
    0x3fd6f619,     /* ATAN (0x3fd80...0) */
    0x3fd71222,     /* ATAN (0x3fd82...0) */
    0x3fd72e22,     /* ATAN (0x3fd84...0) */
    0x3fd74a19,     /* ATAN (0x3fd86...0) */
    0x3fd76607,     /* ATAN (0x3fd88...0) */
    0x3fd781eb,     /* ATAN (0x3fd8a...0) */
    0x3fd79dc6,     /* ATAN (0x3fd8c...0) */
    0x3fd7b998,     /* ATAN (0x3fd8e...0) */
    0x3fd7d560,     /* ATAN (0x3fd90...0) */
    0x3fd7f11f,     /* ATAN (0x3fd92...0) */
    0x3fd80cd4,     /* ATAN (0x3fd94...0) */
    0x3fd82880,     /* ATAN (0x3fd96...0) */
    0x3fd84422,     /* ATAN (0x3fd98...0) */
    0x3fd85fbb,     /* ATAN (0x3fd9a...0) */
    0x3fd87b4b,     /* ATAN (0x3fd9c...0) */
    0x3fd896d0,     /* ATAN (0x3fd9e...0) */
    0x3fd8b24d,     /* ATAN (0x3fda0...0) */
    0x3fd8cdbf,     /* ATAN (0x3fda2...0) */
    0x3fd8e929,     /* ATAN (0x3fda4...0) */
    0x3fd90488,     /* ATAN (0x3fda6...0) */
    0x3fd91fde,     /* ATAN (0x3fda8...0) */
    0x3fd93b2a,     /* ATAN (0x3fdaa...0) */
    0x3fd9566d,     /* ATAN (0x3fdac...0) */
    0x3fd971a6,     /* ATAN (0x3fdae...0) */
    0x3fd98cd5,     /* ATAN (0x3fdb0...0) */
    0x3fd9a7fa,     /* ATAN (0x3fdb2...0) */
    0x3fd9c316,     /* ATAN (0x3fdb4...0) */
    0x3fd9de28,     /* ATAN (0x3fdb6...0) */
    0x3fd9f930,     /* ATAN (0x3fdb8...0) */
    0x3fda142e,     /* ATAN (0x3fdba...0) */
    0x3fda2f23,     /* ATAN (0x3fdbc...0) */
    0x3fda4a0d,     /* ATAN (0x3fdbe...0) */
    0x3fda64ee,     /* ATAN (0x3fdc0...0) */
    0x3fda7fc5,     /* ATAN (0x3fdc2...0) */
    0x3fda9a92,     /* ATAN (0x3fdc4...0) */
    0x3fdab556,     /* ATAN (0x3fdc6...0) */
    0x3fdad00f,     /* ATAN (0x3fdc8...0) */
    0x3fdaeabe,     /* ATAN (0x3fdca...0) */
    0x3fdb0564,     /* ATAN (0x3fdcc...0) */
    0x3fdb1fff,     /* ATAN (0x3fdce...0) */
    0x3fdb3a91,     /* ATAN (0x3fdd0...0) */
    0x3fdb5518,     /* ATAN (0x3fdd2...0) */
    0x3fdb6f96,     /* ATAN (0x3fdd4...0) */
    0x3fdb8a09,     /* ATAN (0x3fdd6...0) */
    0x3fdba473,     /* ATAN (0x3fdd8...0) */
    0x3fdbbed2,     /* ATAN (0x3fdda...0) */
    0x3fdbd928,     /* ATAN (0x3fddc...0) */
    0x3fdbf373,     /* ATAN (0x3fdde...0) */
    0x3fdc0db4,     /* ATAN (0x3fde0...0) */
    0x3fdc27ec,     /* ATAN (0x3fde2...0) */
    0x3fdc4219,     /* ATAN (0x3fde4...0) */
    0x3fdc5c3c,     /* ATAN (0x3fde6...0) */
    0x3fdc7655,     /* ATAN (0x3fde8...0) */
    0x3fdc9063,     /* ATAN (0x3fdea...0) */
    0x3fdcaa68,     /* ATAN (0x3fdec...0) */
    0x3fdcc462,     /* ATAN (0x3fdee...0) */
    0x3fdcde53,     /* ATAN (0x3fdf0...0) */
    0x3fdcf839,     /* ATAN (0x3fdf2...0) */
    0x3fdd1215,     /* ATAN (0x3fdf4...0) */
    0x3fdd2be7,     /* ATAN (0x3fdf6...0) */
    0x3fdd45ae,     /* ATAN (0x3fdf8...0) */
    0x3fdd5f6c,     /* ATAN (0x3fdfa...0) */
    0x3fdd791f,     /* ATAN (0x3fdfc...0) */
    0x3fdd92c8,     /* ATAN (0x3fdfe...0) */
    0x3fddac67,     /* ATAN (0x3fe00...0) */
    0x3fdddf85,     /* ATAN (0x3fe02...0) */
    0x3fde127b,     /* ATAN (0x3fe04...0) */
    0x3fde4548,     /* ATAN (0x3fe06...0) */
    0x3fde77eb,     /* ATAN (0x3fe08...0) */
    0x3fdeaa65,     /* ATAN (0x3fe0a...0) */
    0x3fdedcb6,     /* ATAN (0x3fe0c...0) */
    0x3fdf0ede,     /* ATAN (0x3fe0e...0) */
    0x3fdf40dd,     /* ATAN (0x3fe10...0) */
    0x3fdf72b2,     /* ATAN (0x3fe12...0) */
    0x3fdfa45d,     /* ATAN (0x3fe14...0) */
    0x3fdfd5e0,     /* ATAN (0x3fe16...0) */
    0x3fe0039c,     /* ATAN (0x3fe18...0) */
    0x3fe01c34,     /* ATAN (0x3fe1a...0) */
    0x3fe034b7,     /* ATAN (0x3fe1c...0) */
    0x3fe04d25,     /* ATAN (0x3fe1e...0) */
    0x3fe0657e,     /* ATAN (0x3fe20...0) */
    0x3fe07dc3,     /* ATAN (0x3fe22...0) */
    0x3fe095f3,     /* ATAN (0x3fe24...0) */
    0x3fe0ae0e,     /* ATAN (0x3fe26...0) */
    0x3fe0c614,     /* ATAN (0x3fe28...0) */
    0x3fe0de05,     /* ATAN (0x3fe2a...0) */
    0x3fe0f5e2,     /* ATAN (0x3fe2c...0) */
    0x3fe10daa,     /* ATAN (0x3fe2e...0) */
    0x3fe1255d,     /* ATAN (0x3fe30...0) */
    0x3fe13cfb,     /* ATAN (0x3fe32...0) */
    0x3fe15485,     /* ATAN (0x3fe34...0) */
    0x3fe16bfa,     /* ATAN (0x3fe36...0) */
    0x3fe1835a,     /* ATAN (0x3fe38...0) */
    0x3fe19aa5,     /* ATAN (0x3fe3a...0) */
    0x3fe1b1dc,     /* ATAN (0x3fe3c...0) */
    0x3fe1c8fe,     /* ATAN (0x3fe3e...0) */
    0x3fe1e00b,     /* ATAN (0x3fe40...0) */
    0x3fe1f704,     /* ATAN (0x3fe42...0) */
    0x3fe20de8,     /* ATAN (0x3fe44...0) */
    0x3fe224b7,     /* ATAN (0x3fe46...0) */
    0x3fe23b71,     /* ATAN (0x3fe48...0) */
    0x3fe25217,     /* ATAN (0x3fe4a...0) */
    0x3fe268a9,     /* ATAN (0x3fe4c...0) */
    0x3fe27f26,     /* ATAN (0x3fe4e...0) */
    0x3fe2958e,     /* ATAN (0x3fe50...0) */
    0x3fe2abe2,     /* ATAN (0x3fe52...0) */
    0x3fe2c221,     /* ATAN (0x3fe54...0) */
    0x3fe2d84c,     /* ATAN (0x3fe56...0) */
    0x3fe2ee62,     /* ATAN (0x3fe58...0) */
    0x3fe30464,     /* ATAN (0x3fe5a...0) */
    0x3fe31a52,     /* ATAN (0x3fe5c...0) */
    0x3fe3302b,     /* ATAN (0x3fe5e...0) */
    0x3fe345f0,     /* ATAN (0x3fe60...0) */
    0x3fe35ba0,     /* ATAN (0x3fe62...0) */
    0x3fe3713d,     /* ATAN (0x3fe64...0) */
    0x3fe386c5,     /* ATAN (0x3fe66...0) */
    0x3fe39c39,     /* ATAN (0x3fe68...0) */
    0x3fe3b198,     /* ATAN (0x3fe6a...0) */
    0x3fe3c6e4,     /* ATAN (0x3fe6c...0) */
    0x3fe3dc1c,     /* ATAN (0x3fe6e...0) */
    0x3fe3f13f,     /* ATAN (0x3fe70...0) */
    0x3fe4064f,     /* ATAN (0x3fe72...0) */
    0x3fe41b4a,     /* ATAN (0x3fe74...0) */
    0x3fe43032,     /* ATAN (0x3fe76...0) */
    0x3fe44506,     /* ATAN (0x3fe78...0) */
    0x3fe459c6,     /* ATAN (0x3fe7a...0) */
    0x3fe46e72,     /* ATAN (0x3fe7c...0) */
    0x3fe4830a,     /* ATAN (0x3fe7e...0) */
    0x3fe4978f,     /* ATAN (0x3fe80...0) */
    0x3fe4ac00,     /* ATAN (0x3fe82...0) */
    0x3fe4c05e,     /* ATAN (0x3fe84...0) */
    0x3fe4d4a8,     /* ATAN (0x3fe86...0) */
    0x3fe4e8de,     /* ATAN (0x3fe88...0) */
    0x3fe4fd01,     /* ATAN (0x3fe8a...0) */
    0x3fe51110,     /* ATAN (0x3fe8c...0) */
    0x3fe5250c,     /* ATAN (0x3fe8e...0) */
    0x3fe538f5,     /* ATAN (0x3fe90...0) */
    0x3fe54cca,     /* ATAN (0x3fe92...0) */
    0x3fe5608d,     /* ATAN (0x3fe94...0) */
    0x3fe5743c,     /* ATAN (0x3fe96...0) */
    0x3fe587d8,     /* ATAN (0x3fe98...0) */
    0x3fe59b60,     /* ATAN (0x3fe9a...0) */
    0x3fe5aed6,     /* ATAN (0x3fe9c...0) */
    0x3fe5c239,     /* ATAN (0x3fe9e...0) */
    0x3fe5d589,     /* ATAN (0x3fea0...0) */
    0x3fe5e8c6,     /* ATAN (0x3fea2...0) */
    0x3fe5fbf0,     /* ATAN (0x3fea4...0) */
    0x3fe60f08,     /* ATAN (0x3fea6...0) */
    0x3fe6220d,     /* ATAN (0x3fea8...0) */
    0x3fe634ff,     /* ATAN (0x3feaa...0) */
    0x3fe647de,     /* ATAN (0x3feac...0) */
    0x3fe65aab,     /* ATAN (0x3feae...0) */
    0x3fe66d66,     /* ATAN (0x3feb0...0) */
    0x3fe6800e,     /* ATAN (0x3feb2...0) */
    0x3fe692a4,     /* ATAN (0x3feb4...0) */
    0x3fe6a527,     /* ATAN (0x3feb6...0) */
    0x3fe6b798,     /* ATAN (0x3feb8...0) */
    0x3fe6c9f7,     /* ATAN (0x3feba...0) */
    0x3fe6dc44,     /* ATAN (0x3febc...0) */
    0x3fe6ee7f,     /* ATAN (0x3febe...0) */
    0x3fe700a7,     /* ATAN (0x3fec0...0) */
    0x3fe712be,     /* ATAN (0x3fec2...0) */
    0x3fe724c3,     /* ATAN (0x3fec4...0) */
    0x3fe736b6,     /* ATAN (0x3fec6...0) */
    0x3fe74897,     /* ATAN (0x3fec8...0) */
    0x3fe75a67,     /* ATAN (0x3feca...0) */
    0x3fe76c24,     /* ATAN (0x3fecc...0) */
    0x3fe77dd1,     /* ATAN (0x3fece...0) */
    0x3fe78f6b,     /* ATAN (0x3fed0...0) */
    0x3fe7a0f4,     /* ATAN (0x3fed2...0) */
    0x3fe7b26c,     /* ATAN (0x3fed4...0) */
    0x3fe7c3d3,     /* ATAN (0x3fed6...0) */
    0x3fe7d528,     /* ATAN (0x3fed8...0) */
    0x3fe7e66c,     /* ATAN (0x3feda...0) */
    0x3fe7f79e,     /* ATAN (0x3fedc...0) */
    0x3fe808c0,     /* ATAN (0x3fede...0) */
    0x3fe819d0,     /* ATAN (0x3fee0...0) */
    0x3fe82ad0,     /* ATAN (0x3fee2...0) */
    0x3fe83bbe,     /* ATAN (0x3fee4...0) */
    0x3fe84c9c,     /* ATAN (0x3fee6...0) */
    0x3fe85d69,     /* ATAN (0x3fee8...0) */
    0x3fe86e25,     /* ATAN (0x3feea...0) */
    0x3fe87ed0,     /* ATAN (0x3feec...0) */
    0x3fe88f6b,     /* ATAN (0x3feee...0) */
    0x3fe89ff5,     /* ATAN (0x3fef0...0) */
    0x3fe8b06f,     /* ATAN (0x3fef2...0) */
    0x3fe8c0d9,     /* ATAN (0x3fef4...0) */
    0x3fe8d132,     /* ATAN (0x3fef6...0) */
    0x3fe8e17a,     /* ATAN (0x3fef8...0) */
    0x3fe8f1b3,     /* ATAN (0x3fefa...0) */
    0x3fe901db,     /* ATAN (0x3fefc...0) */
    0x3fe911f3,     /* ATAN (0x3fefe...0) */
    0x3fe921fb,     /* ATAN (0x3ff00...0) */
    0x3fe941db,     /* ATAN (0x3ff02...0) */
    0x3fe9617b,     /* ATAN (0x3ff04...0) */
    0x3fe980dd,     /* ATAN (0x3ff06...0) */
    0x3fe9a000,     /* ATAN (0x3ff08...0) */
    0x3fe9bee5,     /* ATAN (0x3ff0a...0) */
    0x3fe9dd8d,     /* ATAN (0x3ff0c...0) */
    0x3fe9fbf7,     /* ATAN (0x3ff0e...0) */
    0x3fea1a25,     /* ATAN (0x3ff10...0) */
    0x3fea3817,     /* ATAN (0x3ff12...0) */
    0x3fea55ce,     /* ATAN (0x3ff14...0) */
    0x3fea734a,     /* ATAN (0x3ff16...0) */
    0x3fea908a,     /* ATAN (0x3ff18...0) */
    0x3feaad91,     /* ATAN (0x3ff1a...0) */
    0x3feaca5f,     /* ATAN (0x3ff1c...0) */
    0x3feae6f3,     /* ATAN (0x3ff1e...0) */
    0x3feb034f,     /* ATAN (0x3ff20...0) */
    0x3feb1f72,     /* ATAN (0x3ff22...0) */
    0x3feb3b5e,     /* ATAN (0x3ff24...0) */
    0x3feb5713,     /* ATAN (0x3ff26...0) */
    0x3feb7291,     /* ATAN (0x3ff28...0) */
    0x3feb8dd9,     /* ATAN (0x3ff2a...0) */
    0x3feba8eb,     /* ATAN (0x3ff2c...0) */
    0x3febc3c8,     /* ATAN (0x3ff2e...0) */
    0x3febde70,     /* ATAN (0x3ff30...0) */
    0x3febf8e4,     /* ATAN (0x3ff32...0) */
    0x3fec1324,     /* ATAN (0x3ff34...0) */
    0x3fec2d31,     /* ATAN (0x3ff36...0) */
    0x3fec470a,     /* ATAN (0x3ff38...0) */
    0x3fec60b1,     /* ATAN (0x3ff3a...0) */
    0x3fec7a26,     /* ATAN (0x3ff3c...0) */
    0x3fec936a,     /* ATAN (0x3ff3e...0) */
    0x3fecac7c,     /* ATAN (0x3ff40...0) */
    0x3fecc55d,     /* ATAN (0x3ff42...0) */
    0x3fecde0e,     /* ATAN (0x3ff44...0) */
    0x3fecf690,     /* ATAN (0x3ff46...0) */
    0x3fed0ee2,     /* ATAN (0x3ff48...0) */
    0x3fed2705,     /* ATAN (0x3ff4a...0) */
    0x3fed3ef9,     /* ATAN (0x3ff4c...0) */
    0x3fed56bf,     /* ATAN (0x3ff4e...0) */
    0x3fed6e57,     /* ATAN (0x3ff50...0) */
    0x3fed85c2,     /* ATAN (0x3ff52...0) */
    0x3fed9d01,     /* ATAN (0x3ff54...0) */
    0x3fedb412,     /* ATAN (0x3ff56...0) */
    0x3fedcaf8,     /* ATAN (0x3ff58...0) */
    0x3fede1b2,     /* ATAN (0x3ff5a...0) */
    0x3fedf840,     /* ATAN (0x3ff5c...0) */
    0x3fee0ea4,     /* ATAN (0x3ff5e...0) */
    0x3fee24dd,     /* ATAN (0x3ff60...0) */
    0x3fee3aec,     /* ATAN (0x3ff62...0) */
    0x3fee50d1,     /* ATAN (0x3ff64...0) */
    0x3fee668d,     /* ATAN (0x3ff66...0) */
    0x3fee7c20,     /* ATAN (0x3ff68...0) */
    0x3fee918a,     /* ATAN (0x3ff6a...0) */
    0x3feea6cc,     /* ATAN (0x3ff6c...0) */
    0x3feebbe6,     /* ATAN (0x3ff6e...0) */
    0x3feed0d9,     /* ATAN (0x3ff70...0) */
    0x3feee5a5,     /* ATAN (0x3ff72...0) */
    0x3feefa49,     /* ATAN (0x3ff74...0) */
    0x3fef0ec8,     /* ATAN (0x3ff76...0) */
    0x3fef2320,     /* ATAN (0x3ff78...0) */
    0x3fef3753,     /* ATAN (0x3ff7a...0) */
    0x3fef4b60,     /* ATAN (0x3ff7c...0) */
    0x3fef5f48,     /* ATAN (0x3ff7e...0) */
    0x3fef730b,     /* ATAN (0x3ff80...0) */
    0x3fef86aa,     /* ATAN (0x3ff82...0) */
    0x3fef9a26,     /* ATAN (0x3ff84...0) */
    0x3fefad7d,     /* ATAN (0x3ff86...0) */
    0x3fefc0b1,     /* ATAN (0x3ff88...0) */
    0x3fefd3c2,     /* ATAN (0x3ff8a...0) */
    0x3fefe6b0,     /* ATAN (0x3ff8c...0) */
    0x3feff97c,     /* ATAN (0x3ff8e...0) */
    0x3ff00613,     /* ATAN (0x3ff90...0) */
    0x3ff00f57,     /* ATAN (0x3ff92...0) */
    0x3ff0188a,     /* ATAN (0x3ff94...0) */
    0x3ff021ad,     /* ATAN (0x3ff96...0) */
    0x3ff02abf,     /* ATAN (0x3ff98...0) */
    0x3ff033c1,     /* ATAN (0x3ff9a...0) */
    0x3ff03cb3,     /* ATAN (0x3ff9c...0) */
    0x3ff04595,     /* ATAN (0x3ff9e...0) */
    0x3ff04e67,     /* ATAN (0x3ffa0...0) */
    0x3ff05729,     /* ATAN (0x3ffa2...0) */
    0x3ff05fdc,     /* ATAN (0x3ffa4...0) */
    0x3ff0687f,     /* ATAN (0x3ffa6...0) */
    0x3ff07113,     /* ATAN (0x3ffa8...0) */
    0x3ff07998,     /* ATAN (0x3ffaa...0) */
    0x3ff0820e,     /* ATAN (0x3ffac...0) */
    0x3ff08a75,     /* ATAN (0x3ffae...0) */
    0x3ff092ce,     /* ATAN (0x3ffb0...0) */
    0x3ff09b18,     /* ATAN (0x3ffb2...0) */
    0x3ff0a353,     /* ATAN (0x3ffb4...0) */
    0x3ff0ab80,     /* ATAN (0x3ffb6...0) */
    0x3ff0b39f,     /* ATAN (0x3ffb8...0) */
    0x3ff0bbb0,     /* ATAN (0x3ffba...0) */
    0x3ff0c3b2,     /* ATAN (0x3ffbc...0) */
    0x3ff0cba7,     /* ATAN (0x3ffbe...0) */
    0x3ff0d38f,     /* ATAN (0x3ffc0...0) */
    0x3ff0db68,     /* ATAN (0x3ffc2...0) */
    0x3ff0e335,     /* ATAN (0x3ffc4...0) */
    0x3ff0eaf4,     /* ATAN (0x3ffc6...0) */
    0x3ff0f2a5,     /* ATAN (0x3ffc8...0) */
    0x3ff0fa4a,     /* ATAN (0x3ffca...0) */
    0x3ff101e2,     /* ATAN (0x3ffcc...0) */
    0x3ff1096c,     /* ATAN (0x3ffce...0) */
    0x3ff110eb,     /* ATAN (0x3ffd0...0) */
    0x3ff1185c,     /* ATAN (0x3ffd2...0) */
    0x3ff11fc1,     /* ATAN (0x3ffd4...0) */
    0x3ff12719,     /* ATAN (0x3ffd6...0) */
    0x3ff12e65,     /* ATAN (0x3ffd8...0) */
    0x3ff135a5,     /* ATAN (0x3ffda...0) */
    0x3ff13cd9,     /* ATAN (0x3ffdc...0) */
    0x3ff14401,     /* ATAN (0x3ffde...0) */
    0x3ff14b1d,     /* ATAN (0x3ffe0...0) */
    0x3ff1522e,     /* ATAN (0x3ffe2...0) */
    0x3ff15932,     /* ATAN (0x3ffe4...0) */
    0x3ff1602b,     /* ATAN (0x3ffe6...0) */
    0x3ff16719,     /* ATAN (0x3ffe8...0) */
    0x3ff16dfb,     /* ATAN (0x3ffea...0) */
    0x3ff174d2,     /* ATAN (0x3ffec...0) */
    0x3ff17b9e,     /* ATAN (0x3ffee...0) */
    0x3ff1825f,     /* ATAN (0x3fff0...0) */
    0x3ff18914,     /* ATAN (0x3fff2...0) */
    0x3ff18fbf,     /* ATAN (0x3fff4...0) */
    0x3ff1965f,     /* ATAN (0x3fff6...0) */
    0x3ff19cf5,     /* ATAN (0x3fff8...0) */
    0x3ff1a37f,     /* ATAN (0x3fffa...0) */
    0x3ff1aa00,     /* ATAN (0x3fffc...0) */
    0x3ff1b076,     /* ATAN (0x3fffe...0) */
    0x3ff1b6e1,     /* ATAN (0x40000...0) */
    0x3ff1c39a,     /* ATAN (0x40002...0) */
    0x3ff1d02a,     /* ATAN (0x40004...0) */
    0x3ff1dc92,     /* ATAN (0x40006...0) */
    0x3ff1e8d4,     /* ATAN (0x40008...0) */
    0x3ff1f4ef,     /* ATAN (0x4000a...0) */
    0x3ff200e5,     /* ATAN (0x4000c...0) */
    0x3ff20cb6,     /* ATAN (0x4000e...0) */
    0x3ff21862,     /* ATAN (0x40010...0) */
    0x3ff223eb,     /* ATAN (0x40012...0) */
    0x3ff22f51,     /* ATAN (0x40014...0) */
    0x3ff23a94,     /* ATAN (0x40016...0) */
    0x3ff245b4,     /* ATAN (0x40018...0) */
    0x3ff250b4,     /* ATAN (0x4001a...0) */
    0x3ff25b92,     /* ATAN (0x4001c...0) */
    0x3ff26651,     /* ATAN (0x4001e...0) */
    0x3ff270ef,     /* ATAN (0x40020...0) */
    0x3ff27b6e,     /* ATAN (0x40022...0) */
    0x3ff285ce,     /* ATAN (0x40024...0) */
    0x3ff29010,     /* ATAN (0x40026...0) */
    0x3ff29a33,     /* ATAN (0x40028...0) */
    0x3ff2a43a,     /* ATAN (0x4002a...0) */
    0x3ff2ae24,     /* ATAN (0x4002c...0) */
    0x3ff2b7f1,     /* ATAN (0x4002e...0) */
    0x3ff2c1a2,     /* ATAN (0x40030...0) */
    0x3ff2cb37,     /* ATAN (0x40032...0) */
    0x3ff2d4b2,     /* ATAN (0x40034...0) */
    0x3ff2de11,     /* ATAN (0x40036...0) */
    0x3ff2e757,     /* ATAN (0x40038...0) */
    0x3ff2f082,     /* ATAN (0x4003a...0) */
    0x3ff2f994,     /* ATAN (0x4003c...0) */
    0x3ff3028d,     /* ATAN (0x4003e...0) */
    0x3ff30b6d,     /* ATAN (0x40040...0) */
    0x3ff31435,     /* ATAN (0x40042...0) */
    0x3ff31ce4,     /* ATAN (0x40044...0) */
    0x3ff3257d,     /* ATAN (0x40046...0) */
    0x3ff32dfe,     /* ATAN (0x40048...0) */
    0x3ff33667,     /* ATAN (0x4004a...0) */
    0x3ff33ebb,     /* ATAN (0x4004c...0) */
    0x3ff346f8,     /* ATAN (0x4004e...0) */
    0x3ff34f1f,     /* ATAN (0x40050...0) */
    0x3ff35731,     /* ATAN (0x40052...0) */
    0x3ff35f2d,     /* ATAN (0x40054...0) */
    0x3ff36715,     /* ATAN (0x40056...0) */
    0x3ff36ee7,     /* ATAN (0x40058...0) */
    0x3ff376a6,     /* ATAN (0x4005a...0) */
    0x3ff37e50,     /* ATAN (0x4005c...0) */
    0x3ff385e7,     /* ATAN (0x4005e...0) */
    0x3ff38d6a,     /* ATAN (0x40060...0) */
    0x3ff394da,     /* ATAN (0x40062...0) */
    0x3ff39c37,     /* ATAN (0x40064...0) */
    0x3ff3a381,     /* ATAN (0x40066...0) */
    0x3ff3aab9,     /* ATAN (0x40068...0) */
    0x3ff3b1df,     /* ATAN (0x4006a...0) */
    0x3ff3b8f3,     /* ATAN (0x4006c...0) */
    0x3ff3bff5,     /* ATAN (0x4006e...0) */
    0x3ff3c6e6,     /* ATAN (0x40070...0) */
    0x3ff3cdc6,     /* ATAN (0x40072...0) */
    0x3ff3d494,     /* ATAN (0x40074...0) */
    0x3ff3db52,     /* ATAN (0x40076...0) */
    0x3ff3e200,     /* ATAN (0x40078...0) */
    0x3ff3e89e,     /* ATAN (0x4007a...0) */
    0x3ff3ef2b,     /* ATAN (0x4007c...0) */
    0x3ff3f5a9,     /* ATAN (0x4007e...0) */
    0x3ff3fc17,     /* ATAN (0x40080...0) */
    0x3ff40276,     /* ATAN (0x40082...0) */
    0x3ff408c5,     /* ATAN (0x40084...0) */
    0x3ff40f06,     /* ATAN (0x40086...0) */
    0x3ff41538,     /* ATAN (0x40088...0) */
    0x3ff41b5b,     /* ATAN (0x4008a...0) */
    0x3ff42170,     /* ATAN (0x4008c...0) */
    0x3ff42777,     /* ATAN (0x4008e...0) */
    0x3ff42d70,     /* ATAN (0x40090...0) */
    0x3ff4335b,     /* ATAN (0x40092...0) */
    0x3ff43938,     /* ATAN (0x40094...0) */
    0x3ff43f08,     /* ATAN (0x40096...0) */
    0x3ff444cb,     /* ATAN (0x40098...0) */
    0x3ff44a80,     /* ATAN (0x4009a...0) */
    0x3ff45029,     /* ATAN (0x4009c...0) */
    0x3ff455c5,     /* ATAN (0x4009e...0) */
    0x3ff45b54,     /* ATAN (0x400a0...0) */
    0x3ff460d7,     /* ATAN (0x400a2...0) */
    0x3ff4664d,     /* ATAN (0x400a4...0) */
    0x3ff46bb8,     /* ATAN (0x400a6...0) */
    0x3ff47116,     /* ATAN (0x400a8...0) */
    0x3ff47669,     /* ATAN (0x400aa...0) */
    0x3ff47bb0,     /* ATAN (0x400ac...0) */
    0x3ff480eb,     /* ATAN (0x400ae...0) */
    0x3ff4861b,     /* ATAN (0x400b0...0) */
    0x3ff48b3f,     /* ATAN (0x400b2...0) */
    0x3ff49059,     /* ATAN (0x400b4...0) */
    0x3ff49568,     /* ATAN (0x400b6...0) */
    0x3ff49a6b,     /* ATAN (0x400b8...0) */
    0x3ff49f64,     /* ATAN (0x400ba...0) */
    0x3ff4a453,     /* ATAN (0x400bc...0) */
    0x3ff4a937,     /* ATAN (0x400be...0) */
    0x3ff4ae10,     /* ATAN (0x400c0...0) */
    0x3ff4b2e0,     /* ATAN (0x400c2...0) */
    0x3ff4b7a5,     /* ATAN (0x400c4...0) */
    0x3ff4bc61,     /* ATAN (0x400c6...0) */
    0x3ff4c112,     /* ATAN (0x400c8...0) */
    0x3ff4c5ba,     /* ATAN (0x400ca...0) */
    0x3ff4ca58,     /* ATAN (0x400cc...0) */
    0x3ff4ceed,     /* ATAN (0x400ce...0) */
    0x3ff4d378,     /* ATAN (0x400d0...0) */
    0x3ff4d7fa,     /* ATAN (0x400d2...0) */
    0x3ff4dc73,     /* ATAN (0x400d4...0) */
    0x3ff4e0e3,     /* ATAN (0x400d6...0) */
    0x3ff4e54a,     /* ATAN (0x400d8...0) */
    0x3ff4e9a8,     /* ATAN (0x400da...0) */
    0x3ff4edfd,     /* ATAN (0x400dc...0) */
    0x3ff4f249,     /* ATAN (0x400de...0) */
    0x3ff4f68d,     /* ATAN (0x400e0...0) */
    0x3ff4fac9,     /* ATAN (0x400e2...0) */
    0x3ff4fefc,     /* ATAN (0x400e4...0) */
    0x3ff50327,     /* ATAN (0x400e6...0) */
    0x3ff5074a,     /* ATAN (0x400e8...0) */
    0x3ff50b64,     /* ATAN (0x400ea...0) */
    0x3ff50f77,     /* ATAN (0x400ec...0) */
    0x3ff51382,     /* ATAN (0x400ee...0) */
    0x3ff51784,     /* ATAN (0x400f0...0) */
    0x3ff51b80,     /* ATAN (0x400f2...0) */
    0x3ff51f73,     /* ATAN (0x400f4...0) */
    0x3ff5235f,     /* ATAN (0x400f6...0) */
    0x3ff52744,     /* ATAN (0x400f8...0) */
    0x3ff52b21,     /* ATAN (0x400fa...0) */
    0x3ff52ef6,     /* ATAN (0x400fc...0) */
    0x3ff532c5,     /* ATAN (0x400fe...0) */
    0x3ff5368c,     /* ATAN (0x40100...0) */
    0x3ff53e06,     /* ATAN (0x40102...0) */
    0x3ff54563,     /* ATAN (0x40104...0) */
    0x3ff54ca6,     /* ATAN (0x40106...0) */
    0x3ff553ce,     /* ATAN (0x40108...0) */
    0x3ff55adc,     /* ATAN (0x4010a...0) */
    0x3ff561d0,     /* ATAN (0x4010c...0) */
    0x3ff568ac,     /* ATAN (0x4010e...0) */
    0x3ff56f6f,     /* ATAN (0x40110...0) */
    0x3ff5761a,     /* ATAN (0x40112...0) */
    0x3ff57cad,     /* ATAN (0x40114...0) */
    0x3ff5832a,     /* ATAN (0x40116...0) */
    0x3ff58990,     /* ATAN (0x40118...0) */
    0x3ff58fe0,     /* ATAN (0x4011a...0) */
    0x3ff5961a,     /* ATAN (0x4011c...0) */
    0x3ff59c40,     /* ATAN (0x4011e...0) */
    0x3ff5a250,     /* ATAN (0x40120...0) */
    0x3ff5a84c,     /* ATAN (0x40122...0) */
    0x3ff5ae34,     /* ATAN (0x40124...0) */
    0x3ff5b408,     /* ATAN (0x40126...0) */
    0x3ff5b9c9,     /* ATAN (0x40128...0) */
    0x3ff5bf77,     /* ATAN (0x4012a...0) */
    0x3ff5c512,     /* ATAN (0x4012c...0) */
    0x3ff5ca9c,     /* ATAN (0x4012e...0) */
    0x3ff5d013,     /* ATAN (0x40130...0) */
    0x3ff5d579,     /* ATAN (0x40132...0) */
    0x3ff5dace,     /* ATAN (0x40134...0) */
    0x3ff5e012,     /* ATAN (0x40136...0) */
    0x3ff5e545,     /* ATAN (0x40138...0) */
    0x3ff5ea68,     /* ATAN (0x4013a...0) */
    0x3ff5ef7b,     /* ATAN (0x4013c...0) */
    0x3ff5f47f,     /* ATAN (0x4013e...0) */
    0x3ff5f973,     /* ATAN (0x40140...0) */
    0x3ff5fe57,     /* ATAN (0x40142...0) */
    0x3ff6032d,     /* ATAN (0x40144...0) */
    0x3ff607f5,     /* ATAN (0x40146...0) */
    0x3ff60cad,     /* ATAN (0x40148...0) */
    0x3ff61158,     /* ATAN (0x4014a...0) */
    0x3ff615f5,     /* ATAN (0x4014c...0) */
    0x3ff61a84,     /* ATAN (0x4014e...0) */
    0x3ff61f06,     /* ATAN (0x40150...0) */
    0x3ff6237b,     /* ATAN (0x40152...0) */
    0x3ff627e3,     /* ATAN (0x40154...0) */
    0x3ff62c3e,     /* ATAN (0x40156...0) */
    0x3ff6308c,     /* ATAN (0x40158...0) */
    0x3ff634ce,     /* ATAN (0x4015a...0) */
    0x3ff63904,     /* ATAN (0x4015c...0) */
    0x3ff63d2e,     /* ATAN (0x4015e...0) */
    0x3ff6414d,     /* ATAN (0x40160...0) */
    0x3ff64560,     /* ATAN (0x40162...0) */
    0x3ff64967,     /* ATAN (0x40164...0) */
    0x3ff64d63,     /* ATAN (0x40166...0) */
    0x3ff65155,     /* ATAN (0x40168...0) */
    0x3ff6553b,     /* ATAN (0x4016a...0) */
    0x3ff65917,     /* ATAN (0x4016c...0) */
    0x3ff65ce9,     /* ATAN (0x4016e...0) */
    0x3ff660b0,     /* ATAN (0x40170...0) */
    0x3ff6646d,     /* ATAN (0x40172...0) */
    0x3ff66820,     /* ATAN (0x40174...0) */
    0x3ff66bc9,     /* ATAN (0x40176...0) */
    0x3ff66f68,     /* ATAN (0x40178...0) */
    0x3ff672fe,     /* ATAN (0x4017a...0) */
    0x3ff6768b,     /* ATAN (0x4017c...0) */
    0x3ff67a0e,     /* ATAN (0x4017e...0) */
    0x3ff67d88,     /* ATAN (0x40180...0) */
    0x3ff680f9,     /* ATAN (0x40182...0) */
    0x3ff68461,     /* ATAN (0x40184...0) */
    0x3ff687c1,     /* ATAN (0x40186...0) */
    0x3ff68b18,     /* ATAN (0x40188...0) */
    0x3ff68e67,     /* ATAN (0x4018a...0) */
    0x3ff691ad,     /* ATAN (0x4018c...0) */
    0x3ff694eb,     /* ATAN (0x4018e...0) */
    0x3ff69821,     /* ATAN (0x40190...0) */
    0x3ff69b4f,     /* ATAN (0x40192...0) */
    0x3ff69e75,     /* ATAN (0x40194...0) */
    0x3ff6a193,     /* ATAN (0x40196...0) */
    0x3ff6a4aa,     /* ATAN (0x40198...0) */
    0x3ff6a7b9,     /* ATAN (0x4019a...0) */
    0x3ff6aac1,     /* ATAN (0x4019c...0) */
    0x3ff6adc1,     /* ATAN (0x4019e...0) */
    0x3ff6b0ba,     /* ATAN (0x401a0...0) */
    0x3ff6b3ad,     /* ATAN (0x401a2...0) */
    0x3ff6b698,     /* ATAN (0x401a4...0) */
    0x3ff6b97c,     /* ATAN (0x401a6...0) */
    0x3ff6bc59,     /* ATAN (0x401a8...0) */
    0x3ff6bf30,     /* ATAN (0x401aa...0) */
    0x3ff6c200,     /* ATAN (0x401ac...0) */
    0x3ff6c4c9,     /* ATAN (0x401ae...0) */
    0x3ff6c78c,     /* ATAN (0x401b0...0) */
    0x3ff6ca49,     /* ATAN (0x401b2...0) */
    0x3ff6ccff,     /* ATAN (0x401b4...0) */
    0x3ff6cfaf,     /* ATAN (0x401b6...0) */
    0x3ff6d259,     /* ATAN (0x401b8...0) */
    0x3ff6d4fd,     /* ATAN (0x401ba...0) */
    0x3ff6d79b,     /* ATAN (0x401bc...0) */
    0x3ff6da33,     /* ATAN (0x401be...0) */
    0x3ff6dcc5,     /* ATAN (0x401c0...0) */
    0x3ff6df51,     /* ATAN (0x401c2...0) */
    0x3ff6e1d8,     /* ATAN (0x401c4...0) */
    0x3ff6e45a,     /* ATAN (0x401c6...0) */
    0x3ff6e6d5,     /* ATAN (0x401c8...0) */
    0x3ff6e94c,     /* ATAN (0x401ca...0) */
    0x3ff6ebbd,     /* ATAN (0x401cc...0) */
    0x3ff6ee28,     /* ATAN (0x401ce...0) */
    0x3ff6f08f,     /* ATAN (0x401d0...0) */
    0x3ff6f2f0,     /* ATAN (0x401d2...0) */
    0x3ff6f54c,     /* ATAN (0x401d4...0) */
    0x3ff6f7a3,     /* ATAN (0x401d6...0) */
    0x3ff6f9f5,     /* ATAN (0x401d8...0) */
    0x3ff6fc42,     /* ATAN (0x401da...0) */
    0x3ff6fe8a,     /* ATAN (0x401dc...0) */
    0x3ff700ce,     /* ATAN (0x401de...0) */
    0x3ff7030c,     /* ATAN (0x401e0...0) */
    0x3ff70547,     /* ATAN (0x401e2...0) */
    0x3ff7077c,     /* ATAN (0x401e4...0) */
    0x3ff709ad,     /* ATAN (0x401e6...0) */
    0x3ff70bd9,     /* ATAN (0x401e8...0) */
    0x3ff70e01,     /* ATAN (0x401ea...0) */
    0x3ff71024,     /* ATAN (0x401ec...0) */
    0x3ff71243,     /* ATAN (0x401ee...0) */
    0x3ff7145e,     /* ATAN (0x401f0...0) */
    0x3ff71675,     /* ATAN (0x401f2...0) */
    0x3ff71887,     /* ATAN (0x401f4...0) */
    0x3ff71a95,     /* ATAN (0x401f6...0) */
    0x3ff71c9f,     /* ATAN (0x401f8...0) */
    0x3ff71ea5,     /* ATAN (0x401fa...0) */
    0x3ff720a7,     /* ATAN (0x401fc...0) */
    0x3ff722a5,     /* ATAN (0x401fe...0) */
    0x3ff7249f      /* ATAN (0x40200...0) */
};

const DB constarctg_lo[] = {
    3.6012520712375104849e-08,     /* мл. ч. ATAN (0x3fc00...0) */
    3.5701969264252209128e-08,     /* мл. ч. ATAN (0x3fc02...0) */
    4.0977771049528955727e-08,     /* мл. ч. ATAN (0x3fc04...0) */
    5.0149084059344895620e-08,     /* мл. ч. ATAN (0x3fc06...0) */
    6.1527623817934386640e-08,     /* мл. ч. ATAN (0x3fc08...0) */
    7.3427679880897114205e-08,     /* мл. ч. ATAN (0x3fc0a...0) */
    8.4166132264926403777e-08,     /* мл. ч. ATAN (0x3fc0c...0) */
    9.2062467808634638730e-08,     /* мл. ч. ATAN (0x3fc0e...0) */
    9.5438796464118434407e-08,     /* мл. ч. ATAN (0x3fc10...0) */
    9.2619867518909568331e-08,     /* мл. ч. ATAN (0x3fc12...0) */
    8.1933085747961027665e-08,     /* мл. ч. ATAN (0x3fc14...0) */
    6.1708527495320907670e-08,     /* мл. ч. ATAN (0x3fc16...0) */
    3.0278956685150279004e-08,     /* мл. ч. ATAN (0x3fc18...0) */
    1.0518913031252578686e-07,     /* мл. ч. ATAN (0x3fc1a...0) */
    4.6358656109002400974e-08,     /* мл. ч. ATAN (0x3fc1c...0) */
    9.0547035195102652125e-08,     /* мл. ч. ATAN (0x3fc1e...0) */
    1.1688865094987086778e-07,     /* мл. ч. ATAN (0x3fc20...0) */
    4.5206533159156064940e-09,     /* мл. ч. ATAN (0x3fc22...0) */
    1.0942013258258568770e-07,     /* мл. ч. ATAN (0x3fc24...0) */
    7.2311239385241169787e-08,     /* мл. ч. ATAN (0x3fc26...0) */
    1.0758095646865345412e-08,     /* мл. ч. ATAN (0x3fc28...0) */
    4.2327651735142297593e-08,     /* мл. ч. ATAN (0x3fc2a...0) */
    4.6171122647190543981e-08,     /* мл. ч. ATAN (0x3fc2c...0) */
    2.0651871873990538833e-08,     /* мл. ч. ATAN (0x3fc2e...0) */
    8.3345426537953552760e-08,     /* мл. ч. ATAN (0x3fc30...0) */
    1.1341162380333295890e-07,     /* мл. ч. ATAN (0x3fc32...0) */
    1.0922249451621372047e-07,     /* мл. ч. ATAN (0x3fc34...0) */
    6.9152988565975648634e-08,     /* мл. ч. ATAN (0x3fc36...0) */
    1.1079027927262953066e-07,     /* мл. ч. ATAN (0x3fc38...0) */
    1.1330590949505376663e-07,     /* мл. ч. ATAN (0x3fc3a...0) */
    7.5083674967662697822e-08,     /* мл. ч. ATAN (0x3fc3c...0) */
    1.1371963890819761151e-07,     /* мл. ч. ATAN (0x3fc3e...0) */
    1.0839427789636621437e-07,     /* мл. ч. ATAN (0x3fc40...0) */
    5.7500364980091180688e-08,     /* мл. ч. ATAN (0x3fc42...0) */
    7.8642984052069450807e-08,     /* мл. ч. ATAN (0x3fc44...0) */
    5.1011675496378780402e-08,     /* мл. ч. ATAN (0x3fc46...0) */
    9.2217608612684121141e-08,     /* мл. ч. ATAN (0x3fc48...0) */
    8.1456437556068748670e-08,     /* мл. ч. ATAN (0x3fc4a...0) */
    1.7136184053956143666e-08,     /* мл. ч. ATAN (0x3fc4c...0) */
    1.6877251392060976113e-08,     /* мл. ч. ATAN (0x3fc4e...0) */
    7.9093859219904891564e-08,     /* мл. ч. ATAN (0x3fc50...0) */
    8.2994057378780150760e-08,     /* мл. ч. ATAN (0x3fc52...0) */
    2.6998318751922780254e-08,     /* мл. ч. ATAN (0x3fc54...0) */
    2.8739552933535501500e-08,     /* мл. ч. ATAN (0x3fc56...0) */
    8.6644540716429318833e-08,     /* мл. ч. ATAN (0x3fc58...0) */
    7.9933947601181262776e-08,     /* мл. ч. ATAN (0x3fc5a...0) */
    7.0409163265844037493e-09,     /* мл. ч. ATAN (0x3fc5c...0) */
    1.0482036976882612246e-07,     /* мл. ч. ATAN (0x3fc5e...0) */
    1.4083997353709246607e-08,     /* мл. ч. ATAN (0x3fc60...0) */
    9.0902453302015538887e-08,     /* мл. ч. ATAN (0x3fc62...0) */
    9.5303184676552148399e-08,     /* мл. ч. ATAN (0x3fc64...0) */
    2.5735471261925266061e-08,     /* мл. ч. ATAN (0x3fc66...0) */
    1.1907043850730760449e-07,     /* мл. ч. ATAN (0x3fc68...0) */
    1.6136043532001203559e-08,     /* мл. ч. ATAN (0x3fc6a...0) */
    7.3019272963131944357e-08,     /* мл. ч. ATAN (0x3fc6c...0) */
    4.9763970574034536353e-08,     /* мл. ч. ATAN (0x3fc6e...0) */
    6.4045166305171624799e-08,     /* мл. ч. ATAN (0x3fc70...0) */
    1.1433193059318429773e-07,     /* мл. ч. ATAN (0x3fc72...0) */
    7.9887386820393653989e-08,     /* мл. ч. ATAN (0x3fc74...0) */
    7.8396592333795475318e-08,     /* мл. ч. ATAN (0x3fc76...0) */
    1.0833868207634367642e-07,     /* мл. ч. ATAN (0x3fc78...0) */
    4.8986880787384336883e-08,     /* мл. ч. ATAN (0x3fc7a...0) */
    1.8036383772074497631e-08,     /* мл. ч. ATAN (0x3fc7c...0) */
    1.3976500282592281824e-08,     /* мл. ч. ATAN (0x3fc7e...0) */
    3.5299955018792275961e-08,     /* мл. ч. ATAN (0x3fc80...0) */
    8.0502899994244980585e-08,     /* мл. ч. ATAN (0x3fc82...0) */
    2.8875636767509011757e-08,     /* мл. ч. ATAN (0x3fc84...0) */
    1.1733978634317585371e-07,     /* мл. ч. ATAN (0x3fc86...0) */
    1.0598327393004307986e-07,     /* мл. ч. ATAN (0x3fc88...0) */
    1.1252536732580682312e-07,     /* мл. ч. ATAN (0x3fc8a...0) */
    1.6270240607824137521e-08,     /* мл. ч. ATAN (0x3fc8c...0) */
    5.4153433246999500410e-08,     /* мл. ч. ATAN (0x3fc8e...0) */
    1.0548612407825955871e-07,     /* мл. ч. ATAN (0x3fc90...0) */
    4.9583011143115073034e-08,     /* мл. ч. ATAN (0x3fc92...0) */
    4.1809018963751096494e-09,     /* мл. ч. ATAN (0x3fc94...0) */
    8.7020145124550872560e-08,     /* мл. ч. ATAN (0x3fc96...0) */
    5.8216773228177655610e-08,     /* мл. ч. ATAN (0x3fc98...0) */
    3.5518250375755727666e-08,     /* мл. ч. ATAN (0x3fc9a...0) */
    1.7466325064358292149e-08,     /* мл. ч. ATAN (0x3fc9c...0) */
    2.6063303485207353429e-09,     /* мл. ч. ATAN (0x3fc9e...0) */
    1.0869648398340394579e-07,     /* мл. ч. ATAN (0x3fca0...0) */
    9.5870740728228049390e-08,     /* мл. ч. ATAN (0x3fca2...0) */
    8.1894540071601461591e-08,     /* мл. ч. ATAN (0x3fca4...0) */
    6.5327658363029958449e-08,     /* мл. ч. ATAN (0x3fca6...0) */
    4.4733508612237763226e-08,     /* мл. ч. ATAN (0x3fca8...0) */
    1.8679150651529933058e-08,     /* мл. ч. ATAN (0x3fcaa...0) */
    1.0494459076267886194e-07,     /* мл. ч. ATAN (0x3fcac...0) */
    6.3685633463571241008e-08,     /* мл. ч. ATAN (0x3fcae...0) */
    1.2689628716261584983e-08,     /* мл. ч. ATAN (0x3fcb0...0) */
    6.9747613091519444000e-08,     /* мл. ч. ATAN (0x3fcb2...0) */
    1.1423574039761717352e-07,     /* мл. ч. ATAN (0x3fcb4...0) */
    2.5533870425639880312e-08,     /* мл. ч. ATAN (0x3fcb6...0) */
    4.0653447158915150430e-08,     /* мл. ч. ATAN (0x3fcb8...0) */
    3.8981770938694894653e-08,     /* мл. ч. ATAN (0x3fcba...0) */
    1.9119166050881368547e-08,     /* мл. ч. ATAN (0x3fcbc...0) */
    9.8878990022811637250e-08,     /* мл. ч. ATAN (0x3fcbe...0) */
    3.8450484630055709627e-08,     /* мл. ч. ATAN (0x3fcc0...0) */
    7.4863811874905762096e-08,     /* мл. ч. ATAN (0x3fcc2...0) */
    8.7525036166987309086e-08,     /* мл. ч. ATAN (0x3fcc4...0) */
    7.5053291475489104570e-08,     /* мл. ч. ATAN (0x3fcc6...0) */
    3.6071500640480727183e-08,     /* мл. ч. ATAN (0x3fcc8...0) */
    8.8415673698758959981e-08,     /* мл. ч. ATAN (0x3fcca...0) */
    1.1150704791951122460e-07,     /* мл. ч. ATAN (0x3fccc...0) */
    1.0397996505758771845e-07,     /* мл. ч. ATAN (0x3fcce...0) */
    6.4472590316552282122e-08,     /* мл. ч. ATAN (0x3fcd0...0) */
    1.1083621032618539271e-07,     /* мл. ч. ATAN (0x3fcd2...0) */
    3.2980832789597865120e-09,     /* мл. ч. ATAN (0x3fcd4...0) */
    9.8135763588762561647e-08,     /* мл. ч. ATAN (0x3fcd6...0) */
    3.6374924997640947770e-08,     /* мл. ч. ATAN (0x3fcd8...0) */
    5.5091553712602254994e-08,     /* мл. ч. ATAN (0x3fcda...0) */
    3.3737639990439328475e-08,     /* мл. ч. ATAN (0x3fcdc...0) */
    9.0187633795587387735e-08,     /* мл. ч. ATAN (0x3fcde...0) */
    1.0390129441383392096e-07,     /* мл. ч. ATAN (0x3fce0...0) */
    7.3551566834394083973e-08,     /* мл. ч. ATAN (0x3fce2...0) */
    1.1702458939255482113e-07,     /* мл. ч. ATAN (0x3fce4...0) */
    1.1379183267290668086e-07,     /* мл. ч. ATAN (0x3fce6...0) */
    6.2537975630216791923e-08,     /* мл. ч. ATAN (0x3fce8...0) */
    8.1160912970937197280e-08,     /* мл. ч. ATAN (0x3fcea...0) */
    4.9143893795377210814e-08,     /* мл. ч. ATAN (0x3fcec...0) */
    8.4392687008384167925e-08,     /* мл. ч. ATAN (0x3fcee...0) */
    6.6398430236848883634e-08,     /* мл. ч. ATAN (0x3fcf0...0) */
    1.1307479506657406172e-07,     /* мл. ч. ATAN (0x3fcf2...0) */
    1.0392083578604706497e-07,     /* мл. ч. ATAN (0x3fcf4...0) */
    3.7648864898880359916e-08,     /* мл. ч. ATAN (0x3fcf6...0) */
    3.2184459897154839228e-08,     /* мл. ч. ATAN (0x3fcf8...0) */
    8.6247890846498101051e-08,     /* мл. ч. ATAN (0x3fcfa...0) */
    7.9354126986076627107e-08,     /* мл. ч. ATAN (0x3fcfc...0) */
    1.0231422343558899726e-08,     /* мл. ч. ATAN (0x3fcfe...0) */
    1.1603061171276583248e-07,     /* мл. ч. ATAN (0x3fd00...0) */
    1.2878714678871882756e-08,     /* мл. ч. ATAN (0x3fd02...0) */
    1.1746462214234774163e-07,     /* мл. ч. ATAN (0x3fd04...0) */
    6.2163206495972492135e-08,     /* мл. ч. ATAN (0x3fd06...0) */
    7.5460401796580907508e-08,     /* мл. ч. ATAN (0x3fd08...0) */
    2.8279072323965410411e-08,     /* мл. ч. ATAN (0x3fd0a...0) */
    1.4923492935620658195e-07,     /* мл. ч. ATAN (0x3fd0c...0) */
    1.9017167830206504128e-07,     /* мл. ч. ATAN (0x3fd0e...0) */
    1.4141692452321744644e-07,     /* мл. ч. ATAN (0x3fd10...0) */
    2.3178233639469869496e-07,     /* мл. ч. ATAN (0x3fd12...0) */
    2.1330806561748350865e-07,     /* мл. ч. ATAN (0x3fd14...0) */
    7.6518636711922106017e-08,     /* мл. ч. ATAN (0x3fd16...0) */
    5.0423093793330234856e-08,     /* мл. ч. ATAN (0x3fd18...0) */
    1.2567798374736085004e-07,     /* мл. ч. ATAN (0x3fd1a...0) */
    5.4587492228165571776e-08,     /* мл. ч. ATAN (0x3fd1c...0) */
    6.6359311598351147767e-08,     /* мл. ч. ATAN (0x3fd1e...0) */
    1.5184902891478687190e-07,     /* мл. ч. ATAN (0x3fd20...0) */
    6.3560245893110688955e-08,     /* мл. ч. ATAN (0x3fd22...0) */
    3.0900430870376950648e-08,     /* мл. ч. ATAN (0x3fd24...0) */
    4.4925290871874896990e-08,     /* мл. ч. ATAN (0x3fd26...0) */
    9.6757454818473837325e-08,     /* мл. ч. ATAN (0x3fd28...0) */
    1.7758657238804361898e-07,     /* мл. ч. ATAN (0x3fd2a...0) */
    4.0250828552932224305e-08,     /* мл. ч. ATAN (0x3fd2c...0) */
    1.5291134842546865819e-07,     /* мл. ч. ATAN (0x3fd2e...0) */
    3.0122226809686111420e-08,     /* мл. ч. ATAN (0x3fd30...0) */
    1.4017923912367764672e-07,     /* мл. ч. ATAN (0x3fd32...0) */
    2.3618986067007929520e-07,     /* мл. ч. ATAN (0x3fd34...0) */
    7.1329071818685914897e-08,     /* мл. ч. ATAN (0x3fd36...0) */
    1.1409515811108089800e-07,     /* мл. ч. ATAN (0x3fd38...0) */
    1.1779829339927677071e-07,     /* мл. ч. ATAN (0x3fd3a...0) */
    7.4234908974657364452e-08,     /* мл. ч. ATAN (0x3fd3c...0) */
    2.1368774129248350769e-07,     /* мл. ч. ATAN (0x3fd3e...0) */
    5.1251558319623056095e-08,     /* мл. ч. ATAN (0x3fd40...0) */
    5.5763251057117510896e-08,     /* мл. ч. ATAN (0x3fd42...0) */
    2.1929039182876393991e-07,     /* мл. ч. ATAN (0x3fd44...0) */
    5.7131262294572385202e-08,     /* мл. ч. ATAN (0x3fd46...0) */
    3.8326351218755392049e-08,     /* мл. ч. ATAN (0x3fd48...0) */
    1.5514689819425520471e-07,     /* мл. ч. ATAN (0x3fd4a...0) */
    1.6151348628409052809e-07,     /* мл. ч. ATAN (0x3fd4c...0) */
    4.9833209001196037046e-08,     /* мл. ч. ATAN (0x3fd4e...0) */
    5.0999674353559015577e-08,     /* мл. ч. ATAN (0x3fd50...0) */
    1.5755584598657861938e-07,     /* мл. ч. ATAN (0x3fd52...0) */
    1.2369403786124677260e-07,     /* мл. ч. ATAN (0x3fd54...0) */
    1.8051164160247563205e-07,     /* мл. ч. ATAN (0x3fd56...0) */
    8.2336795535112381807e-08,     /* мл. ч. ATAN (0x3fd58...0) */
    6.0402681964637862832e-08,     /* мл. ч. ATAN (0x3fd5a...0) */
    1.0759176621305370404e-07,     /* мл. ч. ATAN (0x3fd5c...0) */
    2.1685434756146071498e-07,     /* мл. ч. ATAN (0x3fd5e...0) */
    1.4278994752463183879e-07,     /* мл. ч. ATAN (0x3fd60...0) */
    1.1690301010198596591e-07,     /* мл. ч. ATAN (0x3fd62...0) */
    1.3234712302471189242e-07,     /* мл. ч. ATAN (0x3fd64...0) */
    1.8234355105037469603e-07,     /* мл. ч. ATAN (0x3fd66...0) */
    2.1762606731659822342e-08,     /* мл. ч. ATAN (0x3fd68...0) */
    1.2079791240653575221e-07,     /* мл. ч. ATAN (0x3fd6a...0) */
    2.3445486692304428937e-07,     /* мл. ч. ATAN (0x3fd6c...0) */
    1.1780632006534579931e-07,     /* мл. ч. ATAN (0x3fd6e...0) */
    2.8296637026176812869e-09,     /* мл. ч. ATAN (0x3fd70...0) */
    1.2156960239361550521e-07,     /* мл. ч. ATAN (0x3fd72...0) */
    2.2930091989090763423e-07,     /* мл. ч. ATAN (0x3fd74...0) */
    8.1365557989048326214e-08,     /* мл. ч. ATAN (0x3fd76...0) */
    1.4842827045026130923e-07,     /* мл. ч. ATAN (0x3fd78...0) */
    1.8596506103050320572e-07,     /* мл. ч. ATAN (0x3fd7a...0) */
    1.8793740857431398832e-07,     /* мл. ч. ATAN (0x3fd7c...0) */
    1.4837359269395244031e-07,     /* мл. ч. ATAN (0x3fd7e...0) */
    6.1368594681333420064e-08,     /* мл. ч. ATAN (0x3fd80...0) */
    1.5950257369210565022e-07,     /* мл. ч. ATAN (0x3fd82...0) */
    1.9858502273358382667e-07,     /* мл. ч. ATAN (0x3fd84...0) */
    1.7291039551186463242e-07,     /* мл. ч. ATAN (0x3fd86...0) */
    7.6839413162375310113e-08,     /* мл. ч. ATAN (0x3fd88...0) */
    1.4321752551326335988e-07,     /* мл. ч. ATAN (0x3fd8a...0) */
    1.2811905231243675376e-07,     /* мл. ч. ATAN (0x3fd8c...0) */
    2.6102795474205863375e-08,     /* мл. ч. ATAN (0x3fd8e...0) */
    7.0211910471923655038e-08,     /* мл. ч. ATAN (0x3fd90...0) */
    1.6718036917207188346e-08,     /* мл. ч. ATAN (0x3fd92...0) */
    9.8795479382063635647e-08,     /* мл. ч. ATAN (0x3fd94...0) */
    7.2846751981287996213e-08,     /* мл. ч. ATAN (0x3fd96...0) */
    1.7217675238103498837e-07,     /* мл. ч. ATAN (0x3fd98...0) */
    1.5331829964955684450e-07,     /* мл. ч. ATAN (0x3fd9a...0) */
    1.1287722514616988456e-08,     /* мл. ч. ATAN (0x3fd9c...0) */
    2.1800328615466192451e-07,     /* мл. ч. ATAN (0x3fd9e...0) */
    5.3354982955585195485e-08,     /* мл. ч. ATAN (0x3fda0...0) */
    2.2780874391497305959e-07,     /* мл. ч. ATAN (0x3fda2...0) */
    2.1383327571081677010e-08,     /* мл. ч. ATAN (0x3fda4...0) */
    1.4467310427133517653e-07,     /* мл. ч. ATAN (0x3fda6...0) */
    1.1624351804829057666e-07,     /* мл. ч. ATAN (0x3fda8...0) */
    1.6997954841010175622e-07,     /* мл. ч. ATAN (0x3fdaa...0) */
    6.2992640836905595856e-08,     /* мл. ч. ATAN (0x3fdac...0) */
    2.9294846258936636440e-08,     /* мл. ч. ATAN (0x3fdae...0) */
    6.4542903932802216696e-08,     /* мл. ч. ATAN (0x3fdb0...0) */
    1.6445663787453065277e-07,     /* мл. ч. ATAN (0x3fdb2...0) */
    8.6400192281428203280e-08,     /* мл. ч. ATAN (0x3fdb4...0) */
    6.4637580593508918868e-08,     /* мл. ч. ATAN (0x3fdb6...0) */
    9.5076757220232602783e-08,     /* мл. ч. ATAN (0x3fdb8...0) */
    1.7368800298950010478e-07,     /* мл. ч. ATAN (0x3fdba...0) */
    5.8085149750812133341e-08,     /* мл. ч. ATAN (0x3fdbc...0) */
    2.2119969788476029876e-07,     /* мл. ч. ATAN (0x3fdbe...0) */
    1.8235056113502479129e-07,     /* мл. ч. ATAN (0x3fdc0...0) */
    1.7617391634305205426e-07,     /* мл. ч. ATAN (0x3fdc2...0) */
    1.9894868058739063181e-07,     /* мл. ч. ATAN (0x3fdc4...0) */
    8.5963022920866551617e-09,     /* мл. ч. ATAN (0x3fdc6...0) */
    7.8354866345019782144e-08,     /* мл. ч. ATAN (0x3fdc8...0) */
    1.6626740584509592250e-07,     /* мл. ч. ATAN (0x3fdca...0) */
    3.0437423448679869048e-08,     /* мл. ч. ATAN (0x3fdcc...0) */
    1.4428441044179911406e-07,     /* мл. ч. ATAN (0x3fdce...0) */
    2.7613572562979763605e-08,     /* мл. ч. ATAN (0x3fdd0...0) */
    1.5396424015614917320e-07,     /* мл. ч. ATAN (0x3fdd2...0) */
    4.3261010545420318482e-08,     /* мл. ч. ATAN (0x3fdd4...0) */
    1.6916215392188508840e-07,     /* мл. ч. ATAN (0x3fdd6...0) */
    5.1710751532412748262e-08,     /* мл. ч. ATAN (0x3fdd8...0) */
    1.6468309746031336975e-07,     /* мл. ч. ATAN (0x3fdda...0) */
    2.8239832787584167953e-08,     /* мл. ч. ATAN (0x3fddc...0) */
    1.1627434342830952460e-07,     /* мл. ч. ATAN (0x3fdde...0) */
    1.8748246952419561605e-07,     /* мл. ч. ATAN (0x3fde0...0) */
    6.1800398056310694657e-10,     /* мл. ч. ATAN (0x3fde2...0) */
    2.9748189166271419370e-08,     /* мл. ч. ATAN (0x3fde4...0) */
    3.3741999902443245551e-08,     /* мл. ч. ATAN (0x3fde6...0) */
    9.9442157084358431931e-09,     /* мл. ч. ATAN (0x3fde8...0) */
    1.9417517492176633837e-07,     /* мл. ч. ATAN (0x3fdea...0) */
    1.0705621073039186477e-07,     /* мл. ч. ATAN (0x3fdec...0) */
    2.2293945567899699172e-07,     /* мл. ч. ATAN (0x3fdee...0) */
    6.2558958046688139257e-08,     /* мл. ч. ATAN (0x3fdf0...0) */
    1.0037906228236264749e-07,     /* мл. ч. ATAN (0x3fdf2...0) */
    9.5664101386946462148e-08,     /* мл. ч. ATAN (0x3fdf4...0) */
    4.6152457920554117861e-08,     /* мл. ч. ATAN (0x3fdf6...0) */
    1.8805630714835544747e-07,     /* мл. ч. ATAN (0x3fdf8...0) */
    4.2387042362160318489e-08,     /* мл. ч. ATAN (0x3fdfa...0) */
    8.3885068937955797046e-08,     /* мл. ч. ATAN (0x3fdfc...0) */
    7.2089489626912063977e-08,     /* мл. ч. ATAN (0x3fdfe...0) */
    5.0121586552767562315e-09,     /* мл. ч. ATAN (0x3fe00...0) */
    1.7416609599852210993e-07,     /* мл. ч. ATAN (0x3fe02...0) */
    9.9677957439536383767e-08,     /* мл. ч. ATAN (0x3fe04...0) */
    5.9843202636832312720e-09,     /* мл. ч. ATAN (0x3fe06...0) */
    1.1836292236688760042e-07,     /* мл. ч. ATAN (0x3fe08...0) */
    1.8608683328415422206e-07,     /* мл. ч. ATAN (0x3fe0a...0) */
    1.9767145725134894697e-07,     /* мл. ч. ATAN (0x3fe0c...0) */
    1.4244716071719925661e-07,     /* мл. ч. ATAN (0x3fe0e...0) */
    1.0550424078554664961e-08,     /* мл. ч. ATAN (0x3fe10...0) */
    3.1333521837163925750e-08,     /* мл. ч. ATAN (0x3fe12...0) */
    1.9651841890191453725e-07,     /* мл. ч. ATAN (0x3fe14...0) */
    2.1769203503917374254e-08,     /* мл. ч. ATAN (0x3fe16...0) */
    2.1561311442653000051e-07,     /* мл. ч. ATAN (0x3fe18...0) */
    5.6827109830044147633e-08,     /* мл. ч. ATAN (0x3fe1a...0) */
    1.7033145582336959447e-08,     /* мл. ч. ATAN (0x3fe1c...0) */
    9.1759002809570974428e-08,     /* мл. ч. ATAN (0x3fe1e...0) */
    2.7726630411291660102e-07,     /* мл. ч. ATAN (0x3fe20...0) */
    9.3704193761465718798e-08,     /* мл. ч. ATAN (0x3fe22...0) */
    1.5611634636831705600e-08,     /* мл. ч. ATAN (0x3fe24...0) */
    4.1396743380838325002e-08,     /* мл. ч. ATAN (0x3fe26...0) */
    1.7016474918582164962e-07,     /* мл. ч. ATAN (0x3fe28...0) */
    4.0170878854560012976e-07,     /* мл. ч. ATAN (0x3fe2a...0) */
    2.5966353922605056277e-07,     /* мл. ч. ATAN (0x3fe2c...0) */
    2.2200748765502751796e-07,     /* мл. ч. ATAN (0x3fe2e...0) */
    2.9054225080964408507e-07,     /* мл. ч. ATAN (0x3fe30...0) */
    4.6772053766662891490e-07,     /* мл. ч. ATAN (0x3fe32...0) */
    2.7979980395677259900e-07,     /* мл. ч. ATAN (0x3fe34...0) */
    2.0734455232743259907e-07,     /* мл. ч. ATAN (0x3fe36...0) */
    2.5470569869273522077e-07,     /* мл. ч. ATAN (0x3fe38...0) */
    4.2684858953954849908e-07,     /* мл. ч. ATAN (0x3fe3a...0) */
    2.5250672363355223908e-07,     /* мл. ч. ATAN (0x3fe3c...0) */
    2.1468412993384971310e-07,     /* мл. ч. ATAN (0x3fe3e...0) */
    3.2013482220159650822e-07,     /* мл. ч. ATAN (0x3fe40...0) */
    9.9353756574985597210e-08,     /* мл. ч. ATAN (0x3fe42...0) */
    3.7079294482791743025e-08,     /* мл. ч. ATAN (0x3fe44...0) */
    1.4177274936908375114e-07,     /* мл. ч. ATAN (0x3fe46...0) */
    4.2244660149019883489e-07,     /* мл. ч. ATAN (0x3fe48...0) */
    4.1181843372480152603e-07,     /* мл. ч. ATAN (0x3fe4a...0) */
    1.1997638150260533906e-07,     /* мл. ч. ATAN (0x3fe4c...0) */
    3.4370307857152127059e-08,     /* мл. ч. ATAN (0x3fe4e...0) */
    1.6612870555545327448e-07,     /* мл. ч. ATAN (0x3fe50...0) */
    5.0049961002328300827e-08,     /* мл. ч. ATAN (0x3fe52...0) */
    1.7510513994120808718e-07,     /* мл. ч. ATAN (0x3fe54...0) */
    7.7080714672903058675e-08,     /* мл. ч. ATAN (0x3fe56...0) */
    2.4591860752689585266e-07,     /* мл. ч. ATAN (0x3fe58...0) */
    2.1835902095862622076e-07,     /* мл. ч. ATAN (0x3fe5a...0) */
    8.4434288797644938319e-09,     /* мл. ч. ATAN (0x3fe5c...0) */
    1.0750614868788865168e-07,     /* мл. ч. ATAN (0x3fe5e...0) */
    5.3654495431682122694e-08,     /* мл. ч. ATAN (0x3fe60...0) */
    3.3910910151839663890e-07,     /* мл. ч. ATAN (0x3fe62...0) */
    2.6009872029392081597e-08,     /* мл. ч. ATAN (0x3fe64...0) */
    8.4267899166462150968e-08,     /* мл. ч. ATAN (0x3fe66...0) */
    5.3697223747018384635e-08,     /* мл. ч. ATAN (0x3fe68...0) */
    4.2819255817192170615e-07,     /* мл. ч. ATAN (0x3fe6a...0) */
    2.7153549148395518083e-07,     /* мл. ч. ATAN (0x3fe6c...0) */
    7.8409499814507616170e-08,     /* мл. ч. ATAN (0x3fe6e...0) */
    3.4388059913411747535e-07,     /* мл. ч. ATAN (0x3fe70...0) */
    1.3287806506036650823e-07,     /* мл. ч. ATAN (0x3fe72...0) */
    4.1804680262796764481e-07,     /* мл. ч. ATAN (0x3fe74...0) */
    2.6504241176576604419e-07,     /* мл. ч. ATAN (0x3fe76...0) */
    1.7038369534751869548e-07,     /* мл. ч. ATAN (0x3fe78...0) */
    1.5409649725961355689e-07,     /* мл. ч. ATAN (0x3fe7a...0) */
    2.3654340241245984584e-07,     /* мл. ч. ATAN (0x3fe7c...0) */
    4.3841635010687676855e-07,     /* мл. ч. ATAN (0x3fe7e...0) */
    3.0389216133992780923e-07,     /* мл. ч. ATAN (0x3fe80...0) */
    3.3113677160566492938e-07,     /* мл. ч. ATAN (0x3fe82...0) */
    6.4949429452659069812e-08,     /* мл. ч. ATAN (0x3fe84...0) */
    4.1042342988718208418e-09,     /* мл. ч. ATAN (0x3fe86...0) */
    1.7083164086911389341e-07,     /* мл. ч. ATAN (0x3fe88...0) */
    1.1081151265790922390e-07,     /* мл. ч. ATAN (0x3fe8a...0) */
    3.2367772474978363059e-07,     /* мл. ч. ATAN (0x3fe8c...0) */
    3.5566273425919270620e-07,     /* мл. ч. ATAN (0x3fe8e...0) */
    2.3010233348973825431e-07,     /* мл. ч. ATAN (0x3fe90...0) */
    4.4742900400073864290e-07,     /* мл. ч. ATAN (0x3fe92...0) */
    7.7816713561732984771e-08,     /* мл. ч. ATAN (0x3fe94...0) */
    9.9034529190853562586e-08,     /* мл. ч. ATAN (0x3fe96...0) */
    5.8580091314311401329e-08,     /* мл. ч. ATAN (0x3fe98...0) */
    4.5785906241087189007e-07,     /* мл. ч. ATAN (0x3fe9a...0) */
    3.6799306972339096798e-07,     /* мл. ч. ATAN (0x3fe9c...0) */
    2.9083646432297730480e-07,     /* мл. ч. ATAN (0x3fe9e...0) */
    2.5162157425013142998e-07,     /* мл. ч. ATAN (0x3fea0...0) */
    2.7578982474065284541e-07,     /* мл. ч. ATAN (0x3fea2...0) */
    3.8898577625031441352e-07,     /* мл. ч. ATAN (0x3fea4...0) */
    1.4021408018376805667e-07,     /* мл. ч. ATAN (0x3fea6...0) */
    3.2345143222355066318e-08,     /* мл. ч. ATAN (0x3fea8...0) */
    9.1597918073060873186e-08,     /* мл. ч. ATAN (0x3feaa...0) */
    3.4437140249864051010e-07,     /* мл. ч. ATAN (0x3feac...0) */
    3.4040189721505952964e-07,     /* мл. ч. ATAN (0x3feae...0) */
    1.0643181345370795128e-07,     /* мл. ч. ATAN (0x3feb0...0) */
    1.4620423893233886575e-07,     /* мл. ч. ATAN (0x3feb2...0) */
    9.9461037697204263020e-09,     /* мл. ч. ATAN (0x3feb4...0) */
    2.0171152809268178135e-07,     /* мл. ч. ATAN (0x3feb6...0) */
    2.7202797798619161115e-07,     /* мл. ч. ATAN (0x3feb8...0) */
    2.4840260251169378659e-07,     /* мл. ч. ATAN (0x3feba...0) */
    1.5848001121924965411e-07,     /* мл. ч. ATAN (0x3febc...0) */
    3.0037282811336888185e-08,     /* мл. ч. ATAN (0x3febe...0) */
    3.6781620458354201415e-07,     /* мл. ч. ATAN (0x3fec0...0) */
    2.4616979303234382962e-07,     /* мл. ч. ATAN (0x3fec2...0) */
    1.7008046827020426376e-07,     /* мл. ч. ATAN (0x3fec4...0) */
    1.6780671776387292789e-07,     /* мл. ч. ATAN (0x3fec6...0) */
    2.6771562200690795269e-07,     /* мл. ч. ATAN (0x3fec8...0) */
    2.1441134255029924962e-08,     /* мл. ч. ATAN (0x3feca...0) */
    4.1122822128366910819e-07,     /* мл. ч. ATAN (0x3fecc...0) */
    3.5231175239674981090e-08,     /* мл. ч. ATAN (0x3fece...0) */
    3.5271800039736783527e-07,     /* мл. ч. ATAN (0x3fed0...0) */
    4.3885738799291118145e-07,     /* мл. ч. ATAN (0x3fed2...0) */
    3.2257460675348254209e-07,     /* мл. ч. ATAN (0x3fed4...0) */
    3.2873037118280460825e-08,     /* мл. ч. ATAN (0x3fed6...0) */
    7.5667247060763931091e-08,     /* мл. ч. ATAN (0x3fed8...0) */
    3.2675015531637353378e-09,     /* мл. ч. ATAN (0x3feda...0) */
    3.2172444536209529807e-07,     /* мл. ч. ATAN (0x3fedc...0) */
    1.0663942737177661178e-07,     /* мл. ч. ATAN (0x3fede...0) */
    3.4102078813952473937e-07,     /* мл. ч. ATAN (0x3fee0...0) */
    1.0058283863123260321e-07,     /* мл. ч. ATAN (0x3fee2...0) */
    3.6843943385927664681e-07,     /* мл. ч. ATAN (0x3fee4...0) */
    2.2040307834238805008e-07,     /* мл. ч. ATAN (0x3fee6...0) */
    1.6284146709829818554e-07,     /* мл. ч. ATAN (0x3fee8...0) */
    2.2532534829668076603e-07,     /* мл. ч. ATAN (0x3feea...0) */
    4.3746223822642165204e-07,     /* мл. ч. ATAN (0x3feec...0) */
    3.5205588055504071594e-07,     /* мл. ч. ATAN (0x3feee...0) */
    4.7561439849478182004e-07,     /* мл. ч. ATAN (0x3fef0...0) */
    3.6099839903321534002e-07,     /* мл. ч. ATAN (0x3fef2...0) */
    3.7929243461151406885e-08,     /* мл. ч. ATAN (0x3fef4...0) */
    1.2985901552854973296e-08,     /* мл. ч. ATAN (0x3fef6...0) */
    3.1592754698547493082e-07,     /* мл. ч. ATAN (0x3fef8...0) */
    2.2853367988737984826e-08,     /* мл. ч. ATAN (0x3fefa...0) */
    1.1722254182355318406e-07,     /* мл. ч. ATAN (0x3fefc...0) */
    1.5199120840546442948e-07,     /* мл. ч. ATAN (0x3fefe...0) */
    1.5695823932524066085e-07,     /* мл. ч. ATAN (0x3ff00...0) */
    1.9669394630083127580e-07,     /* мл. ч. ATAN (0x3ff02...0) */
    4.7482669619792715182e-07,     /* мл. ч. ATAN (0x3ff04...0) */
    2.7599414166440890019e-07,     /* мл. ч. ATAN (0x3ff06...0) */
    3.1517804346054903242e-07,     /* мл. ч. ATAN (0x3ff08...0) */
    3.5343942831210259040e-07,     /* мл. ч. ATAN (0x3ff0a...0) */
    1.5151702565713978483e-07,     /* мл. ч. ATAN (0x3ff0c...0) */
    4.2342879672984641361e-07,     /* мл. ч. ATAN (0x3ff0e...0) */
    4.5221636989539646084e-07,     /* мл. ч. ATAN (0x3ff10...0) */
    4.7406399359317644453e-07,     /* мл. ч. ATAN (0x3ff12...0) */
    2.4772308179397835718e-07,     /* мл. ч. ATAN (0x3ff14...0) */
    8.1253319134989237086e-09,     /* мл. ч. ATAN (0x3ff16...0) */
    4.6632423187690824922e-07,     /* мл. ч. ATAN (0x3ff18...0) */
    4.2525340672504373940e-07,     /* мл. ч. ATAN (0x3ff1a...0) */
    1.1753342016056773332e-07,     /* мл. ч. ATAN (0x3ff1c...0) */
    2.5174663571489782978e-07,     /* мл. ч. ATAN (0x3ff1e...0) */
    1.0504017295564284415e-07,     /* мл. ч. ATAN (0x3ff20...0) */
    3.8410278626610322507e-07,     /* мл. ч. ATAN (0x3ff22...0) */
    3.6409811595799483239e-07,     /* мл. ч. ATAN (0x3ff24...0) */
    2.7280888178984060830e-07,     /* мл. ч. ATAN (0x3ff26...0) */
    3.3692310383626165926e-07,     /* мл. ч. ATAN (0x3ff28...0) */
    3.0515955016152298897e-07,     /* мл. ч. ATAN (0x3ff2a...0) */
    4.0190666278074469294e-07,     /* мл. ч. ATAN (0x3ff2c...0) */
    3.7351479083102180583e-07,     /* мл. ч. ATAN (0x3ff2e...0) */
    4.4193893420314017311e-07,     /* мл. ч. ATAN (0x3ff30...0) */
    3.5103467092744695260e-07,     /* мл. ч. ATAN (0x3ff32...0) */
    3.2020447415851850618e-07,     /* мл. ч. ATAN (0x3ff34...0) */
    9.0697094659687471698e-08,     /* мл. ч. ATAN (0x3ff36...0) */
    3.5609437545069338605e-07,     /* мл. ч. ATAN (0x3ff38...0) */
    3.7810238612222356777e-07,     /* мл. ч. ATAN (0x3ff3a...0) */
    3.7071566992511279947e-07,     /* мл. ч. ATAN (0x3ff3c...0) */
    6.9685702732346159292e-08,     /* мл. ч. ATAN (0x3ff3e...0) */
    1.6301372676867480072e-07,     /* мл. ч. ATAN (0x3ff40...0) */
    3.8358485180693308253e-07,     /* мл. ч. ATAN (0x3ff42...0) */
    4.6282643109038547563e-07,     /* мл. ч. ATAN (0x3ff44...0) */
    1.3069339824262330743e-07,     /* мл. ч. ATAN (0x3ff46...0) */
    6.9329151119299160250e-08,     /* мл. ч. ATAN (0x3ff48...0) */
    5.7046713599571250557e-09,     /* мл. ч. ATAN (0x3ff4a...0) */
    1.4211889189038655391e-07,     /* мл. ч. ATAN (0x3ff4c...0) */
    2.0251437075840498403e-07,     /* мл. ч. ATAN (0x3ff4e...0) */
    3.8614265314535360345e-07,     /* мл. ч. ATAN (0x3ff50...0) */
    4.1388201511855731833e-07,     /* мл. ч. ATAN (0x3ff52...0) */
    5.0676568945966925135e-09,     /* мл. ч. ATAN (0x3ff54...0) */
    3.0799714889695969839e-07,     /* мл. ч. ATAN (0x3ff56...0) */
    8.5228036573319139680e-08,     /* мл. ч. ATAN (0x3ff58...0) */
    5.1079973649761403713e-09,     /* мл. ч. ATAN (0x3ff5a...0) */
    2.5758561031540308873e-07,     /* мл. ч. ATAN (0x3ff5c...0) */
    7.7370551815985394895e-08,     /* мл. ч. ATAN (0x3ff5e...0) */
    1.2811750077411293717e-07,     /* мл. ч. ATAN (0x3ff60...0) */
    1.1823918984838038180e-07,     /* мл. ч. ATAN (0x3ff62...0) */
    2.3141741929642141851e-07,     /* мл. ч. ATAN (0x3ff64...0) */
    1.7292894665710166026e-07,     /* мл. ч. ATAN (0x3ff66...0) */
    1.2332064690912660221e-07,     /* мл. ч. ATAN (0x3ff68...0) */
    2.6157380742820094051e-07,     /* мл. ч. ATAN (0x3ff6a...0) */
    2.8826900500473488847e-07,     /* мл. ч. ATAN (0x3ff6c...0) */
    3.7926301307129972998e-07,     /* мл. ч. ATAN (0x3ff6e...0) */
    2.3201760612369019584e-07,     /* мл. ч. ATAN (0x3ff70...0) */
    1.9277502211062611006e-08,     /* мл. ч. ATAN (0x3ff72...0) */
    3.8907447035059552263e-07,     /* мл. ч. ATAN (0x3ff74...0) */
    8.0546105134399635411e-08,     /* мл. ч. ATAN (0x3ff76...0) */
    2.1547525204744550444e-07,     /* мл. ч. ATAN (0x3ff78...0) */
    6.7609994178106636271e-09,     /* мл. ч. ATAN (0x3ff7a...0) */
    9.6284595068571646573e-08,     /* мл. ч. ATAN (0x3ff7c...0) */
    1.7072983786882149825e-07,     /* мл. ч. ATAN (0x3ff7e...0) */
    3.9210108883361071061e-07,     /* мл. ч. ATAN (0x3ff80...0) */
    4.4405582640250815521e-07,     /* мл. ч. ATAN (0x3ff82...0) */
    8.7489934561393045850e-09,     /* мл. ч. ATAN (0x3ff84...0) */
    1.9735196044729774997e-07,     /* мл. ч. ATAN (0x3ff86...0) */
    2.1220018941586738918e-07,     /* мл. ч. ATAN (0x3ff88...0) */
    2.0782422137516874231e-07,     /* мл. ч. ATAN (0x3ff8a...0) */
    3.3728364921116691532e-07,     /* мл. ч. ATAN (0x3ff8c...0) */
    2.7533848490371908457e-07,     /* мл. ч. ATAN (0x3ff8e...0) */
    1.7213222301416832960e-07,     /* мл. ч. ATAN (0x3ff90...0) */
    1.7636363729895852488e-07,     /* мл. ч. ATAN (0x3ff92...0) */
    4.3529592986427925831e-07,     /* мл. ч. ATAN (0x3ff94...0) */
    1.4109174536302909479e-07,     /* мл. ч. ATAN (0x3ff96...0) */
    3.9184561986858918078e-07,     /* мл. ч. ATAN (0x3ff98...0) */
    3.7689637203725622177e-07,     /* мл. ч. ATAN (0x3ff9a...0) */
    2.3785985478992244437e-07,     /* мл. ч. ATAN (0x3ff9c...0) */
    1.1496457603531202086e-07,     /* мл. ч. ATAN (0x3ff9e...0) */
    1.4706175989088834292e-07,     /* мл. ч. ATAN (0x3ffa0...0) */
    4.7163552184003175896e-07,     /* мл. ч. ATAN (0x3ffa2...0) */
    2.7113883167059728354e-07,     /* мл. ч. ATAN (0x3ffa4...0) */
    6.3402683687083735759e-07,     /* мл. ч. ATAN (0x3ffa6...0) */
    7.4007005703925329503e-07,     /* мл. ч. ATAN (0x3ffa8...0) */
    7.2138787038930387043e-07,     /* мл. ч. ATAN (0x3ffaa...0) */
    7.0878480362962953782e-07,     /* мл. ч. ATAN (0x3ffac...0) */
    8.3176119934462038747e-07,     /* мл. ч. ATAN (0x3ffae...0) */
    2.6484962048908469447e-07,     /* мл. ч. ATAN (0x3ffb0...0) */
    8.8648566776027320909e-08,     /* мл. ч. ATAN (0x3ffb2...0) */
    4.2881033195343263576e-07,     /* мл. ч. ATAN (0x3ffb4...0) */
    4.5605184298476900336e-07,     /* мл. ч. ATAN (0x3ffb6...0) */
    2.9351415837523977653e-07,     /* мл. ч. ATAN (0x3ffb8...0) */
    6.3099037540642048741e-08,     /* мл. ч. ATAN (0x3ffba...0) */
    8.3915415744834643599e-07,     /* мл. ч. ATAN (0x3ffbc...0) */
    8.3378675614566040391e-07,     /* мл. ч. ATAN (0x3ffbe...0) */
    1.6524612757370986731e-07,     /* мл. ч. ATAN (0x3ffc0...0) */
    8.5793453293273888387e-07,     /* мл. ч. ATAN (0x3ffc2...0) */
    1.6669789097389930639e-07,     /* мл. ч. ATAN (0x3ffc4...0) */
    1.1357983638093946008e-07,     /* мл. ч. ATAN (0x3ffc6...0) */
    8.1211238406045562730e-07,     /* мл. ч. ATAN (0x3ffc8...0) */
    4.6732678893178886887e-07,     /* мл. ч. ATAN (0x3ffca...0) */
    1.4413596444747318832e-07,     /* мл. ч. ATAN (0x3ffcc...0) */
    9.0632234280135417059e-07,     /* мл. ч. ATAN (0x3ffce...0) */
    1.8513880943291763474e-09,     /* мл. ч. ATAN (0x3ffd0...0) */
    3.9962550515858443149e-07,     /* мл. ч. ATAN (0x3ffd2...0) */
    2.9907726660293876762e-07,     /* мл. ч. ATAN (0x3ffd4...0) */
    7.5957461127841061742e-07,     /* мл. ч. ATAN (0x3ffd6...0) */
    9.3205988627461091767e-07,     /* мл. ч. ATAN (0x3ffd8...0) */
    9.2008337424423115641e-07,     /* мл. ч. ATAN (0x3ffda...0) */
    8.2613950659618889683e-07,     /* мл. ч. ATAN (0x3ffdc...0) */
    7.5167734214810341975e-07,     /* мл. ч. ATAN (0x3ffde...0) */
    7.9711099252963715673e-07,     /* мл. ч. ATAN (0x3ffe0...0) */
    1.0815567576128473964e-07,     /* мл. ч. ATAN (0x3ffe2...0) */
    6.9053529440484615499e-07,     /* мл. ч. ATAN (0x3ffe4...0) */
    7.3427247386725412270e-07,     /* мл. ч. ATAN (0x3ffe6...0) */
    3.3574465280755404678e-07,     /* мл. ч. ATAN (0x3ffe8...0) */
    5.4401989583531860000e-07,     /* мл. ч. ATAN (0x3ffea...0) */
    4.9984400895402445196e-07,     /* мл. ч. ATAN (0x3ffec...0) */
    2.9667348752632753221e-07,     /* мл. ч. ATAN (0x3ffee...0) */
    2.7011131326142788315e-08,     /* мл. ч. ATAN (0x3fff0...0) */
    7.3609022383885755129e-07,     /* мл. ч. ATAN (0x3fff2...0) */
    6.0718705978227300376e-07,     /* мл. ч. ATAN (0x3fff4...0) */
    6.8367656584711034334e-07,     /* мл. ч. ATAN (0x3fff6...0) */
    1.0067036911601747168e-07,     /* мл. ч. ATAN (0x3fff8...0) */
    8.5339795676409487781e-07,     /* мл. ч. ATAN (0x3fffa...0) */
    1.6782164884714841380e-07,     /* мл. ч. ATAN (0x3fffc...0) */
    3.7389191996290698169e-08,     /* мл. ч. ATAN (0x3fffe...0) */
    5.4732289909676706546e-07,     /* мл. ч. ATAN (0x40000...0) */
    1.0152382758064486991e-08,     /* мл. ч. ATAN (0x40002...0) */
    1.7457758241544437999e-07,     /* мл. ч. ATAN (0x40004...0) */
    7.3807439607756864724e-07,     /* мл. ч. ATAN (0x40006...0) */
    4.3128726419387877595e-07,     /* мл. ч. ATAN (0x40008...0) */
    8.3300723921990824365e-07,     /* мл. ч. ATAN (0x4000a...0) */
    6.4840185780971669110e-07,     /* мл. ч. ATAN (0x4000c...0) */
    4.7765744525727934697e-07,     /* мл. ч. ATAN (0x4000e...0) */
    9.0889615263397093352e-07,     /* мл. ч. ATAN (0x40010...0) */
    6.1108794690985897572e-07,     /* мл. ч. ATAN (0x40012...0) */
    1.4900346671204116832e-07,     /* мл. ч. ATAN (0x40014...0) */
    7.6115968987399267145e-08,     /* мл. ч. ATAN (0x40016...0) */
    9.3484692710660145184e-07,     /* мл. ч. ATAN (0x40018...0) */
    3.9578373408596588293e-07,     /* мл. ч. ATAN (0x4001a...0) */
    8.8730998207494947933e-07,     /* мл. ч. ATAN (0x4001c...0) */
    5.9093188923137505969e-08,     /* мл. ч. ATAN (0x4001e...0) */
    3.1905404642429014986e-07,     /* мл. ч. ATAN (0x40020...0) */
    2.5051890559309618504e-07,     /* мл. ч. ATAN (0x40022...0) */
    3.3448231554157581666e-07,     /* мл. ч. ATAN (0x40024...0) */
    8.8796084068733658331e-08,     /* мл. ч. ATAN (0x40026...0) */
    9.2939967061982426361e-07,     /* мл. ч. ATAN (0x40028...0) */
    4.4847723240618182613e-07,     /* мл. ч. ATAN (0x4002a...0) */
    4.4050669890109652143e-08,     /* мл. ч. ATAN (0x4002c...0) */
    1.5180219918550586378e-07,     /* мл. ч. ATAN (0x4002e...0) */
    2.4526421857410059494e-07,     /* мл. ч. ATAN (0x40030...0) */
    7.4335359290786218865e-07,     /* мл. ч. ATAN (0x40032...0) */
    1.4953021490197270882e-07,     /* мл. ч. ATAN (0x40034...0) */
    7.7402034749103461628e-07,     /* мл. ч. ATAN (0x40036...0) */
    1.5092122841855203804e-07,     /* мл. ч. ATAN (0x40038...0) */
    5.7511377004323123083e-07,     /* мл. ч. ATAN (0x4003a...0) */
    5.1935940617966190423e-07,     /* мл. ч. ATAN (0x4003c...0) */
    3.5650791897360936278e-07,     /* мл. ч. ATAN (0x4003e...0) */
    4.5230704345167773377e-07,     /* мл. ч. ATAN (0x40040...0) */
    2.1188277205736396991e-07,     /* мл. ч. ATAN (0x40042...0) */
    9.4091338260910992866e-07,     /* мл. ч. ATAN (0x40044...0) */
    1.2373115007727742545e-07,     /* мл. ч. ATAN (0x40046...0) */
    6.5354089802893699571e-09,     /* мл. ч. ATAN (0x40048...0) */
    9.2181323014895018901e-07,     /* мл. ч. ATAN (0x4004a...0) */
    3.3480274436112031674e-07,     /* мл. ч. ATAN (0x4004c...0) */
    4.7302213278970441514e-07,     /* мл. ч. ATAN (0x4004e...0) */
    6.9700644393664551158e-07,     /* мл. ч. ATAN (0x40050...0) */
    3.6145884890751117911e-07,     /* мл. ч. ATAN (0x40052...0) */
    7.2272460605296022760e-07,     /* мл. ч. ATAN (0x40054...0) */
    1.7054190342076013723e-07,     /* мл. ч. ATAN (0x40056...0) */
    9.0388165656796974865e-07,     /* мл. ч. ATAN (0x40058...0) */
    3.4799546711358916904e-07,     /* мл. ч. ATAN (0x4005a...0) */
    6.9127287990942068573e-07,     /* мл. ч. ATAN (0x4005c...0) */
    3.0228397793770328396e-07,     /* мл. ч. ATAN (0x4005e...0) */
    4.0560845057454174174e-07,     /* мл. ч. ATAN (0x40060...0) */
    3.1357033612917851892e-07,     /* мл. ч. ATAN (0x40062...0) */
    2.8736482592762358709e-07,     /* мл. ч. ATAN (0x40064...0) */
    5.8348539106414941196e-07,     /* мл. ч. ATAN (0x40066...0) */
    5.0014855429824706666e-07,     /* мл. ч. ATAN (0x40068...0) */
    2.8473931202390260121e-07,     /* мл. ч. ATAN (0x4006a...0) */
    1.8023136244084268674e-07,     /* мл. ч. ATAN (0x4006c...0) */
    4.2527945898435952870e-07,     /* мл. ч. ATAN (0x4006e...0) */
    3.0063530850440945197e-07,     /* мл. ч. ATAN (0x40070...0) */
    3.6584331002412690449e-08,     /* мл. ч. ATAN (0x40072...0) */
    8.1303174896817583369e-07,     /* мл. ч. ATAN (0x40074...0) */
    9.4488942247957481430e-07,     /* мл. ч. ATAN (0x40076...0) */
    6.5052959123514857090e-07,     /* мл. ч. ATAN (0x40078...0) */
    1.4451650783550757505e-07,     /* мл. ч. ATAN (0x4007a...0) */
    5.9135917213303620265e-07,     /* мл. ч. ATAN (0x4007c...0) */
    2.9089068011273489085e-07,     /* мл. ч. ATAN (0x4007e...0) */
    4.0038897708207991708e-07,     /* мл. ч. ATAN (0x40080...0) */
    1.6627841331000776871e-07,     /* мл. ч. ATAN (0x40082...0) */
    7.3889847191500687750e-07,     /* мл. ч. ATAN (0x40084...0) */
    4.0420201438519557969e-07,     /* мл. ч. ATAN (0x40086...0) */
    3.0586941158823583320e-07,     /* мл. ч. ATAN (0x40088...0) */
    6.3067795452746850450e-07,     /* мл. ч. ATAN (0x4008a...0) */
    6.0856701239169127262e-07,     /* мл. ч. ATAN (0x4008c...0) */
    4.2005034203591520686e-07,     /* мл. ч. ATAN (0x4008e...0) */
    2.4260400291931394088e-07,     /* мл. ч. ATAN (0x40090...0) */
    2.5072717830820984535e-07,     /* мл. ч. ATAN (0x40092...0) */
    6.1600162066382521109e-07,     /* мл. ч. ATAN (0x40094...0) */
    5.5347543829537823805e-07,     /* мл. ч. ATAN (0x40096...0) */
    2.2906852156885663334e-07,     /* мл. ч. ATAN (0x40098...0) */
    7.5962805850161012526e-07,     /* мл. ч. ATAN (0x4009a...0) */
    3.9828553800514564709e-07,     /* мл. ч. ATAN (0x4009c...0) */
    2.5655570076929069465e-07,     /* мл. ч. ATAN (0x4009e...0) */
    4.8969113921629619375e-07,     /* мл. ч. ATAN (0x400a0...0) */
    2.9673300503079664540e-07,     /* мл. ч. ATAN (0x400a2...0) */
    7.8158353595938272970e-07,     /* мл. ч. ATAN (0x400a4...0) */
    1.8468294880915614769e-07,     /* мл. ч. ATAN (0x400a6...0) */
    5.5877705348280208805e-07,     /* мл. ч. ATAN (0x400a8...0) */
    1.3956907112444441557e-07,     /* мл. ч. ATAN (0x400aa...0) */
    2.1485173953096367314e-08,     /* мл. ч. ATAN (0x400ac...0) */
    3.4302154497238125430e-07,     /* мл. ч. ATAN (0x400ae...0) */
    2.8678772777861288525e-07,     /* мл. ч. ATAN (0x400b0...0) */
    9.4057197358152823232e-07,     /* мл. ч. ATAN (0x400b2...0) */
    5.2901112789643498576e-07,     /* мл. ч. ATAN (0x400b4...0) */
    1.3567709097550045264e-07,     /* мл. ч. ATAN (0x400b6...0) */
    8.4209354563783960841e-07,     /* мл. ч. ATAN (0x400b8...0) */
    8.6675182138893117339e-07,     /* мл. ч. ATAN (0x400ba...0) */
    3.3352044677692255584e-07,     /* мл. ч. ATAN (0x400bc...0) */
    3.1800798047254826213e-07,     /* мл. ч. ATAN (0x400be...0) */
    9.4025072276790309142e-07,     /* мл. ч. ATAN (0x400c0...0) */
    4.1107395947315309583e-07,     /* мл. ч. ATAN (0x400c2...0) */
    7.5417265533040719451e-07,     /* мл. ч. ATAN (0x400c4...0) */
    1.7675097062263551671e-07,     /* мл. ч. ATAN (0x400c6...0) */
    6.9895011091368418253e-07,     /* мл. ч. ATAN (0x400c8...0) */
    5.2448640221907995419e-07,     /* мл. ч. ATAN (0x400ca...0) */
    7.1640341706367443706e-07,     /* мл. ч. ATAN (0x400cc...0) */
    4.2873162489348176744e-07,     /* мл. ч. ATAN (0x400ce...0) */
    7.2121622739380478585e-07,     /* мл. ч. ATAN (0x400d0...0) */
    7.4464981542366486001e-07,     /* мл. ч. ATAN (0x400d2...0) */
    6.0192460888239578930e-07,     /* мл. ч. ATAN (0x400d4...0) */
    3.9438681387778243825e-07,     /* мл. ч. ATAN (0x400d6...0) */
    2.2186469369107374728e-07,     /* мл. ч. ATAN (0x400d8...0) */
    1.8269605123233548910e-07,     /* мл. ч. ATAN (0x400da...0) */
    3.7375513673094270897e-07,     /* мл. ч. ATAN (0x400dc...0) */
    8.9047899406031160852e-07,     /* мл. ч. ATAN (0x400de...0) */
    8.7321894235300643769e-07,     /* мл. ч. ATAN (0x400e0...0) */
    4.1461447126565385788e-07,     /* мл. ч. ATAN (0x400e2...0) */
    5.5961798027743475131e-07,     /* мл. ч. ATAN (0x400e4...0) */
    4.4449605771983552414e-07,     /* мл. ч. ATAN (0x400e6...0) */
    1.5787615899486597255e-07,     /* мл. ч. ATAN (0x400e8...0) */
    7.4076984783138157552e-07,     /* мл. ч. ATAN (0x400ea...0) */
    3.7189829539178189755e-07,     /* мл. ч. ATAN (0x400ec...0) */
    8.9760477855245291375e-08,     /* мл. ч. ATAN (0x400ee...0) */
    9.3163207142485933020e-07,     /* мл. ч. ATAN (0x400f0...0) */
    1.1888958640932676887e-07,     /* мл. ч. ATAN (0x400f2...0) */
    5.9377449661518304683e-07,     /* мл. ч. ATAN (0x400f4...0) */
    5.2899629972122238600e-07,     /* мл. ч. ATAN (0x400f6...0) */
    3.4728581241686119933e-09,     /* мл. ч. ATAN (0x400f8...0) */
    4.8675803099621316617e-08,     /* мл. ч. ATAN (0x400fa...0) */
    7.4130122629603938169e-07,     /* мл. ч. ATAN (0x400fc...0) */
    2.4961430032182775255e-07,     /* мл. ч. ATAN (0x400fe...0) */
    5.5551373559005923921e-07,     /* мл. ч. ATAN (0x40100...0) */
    3.8364863758857785894e-08,     /* мл. ч. ATAN (0x40102...0) */
    7.1933716860112890213e-07,     /* мл. ч. ATAN (0x40104...0) */
    2.9732343099863466223e-07,     /* мл. ч. ATAN (0x40106...0) */
    2.7055326618005384987e-07,     /* мл. ч. ATAN (0x40108...0) */
    2.1505861146785078072e-07,     /* мл. ч. ATAN (0x4010a...0) */
    6.4618827333386675648e-07,     /* мл. ч. ATAN (0x4010c...0) */
    1.5805764975286221116e-07,     /* мл. ч. ATAN (0x4010e...0) */
    1.9237487844557830375e-07,     /* мл. ч. ATAN (0x40110...0) */
    2.7050649752652529856e-07,     /* мл. ч. ATAN (0x40112...0) */
    8.5492105861150519516e-07,     /* мл. ч. ATAN (0x40114...0) */
    4.8857097224030069308e-07,     /* мл. ч. ATAN (0x40116...0) */
    5.6365369143104894093e-07,     /* мл. ч. ATAN (0x40118...0) */
    5.5361518245991039867e-07,     /* мл. ч. ATAN (0x4011a...0) */
    8.7453399246313119120e-07,     /* мл. ч. ATAN (0x4011c...0) */
    2.4446069853668088060e-08,     /* мл. ч. ATAN (0x4011e...0) */
    3.0572564215718914794e-07,     /* мл. ч. ATAN (0x40120...0) */
    1.9601340662654176784e-07,     /* мл. ч. ATAN (0x40122...0) */
    7.0573401192852936012e-08,     /* мл. ч. ATAN (0x40124...0) */
    2.9524407969983751070e-07,     /* мл. ч. ATAN (0x40126...0) */
    2.7305290181886248989e-07,     /* мл. ч. ATAN (0x40128...0) */
    3.5184351215130597659e-07,     /* мл. ч. ATAN (0x4012a...0) */
    8.7087003176551452828e-07,     /* мл. ч. ATAN (0x4012c...0) */
    2.5370749764007836199e-07,     /* мл. ч. ATAN (0x4012e...0) */
    7.3054768223553929853e-07,     /* мл. ч. ATAN (0x40130...0) */
    7.0904569961314220985e-07,     /* мл. ч. ATAN (0x40132...0) */
    4.9659860845959775786e-07,     /* мл. ч. ATAN (0x40134...0) */
    3.9322138648629849917e-07,     /* мл. ч. ATAN (0x40136...0) */
    6.9176376084638663819e-07,     /* мл. ч. ATAN (0x40138...0) */
    7.2444525182085331333e-07,     /* мл. ч. ATAN (0x4013a...0) */
    7.7040598843490573354e-07,     /* мл. ч. ATAN (0x4013c...0) */
    1.4855336140585884717e-07,     /* мл. ч. ATAN (0x4013e...0) */
    7.8773629142111271926e-08,     /* мл. ч. ATAN (0x40140...0) */
    8.2109125278444527844e-07,     /* мл. ч. ATAN (0x40142...0) */
    7.2217073694521924550e-07,     /* мл. ч. ATAN (0x40144...0) */
    3.0184160236353968383e-08,     /* мл. ч. ATAN (0x40146...0) */
    8.9497577276320366096e-07,     /* мл. ч. ATAN (0x40148...0) */
    6.9250092662971400131e-07,     /* мл. ч. ATAN (0x4014a...0) */
    6.0804882293955371201e-07,     /* мл. ч. ATAN (0x4014c...0) */
    8.6801978054702454835e-07,     /* мл. ч. ATAN (0x4014e...0) */
    7.4006922742055996742e-07,     /* мл. ч. ATAN (0x40150...0) */
    4.4059564662919316780e-07,     /* мл. ч. ATAN (0x40152...0) */
    1.8120106859825160986e-07,     /* мл. ч. ATAN (0x40154...0) */
    1.6882154173046278321e-07,     /* мл. ч. ATAN (0x40156...0) */
    6.0585342457932893811e-07,     /* мл. ч. ATAN (0x40158...0) */
    7.3660133619563731972e-07,     /* мл. ч. ATAN (0x4015a...0) */
    7.5474517703000489222e-07,     /* мл. ч. ATAN (0x4015c...0) */
    8.4978046272490098276e-07,     /* мл. ч. ATAN (0x4015e...0) */
    2.5345505506739276869e-07,     /* мл. ч. ATAN (0x40160...0) */
    1.0089968531712737001e-07,     /* мл. ч. ATAN (0x40162...0) */
    5.6970923021668692138e-07,     /* мл. ч. ATAN (0x40164...0) */
    8.8004370772170505433e-07,     /* мл. ч. ATAN (0x40166...0) */
    2.4840047425114481221e-07,     /* мл. ч. ATAN (0x40168...0) */
    7.4873204991836082292e-07,     /* мл. ч. ATAN (0x4016a...0) */
    6.3681788056349481913e-07,     /* мл. ч. ATAN (0x4016c...0) */
    7.2399415457886908515e-08,     /* мл. ч. ATAN (0x4016e...0) */
    1.6559227009501267008e-07,     /* мл. ч. ATAN (0x40170...0) */
    1.1594714982392317231e-07,     /* мл. ч. ATAN (0x40172...0) */
    7.3554155649649447260e-08,     /* мл. ч. ATAN (0x40174...0) */
    1.8544739471907275415e-07,     /* мл. ч. ATAN (0x40176...0) */
    5.9568156076053303048e-07,     /* мл. ч. ATAN (0x40178...0) */
    4.9173193170024343445e-07,     /* мл. ч. ATAN (0x4017a...0) */
    1.1915127050539742580e-08,     /* мл. ч. ATAN (0x4017c...0) */
    2.4545911707283978878e-07,     /* мл. ч. ATAN (0x4017e...0) */
    3.7154823853095219340e-07,     /* мл. ч. ATAN (0x40180...0) */
    5.2041213321863283093e-07,     /* мл. ч. ATAN (0x40182...0) */
    8.1971551114489919455e-07,     /* мл. ч. ATAN (0x40184...0) */
    4.4094606747394959973e-07,     /* мл. ч. ATAN (0x40186...0) */
    4.6049787994496990753e-07,     /* мл. ч. ATAN (0x40188...0) */
    4.5032865129757230856e-08,     /* мл. ч. ATAN (0x4018a...0) */
    2.6623509626853581576e-07,     /* мл. ч. ATAN (0x4018c...0) */
    2.8616897473988697331e-07,     /* мл. ч. ATAN (0x4018e...0) */
    2.1835605380911761367e-07,     /* мл. ч. ATAN (0x40190...0) */
    1.7415308481309820358e-07,     /* мл. ч. ATAN (0x40192...0) */
    2.6280291659374109695e-07,     /* мл. ч. ATAN (0x40194...0) */
    5.9148397878341774158e-07,     /* мл. ч. ATAN (0x40196...0) */
    3.1168407784544625510e-07,     /* мл. ч. ATAN (0x40198...0) */
    4.8027013157819337141e-07,     /* мл. ч. ATAN (0x4019a...0) */
    2.4483640372434900748e-07,     /* мл. ч. ATAN (0x4019c...0) */
    6.5844602538471055417e-07,     /* мл. ч. ATAN (0x4019e...0) */
    8.6497677989690386030e-07,     /* мл. ч. ATAN (0x401a0...0) */
    6.5116186387919858016e-09,     /* мл. ч. ATAN (0x401a2...0) */
    8.4402362163924838255e-08,     /* мл. ч. ATAN (0x401a4...0) */
    2.3726345776367090446e-07,     /* мл. ч. ATAN (0x401a6...0) */
    5.5570783910041330464e-07,     /* мл. ч. ATAN (0x401a8...0) */
    1.7503585815375652113e-07,     /* мл. ч. ATAN (0x401aa...0) */
    1.3629480155254578511e-07,     /* мл. ч. ATAN (0x401ac...0) */
    5.2529154133396000539e-07,     /* мл. ч. ATAN (0x401ae...0) */
    4.7262719724390181905e-07,     /* мл. ч. ATAN (0x401b0...0) */
    6.1079522618609771091e-08,     /* мл. ч. ATAN (0x401b2...0) */
    3.2563577569542300887e-07,     /* мл. ч. ATAN (0x401b4...0) */
    3.9250178648307979117e-07,     /* мл. ч. ATAN (0x401b6...0) */
    3.4015609248354180606e-07,     /* мл. ч. ATAN (0x401b8...0) */
    2.4570600397164014293e-07,     /* мл. ч. ATAN (0x401ba...0) */
    1.8491720479818495160e-07,     /* мл. ч. ATAN (0x401bc...0) */
    2.3224259544148115233e-07,     /* мл. ч. ATAN (0x401be...0) */
    4.6085040066516847007e-07,     /* мл. ч. ATAN (0x401c0...0) */
    9.4265156339573376491e-07,     /* мл. ч. ATAN (0x401c2...0) */
    7.9465212930975939449e-07,     /* мл. ч. ATAN (0x401c4...0) */
    8.6327907961606952940e-08,     /* мл. ч. ATAN (0x401c6...0) */
    7.9332416436697401947e-07,     /* мл. ч. ATAN (0x401c8...0) */
    1.2176014509808210213e-07,     /* мл. ч. ATAN (0x401ca...0) */
    4.4996367328680902300e-08,     /* мл. ч. ATAN (0x401cc...0) */
    6.2793793070315393517e-07,     /* мл. ч. ATAN (0x401ce...0) */
    2.7057462892890471910e-08,     /* мл. ч. ATAN (0x401d0...0) */
    2.1246339899389435434e-07,     /* мл. ч. ATAN (0x401d2...0) */
    2.9220159907401390087e-07,     /* мл. ч. ATAN (0x401d4...0) */
    3.2697391313291638682e-07,     /* мл. ч. ATAN (0x401d6...0) */
    3.7648464599224642753e-07,     /* мл. ч. ATAN (0x401d8...0) */
    4.9946083509283293071e-07,     /* мл. ч. ATAN (0x401da...0) */
    7.5367203887090749329e-07,     /* мл. ч. ATAN (0x401dc...0) */
    2.4227533294041064709e-07,     /* мл. ч. ATAN (0x401de...0) */
    9.2853142572296140425e-07,     /* мл. ч. ATAN (0x401e0...0) */
    6.4285274090968034787e-09,     /* мл. ч. ATAN (0x401e2...0) */
    3.9111779323462358471e-07,     /* мл. ч. ATAN (0x401e4...0) */
    2.2851338042005984449e-07,     /* мл. ч. ATAN (0x401e6...0) */
    5.2470410306114973010e-07,     /* мл. ч. ATAN (0x401e8...0) */
    3.7759857124255012651e-07,     /* мл. ч. ATAN (0x401ea...0) */
    7.9163878623702713079e-07,     /* мл. ч. ATAN (0x401ec...0) */
    8.6311882347339425197e-07,     /* мл. ч. ATAN (0x401ee...0) */
    6.4122335963252529206e-07,     /* мл. ч. ATAN (0x401f0...0) */
    1.7436857335540269088e-07,     /* мл. ч. ATAN (0x401f2...0) */
    4.6389132765115423961e-07,     /* мл. ч. ATAN (0x401f4...0) */
    6.0304074492599175255e-07,     /* мл. ч. ATAN (0x401f6...0) */
    6.3801534777176484536e-07,     /* мл. ч. ATAN (0x401f8...0) */
    6.1430261025511705590e-07,     /* мл. ч. ATAN (0x401fa...0) */
    5.7669251027838049169e-07,     /* мл. ч. ATAN (0x401fc...0) */
    5.6929077489103336347e-07,     /* мл. ч. ATAN (0x401fe...0) */
    6.3553182659044996684e-07      /* мл. ч. ATAN (0x40200...0) */
};
