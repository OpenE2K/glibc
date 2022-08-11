/* Copyright (c) 2016-2018 ZAO "MCST". All rights reserved. */

#include "f2c.h"
#include <e2kintrin.h>

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ���������  ���������  �������  ��������  �������   %
%  '��������'.  ������������   ���������   ���������     %
%  ��������  |�| > 1.  ���������  �  ��������.           %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define KA13 2.87578513674215642476e-02
#define KA12 -1.48518870712472024717e-02
#define KA11 1.74008794426940218992e-02
#define KA10 5.45750671864035852905e-03
#define KA9 1.03228143501857792250e-02
#define KA8 1.14791774151849059071e-02
#define KA7 1.39712129735529333028e-02
#define KA6 1.73523927208699729416e-02
#define KA5 2.23721729421498878750e-02
#define KA4 3.03819441385312472866e-02
#define KA3 4.46428571463554297826e-02
#define KA2 7.49999999999843293317e-02
#define KA1 1.66666666666666678265e-01
#define LKA13 0x3f9d72b2bc8155f7LL
#define LKA12 0xbf8e6aaa8a0a04cbLL
#define LKA11 0x3f91d189408314eeLL
#define LKA10 0x3f765a9c4dfcf8b2LL
#define LKA9  0x3f852420b04b37beLL
#define LKA8  0x3f8782651caa6547LL
#define LKA7  0x3f8c9cf07674736aLL
#define LKA6  0x3f91c4d35cf95421LL
#define LKA5  0x3f96e8bb1c8209a2LL
#define LKA4  0x3f9f1c71c1db0623LL
#define LKA3  0x3fa6db6db6e31f13LL
#define LKA2  0x3fb3333333332ecaLL
#define LKA1  0x3fc5555555555556LL

DB __asin (DB X)
{
    _type_double_bits arg, rez, rez1;
    double z, s, p, zv2, zv4;
#if __iset__ >= 6
    _type_double_bits lz, lzv2, lzv4, lzv8, lres, lres1, lres2;
#endif /* __iset__ < 6 */

    arg.value = X;
    arg.llong = __builtin_e2k_pandd (arg.llong, 0x7fffffffffffffffLL);
    if (arg.twolong.int0 <= 0x3fe00000) {
        z = X * X;
        zv2 = z * z;
        zv4 = zv2 * zv2;
#if __iset__ >= 6
        lz.value = z;
        lzv2.value = zv2;
        lzv4.value = zv4;
        lzv8.value = zv4 * zv4;
        lres.value = KA1 * z;
        lres1.llong = __builtin_e2k_fmad (LKA3, lz.llong, LKA2);
        lres2.llong = __builtin_e2k_fmad (lres1.llong, lzv2.llong, lres.llong);
        lres.llong = __builtin_e2k_fmad (LKA5, lz.llong, LKA4);
        lres1.llong = __builtin_e2k_fmad (LKA7, lz.llong, LKA6);
        lres1.llong = __builtin_e2k_fmad (lres1.llong, lzv2.llong, lres.llong);
        lres2.llong = __builtin_e2k_fmad (lres1.llong, lzv4.llong, lres2.llong);
        lres.llong = __builtin_e2k_fmad (LKA9, lz.llong, LKA8);
        lres1.llong = __builtin_e2k_fmad (LKA11, lz.llong, LKA10);
        lres1.llong = __builtin_e2k_fmad (lres1.llong, lzv2.llong, lres.llong);
        lres.llong = __builtin_e2k_fmad (LKA13, lz.llong, LKA12);
        lres1.llong = __builtin_e2k_fmad (lres.llong, lzv4.llong, lres1.llong);
        arg.llong = __builtin_e2k_fmad (lres1.llong, lzv8.llong, lres2.llong);
        rez1.value = X;
        rez.llong = __builtin_e2k_fmad (arg.llong, rez1.llong, rez1.llong);
        return rez.value;
#else /* __iset__ < 6 */
//        p = KA2 * z * (KA1/KA2 + z) + KA3 * z * zv2 + zv4 * (KA4 +
        p = KA1 * z + zv2 * (KA2 + KA3 * z) + zv4 * (KA4 +
            KA5 * z) + KA9 * zv2 * zv4 * (KA6/KA9 + KA7/KA9 * z +
            zv2 * (z + KA8/KA9)) + KA13 * zv2 * (zv4 * zv4) * (KA10/KA13 +
            KA11/KA13 * z + zv2 * (z + KA12/KA13));
        return p * X + X;
#endif /* __iset__ < 6 */
    }
    if (arg.twolong.int0 >= 0x3ff00000)
        return (arg.value > 1.0) ? SVID_libm_err(X, 0.0, 2)
            : X / arg.value * PION2;  /* NaN ��� ��/2*sign(X) */
    rez.value = 1.0 - arg.value;
    rez1.llong = __builtin_e2k_paddd (rez.llong, 0x10000000000000LL);
    rez.llong = __builtin_e2k_psubd (rez.llong, 0x10000000000000LL);
    s = dsqrt (rez1.value);
    z = rez.value;
    zv2 = z * z;
    zv4 = zv2 * zv2;
    rez.value = s;
    rez.llong = __builtin_e2k_pandnd (rez.llong, 0x00000000ffffffffLL);
#if __iset__ >= 6
    lz.value = z;
    lzv2.value = zv2;
    lzv4.value = zv4;
    lzv8.value = zv4 * zv4;
    lres.value = KA1 * z;
    lres1.llong = __builtin_e2k_fmad (LKA3, lz.llong, LKA2);
    lres2.llong = __builtin_e2k_fmad (lres1.llong, lzv2.llong, lres.llong);
    lres.llong = __builtin_e2k_fmad (LKA5, lz.llong, LKA4);
    lres1.llong = __builtin_e2k_fmad (LKA7, lz.llong, LKA6);
    lres1.llong = __builtin_e2k_fmad (lres1.llong, lzv2.llong, lres.llong);
    lres2.llong = __builtin_e2k_fmad (lres1.llong, lzv4.llong, lres2.llong);
    lres.llong = __builtin_e2k_fmad (LKA9, lz.llong, LKA8);
    lres1.llong = __builtin_e2k_fmad (LKA11, lz.llong, LKA10);
    lres1.llong = __builtin_e2k_fmad (lres1.llong, lzv2.llong, lres.llong);
    lres.llong = __builtin_e2k_fmad (LKA13, lz.llong, LKA12);
    lres1.llong = __builtin_e2k_fmad (lres.llong, lzv4.llong, lres1.llong);
    arg.llong = __builtin_e2k_fmad (lres1.llong, lzv8.llong, lres2.llong);

    rez1.llong = __builtin_e2k_fnmad (rez.llong, rez.llong, rez1.llong);
    lz.value = rez1.value / (s + rez.value);
    rez1.value = s;
    rez1.llong = __builtin_e2k_fmsd (arg.llong, rez1.llong, LPION2_lo);
    p = rez1.value + lz.value;
#else /* __iset__ < 6 */
// ��������� ����� �������� ����������� ��������������� ��� ������� ��������.
// ��� ����� ���� ����� ������� �� ��������� sqrt � fdivd.
///p = (((KA13 * z + KA12) * z + KA11) * z + KA10) * (zv2 * zv4 * zv4) +
//        + (((KA9 * z + KA8) * z + KA7) * z + KA6) * (zv2 * zv4) +
//        ((((KA5 * z + KA4) * z + KA3) * z + KA2) * z + KA1) * z;
    p = zv4 * (KA4 + KA5 * z) + KA9 * zv2 * zv4 * (KA6/KA9 + KA7/KA9 * z +
        zv2 * (z + KA8/KA9)) + KA13 * zv2 * (zv4 * zv4) * (KA10/KA13 + KA11/KA13 * z +
        zv2 * (z + KA12/KA13)) + ((KA3 * z + KA2) * z + KA1) * z;
    p = s * p - PION2_lo + (rez1.value - rez.value * rez.value) / (s + rez.value);
#endif /* __iset__ < 6 */
    s = PION2 - rez.value;
    if (X < 0) {
        return p - s;
    }
    else {
        return s - p;
    }
}

strong_alias (__asin, __asin_finite)
weak_alias (__asin, asin)
weak_alias (__asin, __ieee754_asin)
