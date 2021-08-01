/* Copyright (c) 2016-2018 ZAO "MCST". All rights reserved. */

#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ÚÔÁÚ·ÌÌ·  ˜˘˛ÈÛÏÒÂÙ  ˙Ó·˛ÂÓÈÂ  ÊıÓÎ„ÈÈ  '·ÚÎÛÈ-   %
%  ÓıÛ  ÁÈÂÚ‚ÔÏÈ˛ÂÛÎÈÍ'.                                %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

DB __asinh (DB X)

#define K00 0.8828125
#define LK00 0x3fec400000000000LL

#define KB6 4.63477937452195140167e+00
#define KB5 7.40238485884407997888e+00
#define KB4 7.65465011841961239551e+00
#define KB3 1.14148219305813446527e-01
#define KB2 -5.50981853202639758494e+00
#define KB1 -8.31905586697785988438e+00
#define KB0 -6.30188361130610383759e+00
#define KD7 -1.17988956861094199693e+00
#define KD6 -1.05244743845715359692e+01
#define KD5 -3.33018017039691374484e+01
#define KD4 -7.60092184708505497925e+01
#define KD3 -9.99629981597867292021e+01
#define KD2 -1.23502978214033747524e+02
#define KD1 -7.09888220639143123537e+01
#define KD0 -5.37760956676153573249e+01
#define LKB6 0x40128a039ab6fd0cLL
#define LKB5 0x401d9c0ac6c49048LL
#define LKB4 0x401e9e5c99c3bd32LL
#define LKB3 0x3fbd38d154d0aab2LL
#define LKB2 0xc0160a0dde87cadcLL
#define LKB1 0xc020a35b4a64889fLL
#define LKB0 0xc0193520fa370849LL
#define LKD7 0xbff2e0d3e261356eLL
#define LKD6 0xc0250c87e812a621LL
#define LKD5 0xc040a6a170303657LL
#define LKD4 0xc05300970911b4a0LL
#define LKD3 0xc058fda1c308991aLL
#define LKD2 0xc05ee030cb88f807LL
#define LKD1 0xc051bf48dc5684d0LL
#define LKD0 0xc04ae3571a537cd5LL

#define KA5 2.50952659779946086330e-03
#define KA4 7.82527660021732273362e-02
#define KA3 6.11706634375995876807e-01
#define KA2 1.82613172447329989897e+00
#define KA1 2.27857085875236076898e+00
#define KE6 1.26039481637277767565e-03
#define KE5 7.39376846785012119960e-02
#define KE4 9.56172789930479138235e-01
#define KE3 4.70172368326469818025e+00
#define KE2 1.04515498526895883385e+01
#define KE1 1.06142834350094282384e+01
#define LKA5 0x3f648edbd54fcc00LL
#define LKA4 0x3fb4085f8ecd061fLL
#define LKA3 0x3fe39319caac8427LL
#define LKA2 0x3ffd37d5e62cd083LL
#define LKA1 0x40023a835bbfae22LL
#define LKE6 0x3f54a67aa10c3096LL
#define LKE5 0x3fb2ed9481a2d8f5LL
#define LKE4 0x3fee98f7adc27293LL
#define LKE3 0x4012ce90a739cd5bLL
#define LKE2 0x4024e7318ad3a17cLL
#define LKE1 0x40253a835bbfae19LL

#define KP8 -4.38123508094986244600e-03
#define KP6 -5.90738917214283593101e-01
#define KP4 -4.33743943630887664097e+00
#define KP2 -8.97228735730969077324e+00
#define KP0 -5.47544161973665551677e+00
#define KQ6 1.27997283521200060486e+01
#define KQ4 4.81026571618661305202e+01
#define KQ2 6.86174165171457126459e+01
#define KQ0 3.28526497184199351357e+01
#define LKP8 0xbf71f20ed6386458LL
#define LKP6 0xbfe2e7554d3d1d14LL
#define LKP4 0xc0115989b93d4f28LL
#define LKP2 0xc021f1cfa603eb35LL
#define LKP0 0xc015e6da2affb46fLL
#define LKQ6 0x40299975fe9c142cLL
#define LKQ4 0x40480d23deb07526LL
#define LKQ2 0x40512783c09149abLL
#define LKQ0 0x40406d23a03fc753LL
#define K2 4.00006488775094945494e-01
#define K1 6.66666666629825839191e-01
{
    _type_double_bits arg, y0;
    DB y, yv2, yv4, dob;
    LL exp, i0;
#if __iset__ >= 6
    _type_double_bits ly, lyv2, lyv4, lres, lres1, lres2, lres3;
#endif /* __iset__ < 6 */

    arg.value = dfabs (X);
    if (arg.llong >= 0x7ff0000000000000LL) /* |X| == NaN …Ã… Inf */
        return X + X;
    if (arg.llong <= 0x3ff3333333333333LL) {       /* |X| <= 1.2 */
        yv2 = X * X;
        yv4 = yv2 * yv2;
        if (arg.llong <= 0x3fe0000000000000LL) {    /* |X| <= 0.5 */
            if (arg.llong < 0x3e30000000000000LL)     /* |X| <= 2**-28 */
                return X + X * yv2;
#if __iset__ >= 6
            y0.value = X;
            arg.value = yv2 * X;
            lyv2.value = yv2;
            lyv4.value = yv4;
            lres.llong = __builtin_e2k_fmad (LKP2, lyv2.llong, LKP0);
            lres1.llong = __builtin_e2k_fmad (LKP8, lyv2.llong, LKP6);
            lres1.llong = __builtin_e2k_fmad (lres1.llong, lyv2.llong, LKP4);
            lres2.llong = __builtin_e2k_fmad (lres1.llong, lyv4.llong, lres.llong);

            lres.llong = __builtin_e2k_fmad (LKQ2, lyv2.llong, LKQ0);
            lres1.value = yv2 + KQ6;
            lres1.llong = __builtin_e2k_fmad (lres1.llong, lyv2.llong, LKQ4);
            lres1.llong = __builtin_e2k_fmad (lres1.llong, lyv4.llong, lres.llong);
            lres.value = lres2.value / lres1.value;
            lres.llong = __builtin_e2k_fmad (lres.llong, arg.llong, y0.llong);
            X = lres.value;
#else /* __iset__ < 6 */
            X = (KP0 + KP4 * yv2 * (KP2 / KP4 + yv2) + KP8 * yv2 * yv4 * (KP6 / KP8 + yv2)) /
                (KQ0 + KQ4 * yv2 * (KQ2 / KQ4 + yv2) + yv4 * (yv4 + KQ6 * yv2)) * (yv2 * X) + X;
#endif /* __iset__ < 6 */
        }
        else {
            y = arg.value;
            y0.value = X;
#if __iset__ >= 6
            lyv2.value = yv2;
            lyv4.value = yv4;
            lres.llong = __builtin_e2k_fmad (LKB1, arg.llong, LKB0);
            lres1.llong = __builtin_e2k_fmad (LKB3, arg.llong, LKB2);
            lres2.llong = __builtin_e2k_fmad (lres1.llong, lyv2.llong, lres.llong);
            lres.llong = __builtin_e2k_fmad (LKB5, arg.llong, LKB4);
            lres1.value = y + KB6;
            lres1.llong = __builtin_e2k_fmad (lres1.llong, lyv2.llong, lres.llong);
            lres2.llong = __builtin_e2k_fmad (lres1.llong, lyv4.llong, lres2.llong);

            lres.llong = __builtin_e2k_fmad (LKD1, arg.llong, LKD0);
            lres1.llong = __builtin_e2k_fmad (LKD3, arg.llong, LKD2);
            lres3.llong = __builtin_e2k_fmad (lres1.llong, lyv2.llong, lres.llong);
            lres.llong = __builtin_e2k_fmad (LKD5, arg.llong, LKD4);
            lres1.llong = __builtin_e2k_fmad (LKD7, arg.llong, LKD6);
            lres1.llong = __builtin_e2k_fmad (lres1.llong, lyv2.llong, lres.llong);
            lres1.llong = __builtin_e2k_fmad (lres1.llong, lyv4.llong, lres3.llong);

            arg.value = lres2.value * X / lres1.value;
            arg.llong = __builtin_e2k_fmad (LK00, y0.llong, arg.llong);
            X = arg.value;
#else /* __iset__ < 6 */
            y0.llong &= ~0xffffffffLL;
            X = (KB0 + KB1 * y + KB3 * yv2 * (KB2 / KB3 + y) + yv4 * (KB4 + KB5 * y + yv2 * (KB6 + y))) * X /
                (KD0 + KD1 * y + KD3 * yv2 * (KD2 / KD3 + y) + KD7 * yv4 * (KD4 / KD7 + KD5 / KD7 * y +
                 yv2 * (KD6 / KD7 + y))) + K00 * (X - y0.value) + K00 * y0.value;
#endif /* __iset__ < 6 */
        }
        return X;
    }                           /*  log(2|x|)+P5(x^2)/Q6(x^2) */
    if (arg.llong < 0x4197d78400000000LL) {        /* |X| < 1e8 */
        y = X * X;
        yv2 = y * y;
        yv4 = yv2 * yv2;
#if __iset__ >= 6
        ly.value = y;
        lyv2.value = yv2;
        lyv4.value = yv4;
        lres.llong = __builtin_e2k_fmad (LKA4, ly.llong, LKA5);
        lres1.llong = __builtin_e2k_fmad (LKA2, ly.llong, LKA3);
        lres2.llong = __builtin_e2k_fmad (lres1.llong, lyv2.llong, lres.llong);
        lres.value = y + KA1;
        lres2.llong = __builtin_e2k_fmad (lres.llong, lyv4.llong, lres2.llong);

        lres.llong = __builtin_e2k_fmad (LKE5, ly.llong, LKE6);
        lres1.llong = __builtin_e2k_fmad (LKE3, ly.llong, LKE4);
        lres3.llong = __builtin_e2k_fmad (lres1.llong, lyv2.llong, lres.llong);
        lres.llong = __builtin_e2k_fmad (0x4010000000000000LL, ly.llong, LKE1);
        lres1.llong = __builtin_e2k_fmad (lres.llong, ly.llong, LKE2);
        lres1.llong = __builtin_e2k_fmad (lres1.llong, lyv4.llong, lres3.llong);
        lres.value = lres2.value / lres1.value;
        dob = lres.value;
#else /* __iset__ < 6 */
        dob = (KA5 + KA4 * y + KA2 * yv2 * (KA3 / KA2 + y) + yv4 * (KA1 + y)) / (KE6
            + KE5 * y + KE3 * yv2 * (KE4 / KE3 + y) + yv4 * (KE2 + KE1 * y + 4.0 * yv2));
#endif /* __iset__ < 6 */
    }
    else {
        y = 0.5 / X;
        dob = y * y;
    }
    exp = exponent (arg) - 1022;
    i0 = arg.llong & 0xfe00000000000LL;
    y0.llong = i0 | 0x3ff0000000000000LL;
    arg.llong &= 0xfffffffffffffLL;
    arg.llong |= 0x3ff0000000000000LL;
    y = (arg.value - y0.value) / (arg.value + y0.value);
    yv2 = y * y;
    y0.llong = ((LL) ((unsigned *) constln)[i0 >>= 45]) << 32;
#if __iset__ >= 6
    arg.value = K2 * y * yv2;
    lres.value = yv2 + K1 / K2;
    ly.value = y + y + exp * KLN2_lo + constln_lo[i0];
    lres.llong = __builtin_e2k_fmad (arg.llong, lres.llong, ly.llong);
    y = lres.value + dob + (y0.value + exp * KLN2_hi);
#else /* __iset__ < 6 */
    y = y + y + exp * KLN2_lo + constln_lo[i0] + K2 * y * yv2 * (yv2 + K1 / K2) + dob + (y0.value + exp * KLN2_hi);
#endif /* __iset__ < 6 */
    if (X > 0)
       return y;
    else
       return -y;
}

weak_alias (__asinh, asinh)
#ifdef NO_LONG_DOUBLE
strong_alias (__asinh, __asinhl)
weak_alias (__asinh, asinhl)
#endif
