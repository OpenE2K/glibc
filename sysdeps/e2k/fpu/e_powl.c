/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ПРОГРАММА  ВЫЧИСЛЯЕТ  ЗНАЧЕНИЕ  ФУНКЦИИ  X ** Y .  %
%  НЕДОПУСТИМЫМИ  ЗНАЧЕНИЯМИ  АРГУМЕНТОВ ЯВЛЯЮТСЯ :      %
%  1. X = 0  И  Y <= 0 ;                                 %
%  2. X < 0  И  Y - НЕ ЦЕЛОЕ ЧИСЛО ;                     %
%  3. |X ** Y| > 2 ** 16384 ;                            %
%  4. |X ** Y| < 2 ** -16446 ;                           %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define KB6  3.5023605298761446389230e-17
#define KB5  3.8805808982911052890212e-14
#define KB4  3.5830323053986465190648e-11
#define KB3  2.6466421444307261308017e-08
#define KB2  1.4662262387640424338771e-05L
#define KB1  5.4152123481245727327856e-03L
#define KA7  5.2761730505011613442926e+01L
#define KA5  7.3865986092774273090647e+01L
#define KA3  1.2310997682252487799092e+02L
#define KA1  3.6932993046757463228414e+02L
#define KA1_h 369.328125 /* 0x4077154000000000 */
#define KA1_l 1.80546757463228414065e-03L
#define KINVLN2_hi (KA1_h / 128)
#define KINVLN2_lo (KA1_l / 128)
//#define KINVLN2_lo 1.41052154268147198486e-05L

LD __powl (LD X, LD Y)
{
    _type_long_double_bits absx, absy, absxx, absyy;
    _type_double_bits con;
    LD y, yd, ydv2, u, s, s_l, m_h, m_l;
    LL im;
    int i0, yisint, signx, signy, exp;

    absy.value = Y;
    signy = (absy.twolong.short0 >> 15) & 1;
    absx.value = X;
    signx = (absx.twolong.short0 >> 15) & 1;
    exp = absx.twolong.short0 & 0x7fff;
    absy.value = ldfabs (Y);
    absx.value = ldfabs (X);
    if (exp)
        exp -= 16383;
    else {
        absx.value *= DVAIN63;
        exp = (absx.twolong.short0 & 0x7fff) - (16383 + 63);
    }

    /* вычисление y + u = 128 * log2(X) * Y */
    absxx.twolong.short0 = 0x3fff;
    absxx.twolong.long1 = absx.twolong.long1;
    im = absx.twolong.long1 + 0x0080000000000000LL;
    if (im >= 0) { /* то есть было переполнение мантиссы */
        absyy.value = 2.0L;
        exp++;
    }
    else {
        absyy.twolong.short0 = 0x3fff;
        absyy.twolong.long1 = im & 0xff00000000000000ULL;
    }
    i0 = (im >> 56) & 0x7f;
    u = absxx.value - absyy.value;
    yd = absxx.value + absyy.value;

    if (Y == 0) {
        if (X != X) { /* pow (NaN, 0.0) */
            i0 = 242;
            goto ERROR;
//return 1;
        }
        if (X == 0) { /* pow (0.0, 0.0) */
            i0 = 220;
            goto ERROR;
//return 1;
        }
        return 1;
    }
    s = u / yd;
    ydv2 = s * s;
    absxx.value = s;
    absxx.twolong.long1 &= 0xffffffff00000000ULL;
    s_l = KA1 / yd * (u - (absyy.value + absyy.value)
                      * absxx.value - u * absxx.value);

    if (X != X || Y != Y) { /* X или Y == NaN */
        return X == 1 ? 1 : X + Y;
    }
    con.llong = ((LL) constln[i0]) << 32;
    y = con.value * (KINVLN2_hi * 64) + (LD) (exp << 7);
    yd = absxx.value * KA1_h;

    if (Y == 1) { /* быстрый выход для специальных значений */
        return X;
    }
    m_h = y + yd;
    m_l = m_h - y - yd;

    if ((absy.twolong.short0 & 0x7fff) == 0x7fff) {   /* |Y| = Inf */
        if (absx.value == 1) {
            return 1;       /* +-1 ** +-Inf */
        }
        return ((absx.value < 1) != signy) ? 0 : absy.value;
    }

    y = absxx.value * KA1_l + constln_lo[i0] * (KA1 * 0.5) + con.value * (KINVLN2_lo * 64) +
        s * ydv2 * (KA3 + KA7 * ydv2 * (KA5 / KA7 + ydv2)) - (m_l - s_l);
    s = y + m_h;
    absyy.value = s;
    absyy.twolong.long1 &= 0xffffffff00000000ULL;
    y = y - (absyy.value - m_h);

    yisint = 0;                 /* Y - не целое */

    if (signx) {                  /* X < 0        */
        if (absy.value >= (DVAIN63 * 2)) /* |Y| >= 2^64  */
            yisint = 2;         /* Y - четное   */
        else if (absy.value >= DVAIN63) /* |Y| >= 2^63  */
            yisint = 2  - (absy.twolong.long1 & 1);         /* Y - уже целое */
        else {
            m_h = absy.value;
            absy.value += DVAIN63;
            m_l = absy.value - DVAIN63;
            if (m_h == m_l)
                yisint = 2 - (absy.twolong.long1 & 1);
        }
    }
    if (absx.value == 0) {
        if (signy) {
            i0 = (yisint == 1) ? 223 : 243; /* pow(-0.0,negative) : pow(+0.0,negative) */
            goto ERROR;
//return (yisint == 1 && signx) ? -Inf_LD : Inf_LD;
        }
        return (yisint == 1) ? X : 0;
    }
    absxx.value = Y;
    absxx.twolong.long1 &= 0xffffffff00000000ULL;
    yd = (Y - absxx.value) * absyy.value + Y * y;
    s = absxx.value * absyy.value;
    y = s + yd;

    if ((absx.twolong.short0 & 0x7fff) == 0x7fff) {   /* |X| = Inf */
        if (signy)
            absx.value = 0;
        if (yisint == 1)
            absx.value = -absx.value;
        return absx.value;
    }
    if (signx && yisint == 0) { /* pow neg**non-int */
        i0 = 224;
        goto ERROR;
//return NaN_LD;
    }

    /* вычисление 2 ** ((y+u)/128) */
    u = s - y + yd;
    if ((y > 2097152) || (y == 2097152 && u >= 0)) { /* pow overflow */
        i0 = 221;
        goto ERROR;
//return (X<0 && (yisint & 1)) ? -Inf_LD : Inf_LD;
    }
    if ((y < -2105088) || (y == -2105088 && u <= 0)) { /* pow underflow */
        i0 = 222;
ERROR:  return __kernel_standard_l (X, Y, i0);
//ERROR:  return (X<0 && (yisint & 1)) ? -0 : 0;
    }
    exp = (int) y;
    u = (LD) exp;
    y = s - u + yd;
    ydv2 = y * y;
    y = y * (KB1 + KB2 * y + KB3 * ydv2) + ydv2 * ydv2 * (KB4 + KB5 * y + KB6 * ydv2);
    i0 = exp & 127;
    u = constexpl[i0];
    absx.value = u * y + constexpl_lo[i0] + u;
    exp >>= 7;
    if (exp < -16382)
        i0 = exp + 110;
    else
        i0 = exp;
    absx.twolong.short0 += i0 + (yisint << 15);
    if (exp < -16382)
        return absx.value * K2IN110;
    else
        return absx.value;
}

strong_alias (__powl, __powl_finite)
weak_alias (__powl, powl)
weak_alias (__powl, __ieee754_powl)
