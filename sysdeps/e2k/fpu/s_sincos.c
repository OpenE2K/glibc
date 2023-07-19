/* Copyright (c) 2016-2018 ZAO "MCST". All rights reserved. */

#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ðòïçòáííá  ÷ùþéóìñåô  úîáþåîéå  æõîëãéê  'óéîõó' é %
%  'ëïóéîõó'. áòçõíåîô  ÷  òáäéáîáè.                     %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#if 0
static LD CONSTLD[] = {
    -1.6666666666666616665801e-01L,     /* KA3 */
    9.9999999999999999685282e-01L,      /* KA1 */
    -4.9999999999999999852420e-01L,     /* KA2 */
    1.0000000000000000000000e+00L,      /* KA0 */
    -3.1415926535897931159980 / 2,      /* PIO2_1  */
    1.2246467991473531772015e-16L / 2,  /* PIO2_1T */
};
    LD KA3 = CONSTLD[0];     /* -1.6666666666666616665801e-01    */
    LD KA1 = CONSTLD[1];     /* 9.9999999999999999685282e-01     */
    LD KA2 = CONSTLD[2];     /* -4.9999999999999999852420e-01    */
    LD KA0 = CONSTLD[3];     /* 1.0000000000000000000000e+00     */
    LD PIO2_1 = CONSTLD[4];  /* -3.1415926535897931159980/2 53bits */
    LD PIO2_1T = CONSTLD[5]; /* 1.2246467991473531772015e-16L/2  */
#endif

void __sincos (DB X, DB *SI, DB *CO)

#define KA13 1.5894736651849095247744e-10
#define KA11 -2.5050716974102745027602e-08
#define KA9  2.7557313376400129128204e-06
#define KA7  -1.9841269828650300013059e-04
#define KA5  8.3333333333203624567193e-03
#define KA14 -1.1360687302437253587387e-11
#define KA12 2.0875740157060361726804e-09
#define KA10 -2.7557314436057753118620e-07
#define KA8  2.4801587289509923185682e-05
#define KA6  -1.3888888888873247734608e-03
#define KA4  4.1666666666666577342883e-02
#define LKA14 0xbda8fb7f9656c8efLL
#define LKA12 0x3e21ee9fb1e54f8dLL
#define LKA10 0xbe927e4f81905bf6LL
#define LKA8  0x3efa01a019cb3bd5LL
#define LKA6  0xbf56c16c16c14feaLL
#define LKA4  0x3fa5555555555548LL
#define LKA13 0x3de5d87699eb6a31LL
#define LKA11 0xbe5ae5e37e850d20LL
#define LKA9  0x3ec71de3530aa14dLL
#define LKA7  0xbf2a01a019ba956dLL
#define LKA5  0x3f8111111110f3dcLL

#define TWOONPI 0.636619772367581343075535 /* 0x3fe45f306dc9c883 */
#define RS    6.75539944105574400e+15   /* 3*2^51 = 0x4338000000000000 */

#define KA3  -1.6666666666666616665801e-01L
#define KA1   9.9999999999999999685282e-01L
#define KA2  -4.9999999999999999852420e-01L
#define KA0   1.0000000000000000000000e+00
#define PIO2_1 (-3.1415926535897931159980 * 0.5) /* 53bits */
#define LPIO2_1 0xbff921fb54442d18LL
#define PIO2_1T (1.2246467991473531772015e-16L * 0.5)
{
    type_union_64f darg0, darg1, si, co;
    DB t0, y0, tmp_si, xv2, xv4;
    LD ldx, ldxv2;
    int k0, k1;
#if __iset__ >= 6
    _type_double_bits lyv2, lyv4, lyv8, lres, lres1, lres2;
#else /* __iset__ < 6  */
    DB xv8;
#endif /* __iset__ < 6  */

    if (X == 0) { /* for X = -0.0 only */
        *SI = X;
        *CO = 1;
        return;
    }

    darg0.db = X;
    darg0.ll &= ~0x8000000000000000LL;
    darg1.db = X * TWOONPI;

    if (darg0.ll <= 0x3fe921fb00000000LL) { /* pi/4 */
        ldx = (LD) X;
        ldxv2 = ldx * ldx;
        xv2 = X * X;
        xv4 = xv2 * xv2;
#if __iset__ >= 6
        lyv2.value = xv2;
        lyv4.value = xv4;
        lyv8.value = X * xv4;
        lres2.value = KA5 * lyv8.value;
        lres.llong = __builtin_e2k_fmad (LKA9, lyv2.llong, LKA7);
        lres1.llong = __builtin_e2k_fmad (LKA13, lyv2.llong, LKA11);
        lres1.llong = __builtin_e2k_fmad (lres1.llong, lyv4.llong, lres.llong);
        lres.value = xv2 * lyv8.value;
        lres.llong = __builtin_e2k_fmad (lres1.llong, lres.llong, lres2.llong);
        *SI = (DB) (KA3 * ldx * (KA1 / KA3 + ldxv2) + lres.value);
        lyv8.value = xv4 * xv4;
        lres2.llong = __builtin_e2k_fmad (LKA6, lyv2.llong, LKA4);
        lres2.value *= xv4;
        lres.llong = __builtin_e2k_fmad (LKA10, lyv2.llong, LKA8);
        lres1.llong = __builtin_e2k_fmad (LKA14, lyv2.llong, LKA12);
        lres1.llong = __builtin_e2k_fmad (lres1.llong, lyv4.llong, lres.llong);
        lres.llong = __builtin_e2k_fmad (lres1.llong, lyv8.llong, lres2.llong);
        *CO = (DB) ((KA0 + ldxv2 * KA2) + lres.value);
#else /* __iset__ < 6 */
	xv8 = xv4 * xv4;
        *SI = (DB) (KA3 * ldx * (KA1 / KA3 + ldxv2) + (KA7 * X * xv4 * (KA5 / KA7 + xv2) +
            X * xv8 * (KA9 + KA11 * xv2 + KA13 * xv4)));
        *CO = (DB) ((KA0 + ldxv2 * KA2) + (xv4 * (KA4 + KA6 * xv2) + KA14 * xv8 *
            (KA8 / KA14 + KA10 / KA14 * xv2 + xv4 * (KA12 / KA14 + xv2))));
#endif /* __iset__ < 6 */
        return;
    }
#if __iset__ <= 2
    darg1.db += RS;
    t0 = darg1.db - RS;
    k0 = darg1.ll;
#else /*  __iset__ <= 2 */
#pragma asm_inline
    __asm ("fdtoifd 0x0,%1,%0" : "=r" (t0): "r" (darg1.db)); /* rc=0 - to nearest */
    k0 = (LL) t0;
#endif /*  __iset__ <= 2 */

    if (unlikely(darg0.ll > 0x40d921fb00000000LL)) {      /* 2^13*pi */
        X = __rem_pio2 (X, &y0, &k1);
        ldx = (LD) X + (LD) y0;
        k0 = k1;
    }
    else {
#if __iset__ >= 6
        lres.value = t0;
        lres1.value = X;
        lres.llong = __builtin_e2k_fmad (LPIO2_1, lres.llong, lres1.llong);
        X = (DB) (ldx = (LD) lres.value - t0 * PIO2_1T);
#else /* __iset__ < 6 */
        X = (DB) (ldx = t0 * (LD) PIO2_1 + (LD) X - t0 * PIO2_1T);
#endif /* __iset__ < 6 */
    }
    ldxv2 = ldx * ldx;
    xv2 = X * X;
    xv4 = xv2 * xv2;
#if __iset__ >= 6
    lyv2.value = xv2;
    lyv4.value = xv4;
    lyv8.value = xv4 * xv4;
    lres2.llong = __builtin_e2k_fmad (LKA6, lyv2.llong, LKA4);
    lres2.value *= xv4;
    lres.llong = __builtin_e2k_fmad (LKA10, lyv2.llong, LKA8);
    lres1.llong = __builtin_e2k_fmad (LKA14, lyv2.llong, LKA12);
    lres1.llong = __builtin_e2k_fmad (lres1.llong, lyv4.llong, lres.llong);
    lres.llong = __builtin_e2k_fmad (lres1.llong, lyv8.llong, lres2.llong);
    co.db = (DB) ((KA0 + ldxv2 * KA2) + lres.value);

    lyv8.value = X * xv4;
    lres2.value = KA5 * lyv8.value;
    lres.llong = __builtin_e2k_fmad (LKA9, lyv2.llong, LKA7);
    lres1.llong = __builtin_e2k_fmad (LKA13, lyv2.llong, LKA11);
    lres1.llong = __builtin_e2k_fmad (lres1.llong, lyv4.llong, lres.llong);
    lres.value = xv2 * lyv8.value;
    lres.llong = __builtin_e2k_fmad (lres1.llong, lres.llong, lres2.llong);
    tmp_si = (DB) (KA3 * ldx * (KA1 / KA3 + ldxv2) + lres.value);
#else /* __iset__ < 6 */
    xv8 = xv4 * xv4;
    co.db = (DB) ((KA0 + ldxv2 * KA2) + (xv4 * (KA4 + KA6 * xv2) + KA14 * xv8 *
        (KA8 / KA14 + KA10 / KA14 * xv2 + xv4 * (KA12 / KA14 + xv2))));
    tmp_si = (DB) ((KA1 + KA3 * ldxv2) * ldx + (KA7 * X * xv4 * (KA5 / KA7 + xv2) +
        X * xv8 * (KA9 + KA11 * xv2 + KA13 * xv4)));
#endif /* __iset__ < 6 */
    if (k0 & 1) {
        si.db = co.db;
        co.db = tmp_si;
    }
    else {
        si.db = tmp_si;
    }
    si.ll += ((LL)(k0 & 2)) << 62;
    co.ll += ((LL)((k0 + 1) & 2)) << 62;
    *SI = si.db;
    *CO = co.db;
}

weak_alias (__sincos, sincos)
