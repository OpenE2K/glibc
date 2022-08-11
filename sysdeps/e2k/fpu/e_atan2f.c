/* Copyright (c) 2016-2018 ZAO "MCST". All rights reserved. */

#include "f2c.h"
#include <e2kintrin.h>

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ðòïçòáííá  ÷ùþéóìñåô  úîáþåîéå  õçìá  æé,  ïðòåäå- %
%  ìñåíïçï  áòçõíåîôáíé  è  é  õ.  îåäïðõóôéíùíé  úîáþå- %
%  îéñíé  áòçõíåîôï÷  ñ÷ìñàôóñ  è = 0 é õ = 0 .          %
%        æé  =  0 , åóìé  Y = 0  é  X > 0 ,              %
%        æé  = ðé , åóìé  Y = 0  é  X < 0 ,              %
%  ÷  ïóôáìøîùè  óìõþáñè  -ðé < æé < ðé . òåúõìøôáô  ÷   %
%  òáäéáîáè .                                            %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#if __iset__ >= 6
#define KB19 -1.7011700640685973283e-03
#define KB17  1.0487649254949306388e-02
#define KB15 -3.0351864785781172001e-02
#define KB13  5.7089555930306193354e-02
#define KB11 -8.3497249683016014354e-02
#define KB9   1.0932341501303093068e-01
#define KB7  -1.4260016082820906047e-01
#define KB5   1.9998075281148713911e-01
#define KB3  -3.3333276291982549822e-01
#define KB1   9.9999999716054543963e-01
#define LKB19 0xbf5bdf39729124b6LL
#define LKB17 0x3f857a8c747ed314LL
#define LKB15 0xbf9f148f2a829af1LL
#define LKB13 0x3fad3ad79f53e142LL
#define LKB11 0xbfb5601364b1cac3LL
#define LKB9  0x3fbbfc9e8c2b0243LL
#define LKB7  0xbfc240b8d994abf9LL
#define LKB5  0x3fc998f82493d066LL
#define LKB3  0xbfd55552f0db4320LL
#define LKB1  0x3feffffffe79bf93LL
#else /* __iset__ < 6 */
#define KA9 1.00523913835726148843e-01
#define KA7 -1.42452689179072812273e-01
#define KA5 1.99994988952939798604e-01
#define KA3 -3.33333323562239806408e-01
#define KB3 -3.33327229950751257532e-01
#endif

FL __atan2f (FL Y, FL X)
{
    _type_float_bits arg1, arg2;
    _type_double_bits darg;
    DB y, yv2;
    int signx, signy;
#if __iset__ >= 6
    _type_double_bits lyv2, lyv4, lyv5, lres, lres1, lres2;
#else /* __iset__ < 6 */
    int ind;
#endif /* __iset__ < 6 */

    arg1.value = X;
    signx = (arg1.int0 >> 30) & 2;
    arg2.value = Y;
    signy = (arg2.int0 >> 30) & 2;
    arg1.value = ffabs (X);
    arg2.value = ffabs (Y);

    if (!((Y == Y) && (X == X))) {
        /* If x or y is NaN, NaN is returned */
        return X + Y;
    }
    if (arg2.int0 >= 0x7f800000) {
        /* |Y| == Inf */
        if (arg1.int0 >= 0x7f800000) {  /* |X| == Inf */
            /* atan2(+/-Inf,-Inf) returns +/-3pi/4 */
            /* atan2(+/-Inf,Inf) returns +/-pi/4 */
            FL y0 = (FL) ((signx + 1) * (1 - signy));
            return y0 * (FL) (PION2 * 0.5);
        }
        /* atan2(+/-Inf,x) returns +/-pi/2 for finite x */
        return (FL) (1 - signy) * (FL) PION2;
    }
    if (arg1.int0 >= 0x7f800000 || Y == 0) {
        /* |X| == Inf or |Y| == 0 */
        /* atan2(+/-y,-Inf) returns +/-pi for finite |y| > 0; */
        /* atan2(+/-0,x) returns +/-pi for x < 0 or x = -0 */
        /* atan2(+/-y,Inf) returns +/-0 for finite |y| > 0 */
        /* atan2(+/-0,x) returns +/-0 for x > 0 or x = +0 */
        arg1.value = (FL) (signx >> 1) * (FL) PI;
        arg1.int0 += (signy << 30);
        return arg1.value;
    }

    y = (DB) arg1.value;
    yv2 = (DB) arg2.value;
    darg.value = (arg2.int0 > arg1.int0) ? y / yv2 : yv2 / y;

#if __iset__ >= 6
    lyv2.value = darg.value * darg.value;
    lyv4.value = lyv2.value * lyv2.value;
    lres.value = KB1 * darg.value;
    lres1.value = KB3 * darg.value;
    lres2.llong = __builtin_e2k_fmad (lres1.llong, lyv2.llong, lres.llong);
    lres.llong = __builtin_e2k_fmad (LKB7, lyv2.llong, LKB5);
    lres1.llong = __builtin_e2k_fmad (LKB11, lyv2.llong, LKB9);
    lres1.llong = __builtin_e2k_fmad (lres1.llong, lyv4.llong, lres.llong);
    lyv5.value = lyv4.value * darg.value;
    lres.llong = __builtin_e2k_fmad (lres1.llong, lyv5.llong, lres2.llong);
    y = lres.value;
    lres.llong = __builtin_e2k_fmad (LKB15, lyv2.llong, LKB13);
    lres1.llong = __builtin_e2k_fmsd (LKB19 ^ 0x8000000000000000LL, lyv2.llong, LKB17);
    lres.llong = __builtin_e2k_fmsd (lres1.llong, lyv4.llong, lres.llong);
    yv2 = lyv5.value * (lyv4.value * lyv4.value) * lres.value;
//    y = KB1 * darg.value + KB5 * darg.value * yv2 * (KB3/KB5 + yv2) +
//        darg.value * yv2 * yv4 * (KB7 + KB9 * yv2 + KB11 * yv4);
//    yv2 = -(KB19 * darg.value * yv4 * (yv4 * yv4) * (KB13/KB19 + KB15/KB19 * yv2 + yv4 * (yv2 + KB17/KB19)));
#else /* __iset__ < 6 */
    if (darg.twolong.int0 <= 0x3fd00000) { /* |x/y|<=0.25 */
        yv2 = darg.value * darg.value;
        y = darg.value + KA5 * darg.value * yv2 * (KA3/KA5 + yv2);
        yv2 = -KA9 * darg.value * yv2 * (KA7/KA9 + yv2) * (yv2 * yv2);
    }
    else {
        ind = (darg.twolong.int0 - 0x3fd00000) >> 14;
        y = darg.value;
        darg.llong = __builtin_e2k_pandd (darg.llong, 0x3fffc00000000000LL);
        y = (y - darg.value) / (1.0 + y * darg.value);
        yv2 = -KB3 * y * (y * y);
        y = y + constnarctg[ind];
    }
#endif /* __iset__ < 6 */
    if (arg2.int0 > arg1.int0) {
        if (signx) {
            return (FL) (signy) ? yv2 - (y + PION2) : (y + PION2) - yv2;
        }
        return (FL) (signy) ? (y - PION2) - yv2 : yv2 - (y - PION2);
    }
    else {
        if (signx) {
            return (FL) (signy) ? (y - PI) - yv2 : yv2 - (y - PI);
        }
        return (FL) (signy) ? yv2 - y : y - yv2;
    }
}

strong_alias (__atan2f, __atan2f_finite)
weak_alias (__atan2f, atan2f)
weak_alias (__atan2f, __ieee754_atan2f)
