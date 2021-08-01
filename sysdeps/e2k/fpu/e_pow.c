/* Copyright (c) 2016-2018 ZAO "MCST". All rights reserved. */

#include "f2c.h"
#include <e2kintrin.h>

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ПРОГРАММА  ВЫЧИСЛЯЕТ  ЗНАЧЕНИЕ  ФУНКЦИИ  X ** Y .  %
%  НЕДОПУСТИМЫМИ  ЗНАЧЕНИЯМИ  АРГУМЕНТОВ ЯВЛЯЮТСЯ :      %
%  1. X = 0  И  Y <= 0 ;                                 %
%  2. X < 0  И  Y - НЕ ЦЕЛОЕ ЧИСЛО ;                     %
%  3. |X ** Y| > 2 ** 1024 ;                             %
%  4. |X ** Y| < 2 ** -1075 ;                            %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

DB __pow (DB X, DB Y)

#define KB5 1.21195555854068860923e-15
#define KB4 2.23939573811855104311e-12
#define KB3 3.30830268126604677436e-09
#define KB2 3.66556559691003767877e-06
#define KB1 2.70760617406228636578e-03
#define LKB5 0x3cd5d52893bc7fecLL
#define LKB4 0x3d83b2abc07c93d0LL
#define LKB3 0x3e2c6b08d71f5d1eLL
#define LKB2 0x3ecebfbdff82c4edLL
#define LKB1 0x3f662e42fefa39efLL

#define KA5 (5.77080365796174592306e-01*256)
#define KA3 (9.61796693922627790184e-01*256)
#define KA1_h (2.8853759765625e+00*256)
#define KA1_l (1.410521542681542921e-05*256)
#define KA1 (KA1_h+KA1_l)
#define KINVLN2_lo 1.410521542681471940e-05
#define LKA5 0x406277713e4843e3LL
#define LKA3 0x406ec709dc398e33LL
#define LKA1_h 0x4087154000000000LL
#define LKA1_l 0x3f6d94ae0bf85f82LL
#define LKA1 0x40871547652b82feLL
#define LKINVLN2_lo 0x3eed94ae0bf85ddfLL
{
    _type_double_bits absx, absy, absxx, absyy;
    LL i0, yisint, signx, signy, exp;
    DB y, yd, ydv2, u, s, s_l, m_h, m_l;
#if __iset__ >= 6
    _type_double_bits ly, lyv2, lyv4, lres, lres1;
#endif /* __iset__ < 6 */

    absy.value = Y;
    signy = (unsigned long long) absy.llong >> 63;
    absx.value = X;
    signx = (unsigned long long) absx.llong >> 63;
    absy.value = dfabs (Y);
    absx.value = dfabs (X);
    exp = exponentabs (absx);
    if (unlikely (!(exp))) {
        absx.value *= DVAIN52;
        exp = exponentabs (absx) - (2046 + 52);
    }
    else {
        exp -= 2046;
    }

    /* вычисление y = 256*log2(X) */
    absxx.llong = absx.llong & 0xfffffffffffffLL;
    absxx.llong |= 0x3ff0000000000000LL;
    absyy.llong = absxx.llong + 0x100000000000LL;
    i0 = (absyy.llong >> 45) & 0x7f;
    absyy.llong &= 0xffffe00000000000;
    exp += absyy.llong >> 52;
    u = absxx.value - absyy.value;
    yd = absxx.value + absyy.value;

    if (unlikely (Y == 0)) {
        if (X != X) { /* pow (NaN, 0.0) */
            i0 = 42;
            goto ERROR;
        }
        if (X == 0) { /* pow (0.0, 0.0) */
            i0 = 20;
            goto ERROR;
        }
        return 1;
    }

    s = u / yd;
    ydv2 = s * s;
    absxx.value = s;
    absxx.llong = __builtin_e2k_pandd (absxx.llong, 0xffffffff00000000LL);
//#if __iset__ >= 6
#if 0
    ly.value = u;
    absyy.value += absyy.value;
    lres.llong = __builtin_e2k_fnmad (absyy.llong, absxx.llong, ly.llong);
//    lres.llong = __builtin_e2k_fnmad (ly.llong, absxx.llong, lres.llong);
    s_l = KA1 / yd * (lres.value - u * absxx.value);
//    s_l = KA1 / yd * lres.value;
#else /* __iset__ < 6 */
    s_l = KA1 / yd * (u - (absyy.value + absyy.value) * absxx.value - u * absxx.value);
#endif /*  __iset__ < 6 */

    if (unlikely (X != X || Y != Y)) { /* X или Y == NaN */
        return X == 1 ? 1 : X + Y;
    }
    absyy.llong = (LL)((unsigned *) constln)[i0] << 32;
#if __iset__ >= 6
    ly.value = (DB) (exp << 8);
    lres.llong = __builtin_e2k_fmad (absyy.llong, 0x4077154000000000LL, ly.llong);
    y = lres.value;
#else /* __iset__ < 6 */
    y = absyy.value * (KA1_h * 0.5) + (DB) (exp << 8);
#endif /*  __iset__ < 6 */
    yd = absxx.value * KA1_h;

    if (Y == 1)
        return X;

    m_h = y + yd;
    m_l = m_h - y - yd;

    if (unlikely (absy.llong >= 0x7ff0000000000000LL)) {   /* |Y|=Inf */
        if (absx.value == 1)
            return 1;       /* +-1 ** +-Inf */
        return ((absx.value < 1) != signy) ? 0 : absy.value;
    }

#if __iset__ >= 6
    ly.value = absxx.value * KA1_l + constln_lo[i0] * (KA1 * 0.5) +
        absyy.value * (KINVLN2_lo * 128);
//    ly.value = constln_lo[i0] * (KA1 * 0.5);
//    ly.llong = __builtin_e2k_fmad (absxx.llong, LKA1_l, ly.llong);
//    lres.value = KINVLN2_lo * 128;
//    ly.llong = __builtin_e2k_fmad (absyy.llong, lres.llong, ly.llong);
    absyy.value = KA5 * s * ydv2;
    lres.value = KA3 / KA5 + ydv2;
    lres.llong = __builtin_e2k_fmad (absyy.llong, lres.llong, ly.llong);
    y = lres.value - (m_l - s_l);
#else /* __iset__ < 6 */
    y = absxx.value * KA1_l + constln_lo[i0] * (KA1 * 0.5) + absyy.value
        * (KINVLN2_lo * 128) + KA5 * s * ydv2 * (KA3 / KA5 + ydv2) - (m_l - s_l);
#endif /* __iset__ < 6 */

    yisint = 0;                 /* Y - не целое */

    s = y + m_h;
    absyy.value = s;
//#if __iset__ < 6
#if 1 /* see below */
    absyy.llong = __builtin_e2k_pandd (absyy.llong, 0xffffffff00000000LL);
#endif /*  __iset__ < 6 */
    y = y - (absyy.value - m_h);

    if (signx) {                 /* X < 0        */
        if (absy.llong >= 0x4340000000000000LL) /* |Y| >= 2^53  */
            yisint = 2;         /* Y - четное   */
        else if (absy.llong >= 0x4330000000000000LL) /* |Y| >= 2^52  */
            yisint = 2  - (absy.llong & 1);         /* Y - уже целое */
        else {
#if __iset__ <= 2
            m_h = absy.value;
            absy.value += DVAIN52;
            m_l = absy.value - DVAIN52;
            if (m_h == m_l)
                yisint = 2 - (absy.llong & 1);
#else /*  __iset__ <= 2 */
#pragma asm_inline
            __asm ("fdtoifd 0x3,%1,%0" : "=r" (m_h): "r" (absy.value));
            if (m_h == absy.value)
                yisint = 2 - (((LL) absy.value) & 1);
#endif /*  __iset__ <= 2 */
        }
    }
    if (unlikely (absx.llong == 0)) { /* X = 0 */
        if (signy) {
            i0 = (yisint == 1) ? 23 : 43; /* pow(-0.0,negative) : pow(+0.0,negative) */
            goto ERROR;
        }
        return (yisint == 1) ? X : 0;
    }

    absxx.value = Y;
//#if __iset__ >= 6
//FIXME: эта технология не работает. В случае переполнения получаются s и yd как inf, но разных знаков, а от их суммы -> nan
#if 0
    lres.value = s = s * Y;
    lres1.llong = __builtin_e2k_fmsd (absyy.llong, absxx.llong, lres.llong);
    yd = lres1.value + Y * y;
#else /* __iset__ < 6 */
    absxx.llong &= 0xffffffff00000000LL;
    yd = (Y - absxx.value) * absyy.value + Y * y;
    s = absxx.value * absyy.value;
#endif /*  __iset__ < 6 */
    y = s + yd;

    if (unlikely (absx.llong == 0x7ff0000000000000LL)) {   /* |X| = Inf */
        if (signy)
            absx.value = 0;
        absx.llong += yisint << 63;
        return absx.value;
    }
    if (unlikely (signx && yisint == 0)) {
        i0 = 24;
        goto ERROR;
    }

    /* вычисление 2 ** (y/256) */
    u = s - y + yd;
    exp = (int) y;
    if (unlikely ((y > 262144) || (y == 262144 && u >= 0))) { /* pow overflow */
        i0 = 21;
        goto ERROR;
    }
    if (unlikely ((y < -275200) || (y == -275200 && u <= 0))) { /* pow underflow */
        i0 = 22;
ERROR:  return SVID_libm_err (X, Y, i0);
    }
#if __iset__ <= 2
    u = (DB) exp;
#else /*  __iset__ <= 2 */
#pragma asm_inline
    __asm ("fdtoifd 0x3,%1,%0" : "=r" (u): "r" (y));
#endif /*  __iset__ <= 2 */
    y = s - u + yd;
    ydv2 = y * y;
#if __iset__ >= 6
    ly.value = y;
    lyv2.value = ydv2;
    lyv4.value = ydv2 * ydv2;
    lres1.value = KB1 * y;
    lres.llong = __builtin_e2k_fmad (LKB3, ly.llong, LKB2);
    lres1.llong = __builtin_e2k_fmad (lres.llong, lyv2.llong, lres1.llong);
    lres.llong = __builtin_e2k_fmad (LKB5, ly.llong, LKB4);
    lres.llong = __builtin_e2k_fmad (lres.llong, lyv4.llong, lres1.llong);
    y = lres.value;
#else /* __iset__ < 6 */
    y = KB2 * y * (y + KB1 / KB2) + KB3 * y * ydv2 + ydv2 * ydv2 * (KB5 * y + KB4);
#endif /* __iset__ < 6 */
    i0 = exp & 255;
    u = constexp[i0];
#if __iset__ >= 6
    ly.value = y;
    lres.value = constexp_lo[i0];
    absy.value = u;
    lres.llong = __builtin_e2k_fmad (absy.llong, ly.llong, lres.llong);
    absx.value = lres.value + u;
#else /* __iset__ < 6 */
    absx.value = u * y + constexp_lo[i0] + u;
#endif /* __iset__ < 6 */
    exp >>= 8;
    if (exp < -1021)
        i0 = exp + 54;
    else
        i0 = exp;
    absx.llong = __builtin_e2k_paddd (absx.llong, (i0 << 52) + (yisint << 63));
    if (exp < -1021)
        return absx.value * K2IN54;
    else
        return absx.value;
}

strong_alias (__pow, __pow_finite)
weak_alias (__pow, pow)
weak_alias (__pow, __ieee754_pow)
