/* Copyright (c) 2016-2018 ZAO "MCST". All rights reserved. */

#include <math.h>
#include <math_private.h>

#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%  ðòïçòáííá  ÷ùþéóìñåô  ëïòåîø  ëõâéþåóëéê  éú  þéóìá è.%
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

static const int constcbrt_hi[] = {
    0x3ff00000,                 /* CBRT (1+ 0/32) */
    0x3ff02a3a,                 /* CBRT (1+ 1/32) */
    0x3ff0539d,                 /* CBRT (1+ 2/32) */
    0x3ff07c32,                 /* CBRT (1+ 3/32) */
    0x3ff0a402,                 /* CBRT (1+ 4/32) */
    0x3ff0cb18,                 /* CBRT (1+ 5/32) */
    0x3ff0f17b,                 /* CBRT (1+ 6/32) */
    0x3ff11733,                 /* CBRT (1+ 7/32) */
    0x3ff13c48,                 /* CBRT (1+ 8/32) */
    0x3ff160bf,                 /* CBRT (1+ 9/32) */
    0x3ff184a0,                 /* CBRT (1+10/32) */
    0x3ff1a7f0,                 /* CBRT (1+11/32) */
    0x3ff1cab6,                 /* CBRT (1+12/32) */
    0x3ff1ecf5,                 /* CBRT (1+13/32) */
    0x3ff20eb3,                 /* CBRT (1+14/32) */
    0x3ff22ff5,                 /* CBRT (1+15/32) */
    0x3ff250bf,                 /* CBRT (1+16/32) */
    0x3ff27116,                 /* CBRT (1+17/32) */
    0x3ff290fc,                 /* CBRT (1+18/32) */
    0x3ff2b076,                 /* CBRT (1+19/32) */
    0x3ff2cf88,                 /* CBRT (1+20/32) */
    0x3ff2ee34,                 /* CBRT (1+21/32) */
    0x3ff30c7e,                 /* CBRT (1+22/32) */
    0x3ff32a69,                 /* CBRT (1+23/32) */
    0x3ff347f8,                 /* CBRT (1+24/32) */
    0x3ff3652d,                 /* CBRT (1+25/32) */
    0x3ff3820c,                 /* CBRT (1+26/32) */
    0x3ff39e95,                 /* CBRT (1+27/32) */
    0x3ff3bacd,                 /* CBRT (1+28/32) */
    0x3ff3d6b5,                 /* CBRT (1+29/32) */
    0x3ff3f24f,                 /* CBRT (1+30/32) */
    0x3ff40d9d,                 /* CBRT (1+31/32) */
    0x3ff428a2                  /* CBRT (1+32/32) */
};

static const DB constcbrt_lo[] = {
    0.0000000000000000000e+00,  /* ÍÌ. Þ. CBRT (1+ 0/32) */
    7.8579519600520049752e-07,  /* ÍÌ. Þ. CBRT (1+ 1/32) */
    3.7673666123639144540e-07,  /* ÍÌ. Þ. CBRT (1+ 2/32) */
    2.0373631889234437228e-07,  /* ÍÌ. Þ. CBRT (1+ 3/32) */
    9.4167731924476502841e-07,  /* ÍÌ. Þ. CBRT (1+ 4/32) */
    6.7839350818053832465e-07,  /* ÍÌ. Þ. CBRT (1+ 5/32) */
    7.0349780532222453811e-07,  /* ÍÌ. Þ. CBRT (1+ 6/32) */
    7.9865934248749693213e-07,  /* ÍÌ. Þ. CBRT (1+ 7/32) */
    2.4296516061087964678e-07,  /* ÍÌ. Þ. CBRT (1+ 8/32) */
    7.1964771938168058764e-07,  /* ÍÌ. Þ. CBRT (1+ 9/32) */
    6.3458134731642995088e-07,  /* ÍÌ. Þ. CBRT (1+10/32) */
    8.7439958844311291036e-07,  /* ÍÌ. Þ. CBRT (1+11/32) */
    7.0309071823082681688e-08,  /* ÍÌ. Þ. CBRT (1+12/32) */
    3.4893177164593687050e-07,  /* ÍÌ. Þ. CBRT (1+13/32) */
    6.8241586362611126568e-07,  /* ÍÌ. Þ. CBRT (1+14/32) */
    7.2636156644220048039e-07,  /* ÍÌ. Þ. CBRT (1+15/32) */
    8.4075889827405804221e-07,  /* ÍÌ. Þ. CBRT (1+16/32) */
    2.0155465070491867402e-07,  /* ÍÌ. Þ. CBRT (1+17/32) */
    6.3247546034810251909e-07,  /* ÍÌ. Þ. CBRT (1+18/32) */
    8.9851947898363500558e-07,  /* ÍÌ. Þ. CBRT (1+19/32) */
    5.3477834749475000817e-07,  /* ÍÌ. Þ. CBRT (1+20/32) */
    8.1297638901028126586e-07,  /* ÍÌ. Þ. CBRT (1+21/32) */
    9.3851228241856377923e-07,  /* ÍÌ. Þ. CBRT (1+22/32) */
    9.2221247333644000216e-07,  /* ÍÌ. Þ. CBRT (1+23/32) */
    6.3648214624306418483e-07,  /* ÍÌ. Þ. CBRT (1+24/32) */
    7.7799073155337094285e-07,  /* ÍÌ. Þ. CBRT (1+25/32) */
    1.4926531456532462932e-08,  /* ÍÌ. Þ. CBRT (1+26/32) */
    7.1665785964421199494e-07,  /* ÍÌ. Þ. CBRT (1+27/32) */
    3.7768037176949580566e-07,  /* ÍÌ. Þ. CBRT (1+28/32) */
    2.0715930567360205508e-07,  /* ÍÌ. Þ. CBRT (1+29/32) */
    3.6653866541033171838e-07,  /* ÍÌ. Þ. CBRT (1+30/32) */
    9.2874847037664090444e-07,  /* ÍÌ. Þ. CBRT (1+31/32) */
    9.2965561535226721061e-07   /* ÍÌ. Þ. CBRT (1+32/32) */
};

static const DB constcbrt_dop[] = {
    1.0000000000000000000e+00,  /* (1+ 0/32)^(-23/3)  */
    7.8984697897848201063e-01,  /* (1+ 1/32)^(-23/3)  */
    6.2826780190387058708e-01,  /* (1+ 2/32)^(-23/3)  */
    5.0306969996951432554e-01,  /* (1+ 3/32)^(-23/3)  */
    4.0535045163425542378e-01,  /* (1+ 4/32)^(-23/3)  */
    3.2855137011016549294e-01,  /* (1+ 5/32)^(-23/3)  */
    2.6779898484907485478e-01,  /* (1+ 6/32)^(-23/3)  */
    2.1944272110726537646e-01,  /* (1+ 7/32)^(-23/3)  */
    1.8072708076278980043e-01,  /* (1+ 8/32)^(-23/3)  */
    1.4955706450572437262e-01,  /* (1+ 9/32)^(-23/3)  */
    1.2432885691225552332e-01,  /* (1+10/32)^(-23/3)  */
    1.0380660978591195672e-01,  /* (1+11/32)^(-23/3)  */
    8.7032070201889534660e-02,  /* (1+12/32)^(-23/3)  */
    7.3257803809737400823e-02,  /* (1+13/32)^(-23/3)  */
    6.1897502241360115901e-02,  /* (1+14/32)^(-23/3)  */
    5.2488753136137985519e-02,  /* (1+15/32)^(-23/3)  */
    4.4664966635216119213e-02,  /* (1+16/32)^(-23/3)  */
    3.8134075338481278524e-02,  /* (1+17/32)^(-23/3)  */
    3.2662277721039947154e-02,  /* (1+18/32)^(-23/3)  */
    2.8061560410016949992e-02,  /* (1+19/32)^(-23/3)  */
    2.4180068819661198027e-02,  /* (1+20/32)^(-23/3)  */
    2.0894637140174549464e-02,  /* (1+21/32)^(-23/3)  */
    1.8104964397813803747e-02,  /* (1+22/32)^(-23/3)  */
    1.5729051980329681542e-02,  /* (1+23/32)^(-23/3)  */
    1.3699612824882235516e-02,  /* (1+24/32)^(-23/3)  */
    1.1961232723228675405e-02,  /* (1+25/32)^(-23/3)  */
    1.0468116561022139692e-02,  /* (1+26/32)^(-23/3)  */
    9.1822915454552396846e-03,  /* (1+27/32)^(-23/3)  */
    8.0721690323500153804e-03,  /* (1+28/32)^(-23/3)  */
    7.1113889412106112856e-03,  /* (1+29/32)^(-23/3)  */
    6.2778877713813190766e-03,  /* (1+30/32)^(-23/3)  */
    5.5531442457804519263e-03,  /* (1+31/32)^(-23/3)  */
    4.9215666011518482999e-03   /* (1+32/32)^(-23/3)  */
};

DB __cbrt (DB x)

#define K1 (1.0/3)
#define K2 (5.0/27)
#define K3 (10.0/81)
#define K4 (22.0/729)
#define K5 (7.0/9)
#define K6 (17.0/27)
#define K7 (85.0/162)
#define LK1 0x3fd5555555555555LL
#define LK2 0x3fc7b425ed097b42LL
#define LK3 0x3fbf9add3c0ca458LL
#define LK4 0x3f9ee7113506ac12LL
#define LK5 0x3fe8e38e38e38e39LL
#define LK6 0x3fe425ed097b425fLL
#define LK7 0x3fe0ca4587e6b74fLL

#define KCBRT2 1.2599210498948731648
#define KCBRT2_lo 9.2965561535226721061e-07
#define KCBRT4 1.5874010519681994748
#define KCBRT4_lo 6.1556683228725170564e-07
{
    _type_double_bits arg, x0, por, por_lo, por_hi;
    DB y, x0v2, dxv2;
    LL ind, exp, ost;
#if __iset__ >= 6
    _type_double_bits ly, lyv2, ldv2, lres, lres1, lres2;
#endif /* __iset__ < 6 */

    arg.value = dfabs (x);
    exp = exponentabs (arg);
    if (__glibc_unlikely (arg.llong < 0x10000000000000LL)) { /* 0 or subnormal x */
        if (arg.llong == 0)
            return x;                                  /* +-0 */
        arg.value *= (DVAIN52 * 4.0);
        exp = exponentabs (arg);
        ind = 359;
    }
    else ind = 341;
    if (__glibc_unlikely (arg.llong >= 0x7ff0000000000000LL))
        return x + x;                                       /* NaN or Inf */
    exp *= 2731;
    ost = exp >> 11;
    exp = ((exp >> 13) - ind) << 52;
    x0.value = x;
    exp += x0.llong & 0x8000000000000000LL;
    ind = arg.llong & 0xfffffffffffffLL;
    arg.llong = ind | 0x3ff0000000000000LL;
    ind = (ind + 0x400000000000LL) & 0x1f800000000000LL;
    x0.llong = ind + 0x3ff0000000000000LL;
    arg.value -= x0.value;
    x0v2 = x0.value * x0.value;
    dxv2 = arg.value * arg.value;
    y = K1 * arg.value;
#if __iset__ >= 6
    lyv2.value = x0v2;
    ldv2.value = dxv2;
    lres.value = K2 * x0.value;
    lres.llong = __builtin_e2k_fmsd (LK3, arg.llong, lres.llong);
    lres1.value = dxv2 * lres.value;
    lres.value = y - x0.value;
    lres2.llong = __builtin_e2k_fmad (lres.llong, lyv2.llong, lres1.llong);
    lres2.value *= y * (x0v2 * x0v2);

    lres1.value = K6 * x0.value;
    lres1.llong = __builtin_e2k_fmsd (LK7, arg.llong, lres1.llong);
    lres.llong = __builtin_e2k_fmsd (LK5, arg.llong, x0.llong);
    lres.value = x0v2 * lres.value;
    lres1.llong = __builtin_e2k_fmad (lres1.llong, ldv2.llong, lres.llong);
    lres.value = K4 * arg.value * (dxv2 * dxv2);
    lres.llong = __builtin_e2k_fmad (lres1.llong, lres.llong, lres2.llong);
    y = lres.value;
#else /* __iset__ < 6 */
    y = y * (x0v2 * x0v2) * (x0v2 * (y - x0.value) + dxv2 * (K3 * arg.value - K2 * x0.value))
        + K4 * arg.value * (dxv2 * dxv2) * (x0v2 * (K5 * arg.value - x0.value)
        + dxv2 * (K7 * arg.value - K6 * x0.value));
#endif /* __iset__ < 6 */
    x0.llong = (LL) ((unsigned *) constcbrt_hi)[ind >>= 47] << 32;
    por.value = 1.0;
    por_lo.value = 0.0;
    if (ost & 3) {
        if (ost & 1) {
            por.value = KCBRT2;
            por_lo.value = KCBRT2_lo;
        }
        else {
            por.value = KCBRT4;
            por_lo.value = KCBRT4_lo;
        }
        por_lo.llong += exp;
    }
    por.llong = __builtin_e2k_paddd (por.llong, exp);
//    por.llong += exp;
    por_hi.llong = por.llong & ~0xffffffffLL;
#if __iset__ >= 6
    ly.value = y;
    lres.value = constcbrt_dop[ind] * por.value;
    lres1.value = por_lo.value * x0.value + constcbrt_lo[ind] * por.value;
    ly.llong = __builtin_e2k_fmsd (lres.llong, ly.llong, lres1.llong);
    y = ly.value;
#else /* __iset__ < 6 */
    y = constcbrt_dop[ind] * por.value * y - (por_lo.value * x0.value + constcbrt_lo[ind] * por.value);
#endif /* __iset__ < 6 */
    return x0.value * por_hi.value - y;
}
weak_alias (__cbrt, cbrt)
#ifdef NO_LONG_DOUBLE
strong_alias (__cbrt, __cbrtl)
weak_alias (__cbrt, cbrtl)
#endif
