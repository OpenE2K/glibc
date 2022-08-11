/* Copyright (c) 2016-2018 ZAO "MCST". All rights reserved. */

#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ðòïçòáííá  ÷ùþéóìñåô  úîáþåîéå  æõîëãéé  'ôáîçåîó  %
%  çéðåòâïìéþåóëéê'.                                     %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

DB __tanh (DB X)

#define KB13 -1.93526536869223498289e-06
#define KB12 1.09388960993197729440e-05
#define KB11 -3.56739259770914388058e-05
#define KB10 9.51440628782986401958e-05
#define KB9 -2.38526036139274191152e-04
#define KB8 5.89884392388881163258e-04
#define KB7 -1.45581095070534112012e-03
#define KB6 3.59212544796965318466e-03
#define KB5 -8.86323534557734958087e-03
#define KB4 2.18694885283180994828e-02
#define KB3 -5.39682539680803678316e-02
#define KB2 1.33333333333331818811e-01
#define KB1 -3.33333333333333331139e-01
#define LKB13 0xbec03bf3634a7168LL
#define LKB12 0x3ee6f0c66e997b72LL
#define LKB11 0xbf02b412c35ca2c6LL
#define LKB10 0x3f18f1028dcb4e22LL
#define LKB9  0xbf2f439b0c865fe5LL
#define LKB8  0x3f43544f163cd1e5LL
#define LKB7  0xbf57da1d1b0939d9LL
#define LKB6  0x3f6d6d3baa4f206aLL
#define LKB5  0xbf8226e34f916b95LL
#define LKB4  0x3f9664f48809992fLL
#define LKB3  0xbfaba1ba1ba15861LL
#define LKB2  0x3fc11111111110dbLL
#define LKB1  0xbfd5555555555555LL

#define KA5 8.33333478792298551112e-03
#define KA4 4.16666768487944911020e-02
#define KA3 1.66666666666665333693e-01
#define KA2 4.99999999999990669185e-01
#define LKA5 0x3f811111430bc5c6LL
#define LKA4 0x3fa55555accc11b8LL
#define LKA3 0x3fc5555555555525LL
#define LKA2 0x3fdfffffffffff58LL
{
    _type_double_bits arg, exp, ed, con;
    DB y, yv2, yv4, yp;
    LL k, ind;
#if __iset__ >= 6
    _type_double_bits ly, lyv2, lyv4, lyv8, lyv16, lres, lres1, lres2;
#endif /* __iset__ < 6 */

    ed.value = X;
    ed.llong = (ed.llong & 0x8000000000000000LL) | 0x3ff0000000000000LL;

    if (X != X)
        return X + X;           /* X == NaN */
    arg.value = dfabs (X);
    if (arg.llong <= 0x3fe6666666666666LL) {       /* |X| <= 0.7 */
        yv2 = X * X;
        yv4 = yv2 * yv2;
        yp = yv4 * yv4;
#if __iset__ >= 6
        lyv2.value = yv2;
        lyv4.value = yv4;
        lyv8.value = yp;
        lyv16.value = yp * yp;
        lres.value = KB1 * yv2;
        lres1.llong = __builtin_e2k_fmad (LKB3, lyv2.llong, LKB2);
        lres2.llong = __builtin_e2k_fmad (lres1.llong, lyv4.llong, lres.llong);
        lres.llong = __builtin_e2k_fmad (LKB5, lyv2.llong, LKB4);
        lres1.llong = __builtin_e2k_fmad (LKB7, lyv2.llong, LKB6);
        lres1.llong = __builtin_e2k_fmad (lres1.llong, lyv4.llong, lres.llong);
        lres2.llong = __builtin_e2k_fmad (lres1.llong, lyv8.llong, lres2.llong);
        lres.llong = __builtin_e2k_fmad (LKB9, lyv2.llong, LKB8);
        lres1.llong = __builtin_e2k_fmad (LKB11, lyv2.llong, LKB10);
        lres1.llong = __builtin_e2k_fmad (lres1.llong, lyv4.llong, lres.llong);
        lres.llong = __builtin_e2k_fmad (LKB13, lyv2.llong, LKB12);
        lres1.llong = __builtin_e2k_fmad (lres.llong, lyv8.llong, lres1.llong);
        lres.llong = __builtin_e2k_fmad (lres1.llong, lyv16.llong, lres2.llong);
        arg.value = X;
        lres.llong = __builtin_e2k_fmad (lres.llong, arg.llong, arg.llong);
        return lres.value;
#else /* __iset__ < 6 */
        return (KB2 * yv2 * (KB1 / KB2 + yv2) + KB3 * yv2 * yv4 + yp * (KB4 +
            KB5 * yv2) + KB9 * yv4 * yp * (KB6 / KB9 + KB7 / KB9 * yv2 + yv4 * (yv2 + KB8 / KB9)) +
            KB13 * yv4 * (KB10 / KB13 + KB11 / KB13 * yv2 + yv4 * (yv2 + KB12 / KB13)) * (yp * yp)) * X + X;
#endif /* __iset__ < 6 */
    }
    if (arg.llong > 0x4036000000000000LL) {        /* |X| > 22 */
        return ed.value;
    }
    y = arg.value * (2.0 * K256ONLN2);
    k = (LL) y;
#if __iset__ <= 2
    y = (DB) k;
#else /*  __iset__ <= 2 */
#pragma asm_inline
    __asm ("fdtoifd 0x3,%1,%0" : "=r" (y): "r" (y));
#endif /*  __iset__ <= 2 */
#if __iset__ >= 6
    ly.value = y;
    lres.value = arg.value + arg.value;
    lres.llong = __builtin_e2k_fnmad (LKLN2ON256_hi, ly.llong, lres.llong);
    y = lres.value - y * KLN2ON256_lo;
#else /* __iset__ < 6 */
    y = (arg.value + arg.value) - y * KLN2ON256_hi - y * KLN2ON256_lo;
#endif /* __iset__ < 6 */
    yv2 = y * y;
#if __iset__ >= 6
    ly.value = y;
    lyv2.value = yv2;
    lyv4.value = yv2 * yv2;
    lres.llong = __builtin_e2k_fmad (LKA3, ly.llong, LKA2);
    lres1.llong = __builtin_e2k_fmad (lres.llong, lyv2.llong, ly.llong);
    lres.llong = __builtin_e2k_fmad (LKA5, ly.llong, LKA4);
    lres.llong = __builtin_e2k_fmad (lres.llong, lyv4.llong, lres1.llong);
    y = lres.value;
#else /* __iset__ < 6 */
    y = KA2 * y * (y + 1 / KA2) + KA3 * y * yv2 + yv2 * yv2 * (KA5 * y + KA4);
#endif /* __iset__ < 6 */
    ind = k & 255;
    k = ((k >> 8) - 1) << 52;
    con.value = constexp[ind];
    exp.value = constexp_lo[ind];
    con.llong = __builtin_e2k_paddd (con.llong, k);
    exp.llong = __builtin_e2k_paddd (exp.llong, k);
#if __iset__ >= 6
    ly.value = y;
    lres.llong = __builtin_e2k_fmad (con.llong, ly.llong, exp.llong);
    yp = lres.value + con.value;
#else /* __iset__ < 6 */
    yp = con.value * y + exp.value + con.value;
#endif /* __iset__ < 6 */
    y = yp + 0.5;
    if (arg.llong > 0x3ff0000000000000LL)  /* |X| > 1 */
        return (ed.value - ed.value / y);
    else
        return ((yp - 0.5) * ed.value / y);
}

weak_alias (__tanh, tanh)
#ifdef NO_LONG_DOUBLE
strong_alias (__tanh, __tanhl)
weak_alias (__tanh, tanhl)
#endif
