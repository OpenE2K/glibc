/* Copyright (c) 2016-2018 ZAO "MCST". All rights reserved. */

#include "f2c.h"
#include <e2kintrin.h>

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     πςοηςαννα  χωώισμρετ  ϊξαώεξιε  ζυξλγιι  X ** Y .  %
%  ξεδοπυστινωνι  ϊξαώεξιρνι  αςηυνεξτοχ ρχμρΰτσρ :      %
%  1. X = 0  ι  Y <= 0 ;                                 %
%  2. X < 0  ι  Y - ξε γεμοε ώισμο ;                     %
%  3. |X ** Y| > 2 ** 128 ;                              %
%  4. |X ** Y| < 2 ** -150 ;                             %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define KB2 3.66556671660783833261e-06
#define KB1 2.70760782821392980564e-03
#define KA3 (9.61831373313694322622e-01*256)
#define KA1 (2.88539008151742884443e+00*256)
#define LKA3 0x406ec7529694c54cLL
#define LKA1 0x4087154765228fa1LL

#define KINVLN2 (1.442695040888963387*256)

FL __powf (FL X, FL Y)
{
    _type_float_bits absx, absy;
    _type_double_bits darg, rez;
    int yisint, signx, signy, exp;
    int i0;
    DB y;
    FL iy;
#if __iset__ >= 6
    _type_double_bits ly, lyv2, lres, lres1;
#endif /* __iset__ < 6 */

    absx.value = X;
    signx = (unsigned)absx.int0 >> 31;
    absy.value = Y;
    signy = (unsigned)absy.int0 >> 31;
    absx.value = ffabs (X);
    absy.value = ffabs (Y);

    if (unlikely (Y == 0)) {
        if (X != X) { /* pow (NaN, 0.0) */
            i0 = 142;
            goto ERROR;
        }
        if (X == 0) { /* pow (0.0, 0.0) */
            i0 = 120;
            goto ERROR;
        }
        return 1;
    }
    if (unlikely (X != X || Y != Y)) { /* X ΙΜΙ Y == NaN */
        return X == 1 ? 1 : X + Y;
    }

    if (Y == 1)
        return X;
    if (unlikely (absy.int0 == 0x7f800000)) { /* |Y|=Inf */
        if (absx.int0 == 0x3f800000)
            return 1; /* +-1 ** +-Inf */
        return ((absx.int0 < 0x3f800000) != signy) ? 0.0F : absy.value;
    }

    yisint = 0;                            /* Y - ΞΕ ΓΕΜΟΕ */
    if (signx) {                           /* X < 0        */
        if (absy.int0 >= 0x4b800000)       /* |Y| >= 2^24  */
            yisint = 2;                    /* Y - ήΕΤΞΟΕ   */
        else {
            i0 = (int) absy.value;
#if __iset__ <= 2
            iy = (FL) i0;
#else /*  __iset__ <= 2 */
#pragma asm_inline
            __asm ("fstoifs 0x3,%1,%0" : "=r" (iy): "r" (absy.value));
#endif /*  __iset__ <= 2 */
            if (absy.value == iy) yisint = 2 - (i0 & 1);
        }
    }

    if (unlikely (absx.int0 == 0)) { /* X = 0 */
        if (signy) {
            i0 = (yisint == 1) ? 123 : 143; /* pow(-0.0,negative) : pow(+0.0,negative) */
            goto ERROR;
        }
        return (yisint == 1) ? X : 0;
    }
    if (unlikely (absx.int0 == 0x7f800000)) {     /* |X| = Inf */
        if (signy)
            absx.int0 = 0;
        absx.int0 += yisint << 31;
        return absx.value;
    }
    if (unlikely (signx && yisint == 0)) {
        i0 = 124;
        goto ERROR;
    }

    /* ΧΩήΙΣΜΕΞΙΕ y = 256*log2(X)*Y */
    darg.value = (DB) absx.value;
    rez.llong = __builtin_e2k_paddd (darg.llong, 0x400000000000LL);
    rez.llong = __builtin_e2k_pandd (rez.llong, 0x7fff800000000000LL);
    i0 = rez.twolong.int0;
    y = (darg.value - rez.value) / (darg.value + rez.value);
#if __iset__ >= 6
    ly.value = y;
    lyv2.value = y * y;
    lres.value = (DB) (((i0 >> 20) - 1023) << 8) + KINVLN2 * constnln[(i0 >> 15) & 0x1f];
    lres.llong = __builtin_e2k_fmad (ly.llong, LKA1, lres.llong);
    lres1.value = KA3 * y;
    lres.llong = __builtin_e2k_fmad (lres1.llong, lyv2.llong, lres.llong);
    y = lres.value;
#else /* __iset__ < 6 */
    y = KA1 * y + ((DB) (((i0 >> 20) - 1023) << 8) + KINVLN2 *
        constnln[(i0 >> 15) & 0x1f]) + KA3 * y * (y * y);
#endif /* __iset__ < 6 */
    rez.value = y * Y;

    /* ΧΩήΙΣΜΕΞΙΕ 2 ** (y/256) */
    exp = (int) rez.value;

    if (unlikely (rez.twolong.int0 >= 0x40e00000)) { /* 32768 */
        i0 = 121;
        goto ERROR;
    }
    if (unlikely (rez.value<-38400)) {
        i0 = 122;
ERROR:  return (FL) SVID_libm_err (X, Y, i0);
    }
#if __iset__ <= 2
    y = rez.value - (DB) exp;
#else /*  __iset__ <= 2 */
#pragma asm_inline
    __asm ("fdtoifd 0x3,%1,%0" : "=r" (y): "r" (rez.value));
    y = rez.value - y;
#endif /*  __iset__ <= 2 */
    rez.value = constnexp[exp & 255];
    rez.llong = __builtin_e2k_paddd (rez.llong, ((LL) (exp >> 8) << 52) + ((LL) yisint << 63));
#if __iset__ >= 6
    lres.value = KB2 * y * (y + KB1 / KB2);
    lres.llong = __builtin_e2k_fmad (lres.llong, rez.llong, rez.llong);
    return (FL) lres.value;
#else /* __iset__ < 6 */
    return (FL) (KB2 * y * (y + KB1 / KB2) * rez.value + rez.value);
#endif /* __iset__ < 6 */
}

strong_alias (__powf, __powf_finite)
weak_alias (__powf, powf)
weak_alias (__powf, __ieee754_powf)
