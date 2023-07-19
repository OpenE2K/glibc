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

DB __atan2 (DB Y, DB X)

#define KB5 1.99999455044662533610e-01
#define KB3 -3.33333333333073478475e-01
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

{
    _type_double_bits arg1, arg2, max;
    DB y, b, min, y0, yv2, yv4, yv8;
    LL ind, signx, signy;
#if __iset__ >= 6
    _type_double_bits ly, ly0, lyv2, lyv4, lyv8, lres, lres1;
#endif /* __iset__ < 6 */

// glibc ÚÄÅÓØ ÏÛÉÂËÉ ÎÅ ÇÅÎÅÒÉÔ, Á ×ÙÄÁÅÔ +-0 ÉÌÉ +-pi
//    if ((X == 0.0) && (Y == 0.0))
//        return SVID_libm_err (Y, X, 3);

    arg1.value = X;
    signx = (arg1.llong >> 62) & 2;
    arg2.value = Y;
    signy = (arg2.llong >> 62) & 2;

    arg1.value = X;
    arg2.value = Y;
    arg1.llong = __builtin_e2k_pandd (arg1.llong, 0x7fffffffffffffffLL);
    arg2.llong = __builtin_e2k_pandd (arg2.llong, 0x7fffffffffffffffLL);
    b = arg1.value;

    if (!((Y == Y) && (X == X))) {
        /* If x or y is NaN, NaN is returned */
        return X + Y;
    }
    if (arg2.twolong.int0 >= 0x7ff00000) {
        /* |Y| == Inf */
        if (arg1.twolong.int0 >= 0x7ff00000) {  /* |X| == Inf */
            /* atan2(+/-Inf,-Inf) returns +/-3pi/4 */
            /* atan2(+/-Inf,Inf) returns +/-pi/4 */
            y0 = (DB) ((signx + 1) * (1 - signy));
            return (PION2 * 0.5) * y0;
        }
        /* atan2(+/-Inf,x) returns +/-pi/2 for finite x */
        return PION2 * ((DB) (1 - signy));
    }
    if (arg1.twolong.int0 >= 0x7ff00000 || Y == 0) {
        /* |X| == Inf or |Y| == 0 */
        /* atan2(+/-y,-Inf) returns +/-pi for finite |y| > 0; */
        /* atan2(+/-0,x) returns +/-pi for x < 0 or x = -0 */
        /* atan2(+/-y,Inf) returns +/-0 for finite |y| > 0 */
        /* atan2(+/-0,x) returns +/-0 for x > 0 or x = +0 */
        arg1.value = ((DB) (signx >> 1)) * PI;
        arg1.llong |= signy << 62;
        return arg1.value;
    }

    if (b < arg2.value) {
        y = b / arg2.value;
        min = b;
        max.value = arg2.value;
    }
    else {
        y = arg2.value / b;
        min = arg2.value;
        max.value = b;
    }
    arg1.value = y;
    if (arg1.twolong.int0 < 0x3fc00000) {    /* |arg|<0.125 */
        if (max.twolong.int0 < 0x3fc00000) {
            max.value *= DVAIN1022;
            min *= DVAIN1022;
        }
        arg1.llong = __builtin_e2k_pandd (arg1.llong, 0xffffffff00000000LL);
        y0 = max.value;
        max.llong = __builtin_e2k_pandd (max.llong, 0xffffffff00000000LL);
#if __iset__ >= 6
        lres.value = min;
        lres.llong = __builtin_e2k_fmsd (arg1.llong, max.llong, lres.llong);
        y0 = (lres.value + arg1.value * (y0 - max.value)) * (1.0 / y0);
#else /* __iset__ < 6 */
        y0 = (arg1.value * max.value - min + arg1.value * (y0 - max.value)) * (1.0 / y0);
#endif /* __iset__ < 6 */
        yv2 = y * y;
        yv4 = yv2 * yv2;
        yv8 = yv4 * yv4;
#if __iset__ >= 6
        ly.value = y * yv2;
        ly0.value = y0;
        lyv2.value = yv2;
        lyv4.value = yv4;
        lyv8.value = yv8;
        lres.llong = __builtin_e2k_fmad (LKA5, lyv2.llong, LKA3);
        lres1.llong = __builtin_e2k_fmad (LKA9, lyv2.llong, LKA7);
        lres.llong = __builtin_e2k_fmad (lres1.llong, lyv4.llong, lres.llong);
        lres1.llong = __builtin_e2k_fmad (LKA15, lyv2.llong, LKA13);
        lres1.llong = __builtin_e2k_fmad (lres1.llong, lyv2.llong, LKA11);
        lres.llong = __builtin_e2k_fmad (lres1.llong, lyv8.llong, lres.llong);
        lres.llong = __builtin_e2k_fnmad (ly.llong, lres.llong, ly0.llong);
        if (y != 0) y0 = lres.value; /* for y = -0.0 only */
#else /* __iset__ < 6 */
        y0 -= KA5 * y * yv2 * (KA3 / KA5 + yv2) + KA7 * y * yv2 * yv4 + KA11 * y *
              (KA9 / KA11 + yv2) * yv8 + y * yv4 * yv8 * (KA13 + KA15 * yv2);
#endif /* __iset__ < 6 */
        if (arg1.twolong.int0 < 0x3f000000) {
            y0 -= arg1.value;
            arg1.value = 0.0;
        }
    }
    else {
        arg1.llong = __builtin_e2k_paddd (arg1.llong, 0x100000000000LL);
        arg1.llong = __builtin_e2k_pandd (arg1.llong, 0x7fffe00000000000LL);
        ind = (arg1.llong >> 45) - 0x1fe00;
#if __iset__ >= 6
        lres.value = y;
        lres.llong = __builtin_e2k_fmad (lres.llong, arg1.llong, 0x3ff0000000000000LL);
        y = (arg1.value - y) / lres.value;
#else /* __iset__ < 6 */
        y = (arg1.value - y) / (1.0 + y * arg1.value);
#endif /* __iset__ < 6 */
        yv2 = y * y;
#if __iset__ >= 6
        ly.value = y - constarctg_lo[ind];
        arg1.value = KB5 * y * yv2;
        lres.value = KB3 / KB5 + yv2;
        lres.llong = __builtin_e2k_fmad (arg1.llong, lres.llong, ly.llong);
        y0 = lres.value;
#else /* __iset__ < 6 */
        y0 = y - constarctg_lo[ind] + KB5 * y * yv2 * (KB3 / KB5 + yv2);
#endif /* __iset__ < 6 */
        arg1.llong = ((ULL) ((unsigned *) constarctg)[ind]) << 32;
    }
    if (arg2.value > b) {
        if (signx) {
            return (signy) ? (-PION2_lo + y0) - (PION2 + arg1.value) : (PION2 + arg1.value) - (-PION2_lo + y0);
        }
        return (signy) ? (-PION2_lo - y0) - (PION2 - arg1.value) : (PION2 - arg1.value) - (-PION2_lo - y0);
    }
    else if (signx) {
        return (signy) ? (-PI_lo - y0) - (PI - arg1.value) : (PI - arg1.value) - (-PI_lo - y0);
    }
    return (signy) ? y0 - arg1.value : arg1.value - y0;
}

strong_alias (__atan2, __atan2_finite)
weak_alias (__atan2, atan2)
weak_alias (__atan2, __ieee754_atan2)
