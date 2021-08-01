/* Copyright (c) 2016-2018 ZAO "MCST". All rights reserved. */

#include "f2c.h"
#include <e2kintrin.h>

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ðòïçòáííá  ÷ùþéóìñåô  úîáþåîéå  æõîëãéé  'ôáîçåîó  %
%  çéðåòâïìéþåóëéê'.                                     %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define KB11 -7.56554333894031718715e-03
#define KB9 2.16937728566991232877e-02
#define KB7 -5.39583625149326614810e-02
#define KB5 1.33333136842460507279e-01
#define KB3 -3.33333332705376943759e-01
#define LKB11 0xbf7efd0c1379baf3LL
#define LKB9  0x3f9636e473c91697LL
#define LKB7  0xbfaba06e34b1d79cLL
#define LKB5  0x3fc1110f6b1b1124LL
#define LKB3  0xbfd5555554a8b8d5LL

#define KA2 3.66556671660783833261e-06
#define KA1 2.70760782821392980564e-03
#define LKA2  0x3ecebfbe9d182250LL
#define LKA1  0x3f662e43e2528362LL

FL __tanhf (FL X)
{
    _type_float_bits arg, ed;
    _type_double_bits exp;
    DB y, yp;
    LL k;
#if __iset__ >= 6
    _type_double_bits ly, lyv2, lyv4, lyv5, lres, lres1, lres2;
#endif /* __iset__ < 6 */

    arg.value = ffabs (X);
    ed.value = X;
    ed.int0 = (ed.int0 & 0x80000000) | 0x3f800000;
    y = (DB) X;

    if (X == 0) return X; /* for X = -0.0 only */

    if (arg.int0 < 0x3ecccccd) {        /* |X| < 0.4 */
        yp = y * y;
#if __iset__ >= 6
        ly.value = y;
        lyv2.value = yp;
        lyv4.value = yp * yp;
        lyv5.value = lyv4.value * y;
        lres1.value = KB3 * y;
        lres2.llong = __builtin_e2k_fmad (lres1.llong, lyv2.llong, ly.llong);
        lres.llong = __builtin_e2k_fmad (LKB7, lyv2.llong, LKB5);
        lres1.llong = __builtin_e2k_fmad (LKB11, lyv2.llong, LKB9);
        lres1.llong = __builtin_e2k_fmad (lres1.llong, lyv4.llong, lres.llong);
        lres.llong = __builtin_e2k_fmad (lres1.llong, lyv5.llong, lres2.llong);
        return (FL) lres.value;
#else /* __iset__ < 6 */
        return (FL) (y + KB5 * y * yp * (yp + KB3/KB5) + KB11 * y * yp *
            (yp * yp) * (yp * (KB9/KB11 + yp) + KB7/KB11));
#endif /* __iset__ < 6 */
    }
    if (arg.int0 >= 0x41200000) {       /* |X| >= 10 */
        return ed.value + FL_MIN_UN / X;
    }
    y = (K256ONLN2 * 2.0) * y;
    k = (LL) y;
#if __iset__ <= 2
    y -= (DB) k;
#else /*  __iset__ <= 2 */
#pragma asm_inline
    __asm ("fdtoifd 0x3,%1,%0" : "=r" (yp): "r" (y));
    y -= yp;
#endif /*  __iset__ <= 2 */
    exp.value = constnexp[k & 255];
    exp.llong = __builtin_e2k_paddd (exp.llong, (k >> 8) << 52);
#if __iset__ >= 6
    lres.value = KA2 * y * (y + KA1 / KA2);
    lres.llong = __builtin_e2k_fmad (lres.llong, exp.llong, exp.llong);
    y = lres.value;
#else /* __iset__ < 6 */
    y = KA2 * y * (y + KA1 / KA2) * exp.value + exp.value;
#endif /* __iset__ < 6 */
    return (FL) ((y - 1) / (y + 1));
}

weak_alias (__tanhf, tanhf)
