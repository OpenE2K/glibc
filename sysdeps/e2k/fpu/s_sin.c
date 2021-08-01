/* Copyright (c) 2016-2018 ZAO "MCST". All rights reserved. */

#include <errno.h>
#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ðòïçòáííá  ÷ùþéóìñåô  úîáþåîéå  æõîëãéé  'óéîõó'.  %
%  áòçõíåîô  ÷  òáäéáîáè.                                %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#if 0
static LD CONSTLD[] = {
    8.3333333333331863840149e-03L,     /* KA5   */
    -1.6666666666666665307232e-01L,    /* KA3   */
    -3.1415926535897931159980,         /* PI_1  */
    1.2246467991473531772015e-16L,     /* PI_1T */
};
    LD KA5 = CONSTLD[0];   /* 8.3333333333331863840149e-03L   */
    LD KA3 = CONSTLD[1];   /* -1.6666666666666665307232e-01L  */
    LD PI_1 = CONSTLD[2];  /* -3.1415926535897931159980       */
    LD PI_1T = CONSTLD[3]; /* 1.2246467991473531772015e-16L   */
#endif

DB __sin (DB X)

#define KA17  2.7215749422983442541079e-15
#define KA15 -7.6430265579716326604206e-13
#define KA13  1.6058940878486559930625e-10
#define KA11 -2.5052106890569519539494e-08
#define KA9   2.7557319211229606622011e-06
#define KA7  -1.9841269841208675170001e-04
#define LKA17 0x3ce883864938575aLL
#define LKA15 0xbd6ae439ef902726LL
#define LKA13 0x3de6123ccd99b00cLL
#define LKA11 0xbe5ae6454d07634cLL
#define LKA9  0x3ec71de3a528d19bLL
#define LKA7  0xbf2a01a01a0147f4LL
#define KA5   8.3333333333331863840149e-03L
#define KA3  -1.6666666666666665307232e-01L
#define KA1   9.9999999999999999979295e-01L
#define RS   6.75539944105574400e+15    /* 3*2^51 = 0x4338000000000000 */

#define PI_1 -3.1415926535897931159980 /* 53bits */
#define LPI_1 0xc00921fb54442d18LL
#define PI_1T 1.2246467991473531772015e-16L

#define S5 -2.5027028261085481637890e-8 /* 0xbe5adf60, 0x8c9a6f5d */
#define S4  2.7557305510406862083145e-6 /* 0x3ec71de2, 0xe4566711 */
#define S3 -1.9841269838208986034121e-4 /* 0xbf2a01a0, 0x19f064f2 */
#define S2  8.3333333333330942114919e-3 /* 0x3f811111, 0x11111087 */
#define S1 -1.6666666666666666636777e-1 /* 0xbfc55555, 0x55555555 */
#define LS5 0xbe5adf608c9a6f5dLL
#define LS4 0x3ec71de2e4566711LL
#define LS3 0xbf2a01a019f064f2LL
#define LS2 0x3f81111111111087LL
#define LS1 0xbfc5555555555555LL
{
    type_union_64f darg0, darg1;
    DB t0, y0, xv2, xv4;
    LD ldx, ldxv2;
    int k0;
#if __iset__ >= 6
    _type_double_bits lyv2, lyv4, lyv8, lres, lres1, lres2;
#else /* __iset__ < 6 */
    DB xv6, xv8;
#endif /* __iset__ < 6 */

    darg0.db = X;
    darg0.ll &= ~0x8000000000000000LL;
    xv2 = X * X;
    xv4 = xv2 * xv2;

    if (X == 0) return X; /* for X = -0.0 only */

    if (darg0.ll <= 0x3fd0000000000000LL) { /* 0.25 */
#if __iset__ >= 6
        lyv2.value = xv2;
        lyv4.value = xv4;
        lres2.llong = __builtin_e2k_fmad (LS2, lyv2.llong, LS1);
        lres1.llong = __builtin_e2k_fmad (LS5, lyv2.llong, LS4);
        lres1.llong = __builtin_e2k_fmad (lres1.llong, lyv2.llong, LS3);
        lres.llong = __builtin_e2k_fmad (lres1.llong, lyv4.llong, lres2.llong);
        lyv8.value = X * xv2;
        lres1.value = X;
        lres.llong = __builtin_e2k_fmad (lres.llong, lyv8.llong, lres1.llong);
        return lres.value;
#else /* __iset__ < 6 */
        xv6 = xv2 * xv4;
        return X + (X * xv2) * (S1 + S3 * xv2 * (S2 / S3 + xv2) + xv6 * (S4 + S5 * xv2));
#endif /* __iset__ < 6 */
    }
    darg1.db = X * K1ONPI;
#if __iset__ <= 2
    darg1.db += RS;
    t0 = darg1.db - RS;
    k0 = darg1.two_int.i1;
#else /*  __iset__ <= 2 */
#pragma asm_inline
    __asm ("fdtoifd 0x0,%1,%0" : "=r" (t0): "r" (darg1.db)); /* rc=0 - to nearest */
    k0 = (int) t0;
#endif /*  __iset__ <= 2 */

    if (darg0.ll <= 0x3ff921fb00000000LL) { /* pi / 2 */
        ldx = (LD) X;
        ldxv2 = ldx * ldx;
        ldx = ldx + KA5 * ldx * ldxv2 * (KA3 / KA5 + ldxv2);
#if __iset__ >= 6
        lyv2.value = xv2;
        lyv4.value = xv4;
        lyv8.value = X * xv2 * xv4;
        lres2.llong = __builtin_e2k_fmad (LKA9, lyv2.llong, LKA7);
        lres2.value *= lyv8.value;
        lres.llong = __builtin_e2k_fmad (LKA13, lyv2.llong, LKA11);
        lres1.llong = __builtin_e2k_fmad (LKA17, lyv2.llong, LKA15);
        lres1.llong = __builtin_e2k_fmad (lres1.llong, lyv4.llong, lres.llong);
        lyv8.value *= xv4;
        lres.llong = __builtin_e2k_fmad (lres1.llong, lyv8.llong, lres2.llong);
        return (DB) (ldx + lres.value);
#else /* __iset__ < 6 */
        xv6 = xv2 * xv4;
        xv8 = xv4 * xv4;
        return (DB) (ldx + KA13 * X * xv6 * (KA7 / KA13 + KA9 / KA13 * xv2 +
            xv4 * (KA11 / KA13 + xv2) + xv8 * (KA15 / KA13 + KA17 / KA13 * xv2)));
#endif /* __iset__ < 6 */
    }
    if (unlikely(darg0.ll > 0x40e921fb00000000LL)) {      /* 2^14*pi */
        X = __rem_pio2 (X * 0.5, &y0, &k0);
        ldx = (LD) X + (LD) y0;
        ldx += ldx;
        X += X;
    }
    else {
//FIXME: ÐÏÞÅÍÕ-ÔÏ Ó ×ËÌÀÞÅÎÉÅÍ ÐÒÉ×ÅÄÅÎÉÑ ÞÅÒÅÚ fma ÎÁÞÉÎÁÀÔ ÄÏÌØÛÅ ÒÁÂÏÔÁÔØ ×ÓÅ ×ÅÔËÉ.
// ðÒÏ×ÅÒÉÔØ ÐÏÔÏÍ ÎÁ 24 ×ÅÔËÅ.
//#if __iset__ >= 6
#if 0
        lres.value = t0;
        lres1.value = X;
        lres.llong = __builtin_e2k_fmad (LPI_1, lres.llong, lres1.llong);
        X = (DB) (ldx = (LD) lres.value - t0 * PI_1T);
#else /* __iset__ < 6 */
        X = (DB) (ldx = t0 * (LD) PI_1 + (LD) X - t0 * PI_1T);
#endif /* __iset__ < 6 */
    }
    ldxv2 = ldx * ldx;
    ldx = ldx + KA5 * ldx * ldxv2 * (KA3 / KA5 + ldxv2);
    xv2 = X * X;
    xv4 = xv2 * xv2;
    if (k0 & 1) ldx = -ldx;
    if (k0 & 1) X = -X;
#if __iset__ >= 6
    lyv2.value = xv2;
    lyv4.value = xv4;
    lyv8.value = X * xv2 * xv4;
    lres2.llong = __builtin_e2k_fmad (LKA9, lyv2.llong, LKA7);
    lres2.value *= lyv8.value;
    lres.llong = __builtin_e2k_fmad (LKA13, lyv2.llong, LKA11);
    lres1.llong = __builtin_e2k_fmad (LKA17, lyv2.llong, LKA15);
    lres1.llong = __builtin_e2k_fmad (lres1.llong, lyv4.llong, lres.llong);
    lyv8.value *= xv4;
    lres.llong = __builtin_e2k_fmad (lres1.llong, lyv8.llong, lres2.llong);
    return (DB) (ldx + lres.value);
#else /* __iset__ < 6 */
    xv6 = xv2 * xv4;
    xv8 = xv4 * xv4;
    return (DB) (ldx + KA13 * X * xv6 * (KA7 / KA13 + KA9 / KA13 * xv2 +
        xv4 * (KA11 / KA13 + xv2) + xv8 * (KA15 / KA13 + KA17 / KA13 * xv2)));
#endif /* __iset__ < 6 */
}

weak_alias (__sin, sin)

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%  ðòïçòáííá  ðòé÷åäåîéñ  áòçõíåîôá ôòéçïîïíåôòéþåóëéè   %
%  æõîëãéê  ë  äéáðáúïîõ  ïô  -PI/4  äï  PI/4 .          %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/* **************************************************************************
 *
 * Common argument reduction routine for circular trigonometric functions.
 *
 * Inputs:
 *  x = argument value to be reduced to [-Pi/4..Pi/4] interval;
 * Outputs:
 *  nseg = 0,1,2,3 = (k % 4).
 *  hi+lo "fraction" part of reduced value; the hi+lo pair returned is
 *        closer to "normalized", so |lo| < C*0.5*|hi|, where C - small number.
 * Returns:
 *  hi
 *
 * Description:
 *  Input value is represented as x = k*(Pi/2) + r,
 *  where:
 *    k is integer (returned value is k%4);
 *    |r| <= Pi/4, and r = d[0]+d[1],
 *    and sign of x processed as well as its magnitude.
 *
 *  Two sub-domains:
 *  1. |x| < 2^30: use Cody & Waite algorithm, in extended precision;
 *  2. |x| > 2^30: use Payne & Hanek algorithm, in extended precision.
 *
 *  Usage. Given x, k = __libm_reduce_pi02_d( x, d );
 *    k  cos(x)  sin(x)  tan(x)  cot(x)
 *    - ------- ------- ------- -------
 *    0  cos(r)  sin(r)  tan(r)  cot(r)
 *    1 -sin(r)  cos(r) -cot(r) -tan(r)
 *    2 -cos(r) -sin(r)  tan(r)  cot(r)
 *    3  sin(r) -cos(r) -cot(r) -tan(r)
 *    - ------- ------- ------- -------
 *  i.e.
 *    cos(x) = ((k&1) ? sin(r) : cos(r)) * ones[(k+1)>>1]
 *    sin(x) = ((k&1) ? cos(r) : sin(r)) * ones[(k+0)>>1]
 *    tan(x) = ((k&1) ? -cot(r) : tan(r))
 *    cot(x) = ((k&1) ? -tan(r) : cot(r))
 *  where ones[0]=+1 and ones[1]=-1.
 */

#define BIAS_64F 1023
#define LIBM_64U(f)   (*(unsigned long long *)&f)

typedef struct {
    unsigned int lo_significand:32;
    unsigned int hi_significand:20;
    unsigned int exponent:11;
    unsigned int sign:1;
} bits_64f;

#define zero_none (&(TAB[0]))
#define ones      (&(TAB[2]))
#define TWO_26H      TAB[4]
#define TWO_27H      TAB[5]
#define PI04_25x2 (&(TAB[6]))
#define PI04_INV     TAB[8]
#define TWO_12H      TAB[9]
#define PI04_29x4 (&(TAB[10]))
#define PI04_21x5 (&(TAB[14]))
#define DP        (&(TAB[20]))

#define SHIFT_MASK (((const long long *)tbl_rem_pio2)[19])

/* Double precision pointer to the beginning of the table */
#define TAB ((const DB *)tbl_rem_pio2)

/* Table: all DP constants are presented in hexadecimal form */
static const unsigned int tbl_rem_pio2[140] = {
    0x00000000, 0x00000000,     /* zero_none[0] = 0.0 */
    0x00000000, 0xbff00000,     /* zero_none[1] = -1.0 */
    0x00000000, 0x3ff00000,     /* ones[0]=+1.0 */
    0x00000000, 0xbff00000,     /* ones[1]=-1.0 */
    0x00000000, 0x41980000,     /* TWO_26H=2^26+2^25 */
    0x00000000, 0x41A00000,     /* TWO_27H=2^27 */
    0x40000000, 0x3fe921fb,     /* PI04_25x2[0]=7.853981256484985351e-01 */
    0x18469899, 0x3e64442d,     /* PI04_25x2[1]=3.774894977445941084e-08 */
    0x6dc9c883, 0x3ff45f30,     /* PI04_INV=1.2732395447351627648657777  */
    0x00000000, 0x40b00000,     /* TWO_12H=2^12 */
    0x54000000, 0x3fe921fb,     /* PI04_29x4[0]= 7.85398162901401519e-01 */
    0x12000000, 0x3e010b46,     /* PI04_29x4[1]= 4.96046790449122809e-10 */
    0x3c000000, 0xbc267673,     /* PI04_29x4[2]=-6.08852590659478201e-19 */
    0x839a2520, 0x3a61701b,     /* PI04_29x4[3]= 1.76077993259160019e-27 */
    0x00000000, 0x3fe921fa,     /* PI04_21x5[0]= 7.85397529602050781e-01 */
    0x00000000, 0x3ea54443,     /* PI04_21x5[1]= 6.33795480098342522e-07 */
    0x00000000, 0xbd373dca,     /* PI04_21x5[2]=-8.25699101106303068e-14 */
    0x00000000, 0xbbf3b39a,     /* PI04_21x5[3]=-6.67515108790745199e-20 */
    0xe0e68948, 0x3a845c06,     /* PI04_21x5[4]= 8.22312846816212890e-27 */
    0xffffffff, 0xffffffff,     /* binary mask for shift */

    0x00000000, 0x00000000,     /* DP: 0.0, */
    0x60000000, 0x4c745f30,     /* 2.046016981349385648836250700164e+060 */
    0x00000000, 0x4aeb9391,     /* 8.254054289264490525064660032005e+052 */
    0x20000000, 0x494529fc,     /* 9.439460931682103953897247338562e+044 */
    0xc0000000, 0x47ad5f47,     /* 1.952116767110621547313686005734e+037 */
    0x00000000, 0x4624d377,     /* 8.250042247650623511217989222400e+029 */
    0x80000000, 0x447b6c52,     /* 8.093880509309323837440000000000e+021 */
    0x20000000, 0x430993c4,     /* 8.999181221888000000000000000000e+014 */
    0xe0000000, 0x4189041f,     /* 5.246258800000000000000000000000e+007 */
    0xe0000000, 0x3fe458ea,     /* 6.358541846275329589843750000000e-001 */
    0x50000000, 0x3e67aef1,     /* 4.411371357093685219297185540199e-008 */
    0x30000000, 0x3cd0db92,     /* 9.357903004839670645792493175463e-016 */
    0x80000000, 0x3b3c7484,     /* 2.353754575166693410660302330611e-023 */
    0x00000000, 0x39bba5c0,     /* 1.363125066388675855896694788313e-030 */
    0xa0000000, 0x382924bb,     /* 3.694503502754186795088005929383e-038 */
    0x90000000, 0x36904e8c,     /* 7.140855962175793596900260069782e-046 */
    0x00000000, 0x34ccfe1d,     /* 2.364821726373623754643571498017e-054 */
    0x20000000, 0x337d6396,     /* 1.143051593553354967497544501783e-060 */
    0x60000000, 0x31d4d39f,     /* 1.207036571501449232116064178755e-068 */
    0xf0000000, 0x3054411a,     /* 6.996787921191781539645339571203e-076 */
    0x40000000, 0x2ec52ebb,     /* 2.180767375544163514317983270887e-083 */
    0x60000000, 0x2d2213a6,     /* 2.773140368076338569696355027677e-091 */
    0xd0000000, 0x2ba1c09a,     /* 1.623258097495235854623956926912e-098 */
    0x00000000, 0x29e7df90,     /* 8.132121151468691760671964895375e-107 */
    0x60000000, 0x2883991d,     /* 1.591643177090576102737487139063e-113 */
    0x80000000, 0x26dcc1a9,     /* 1.740033762488903975924945991050e-121 */
    0x40000000, 0x255cfa4e,     /* 1.045121117805295960150498150334e-128 */
    0x80000000, 0x23a17e2e,     /* 4.700604601390667888085976513777e-137 */
    0x30000000, 0x224df928,     /* 1.920287235149126279059737107922e-143 */
    0x20000000, 0x20b63ff1,     /* 4.248227543453209400402242260441e-151 */
    0x10000000, 0x1f2fff78,     /* 1.820765937786180112045487432933e-158 */
    0xe0000000, 0x1d8980fe,     /* 2.162513450891567031488279435248e-166 */
    0xb0000000, 0x1c02f118,     /* 9.573116991753421791054177074741e-174 */
    0x40000000, 0x1a66829b,     /* 1.695241668375062324563676303218e-181 */
    0x40000000, 0x18cf6d36,     /* 3.526714137599195261169041477828e-189 */
    0xe0000000, 0x175f6793,     /* 4.201226574477920251862993278447e-196 */
    0x80000000, 0x15b6136e,     /* 4.400696946477386151692030402938e-204 */
    0xc0000000, 0x143e8c7e,     /* 3.629755203012117638968395506016e-211 */
    0xa0000000, 0x12aa797f,     /* 9.374813311852155776212661660531e-219 */
    0x30000000, 0x11116ba9,     /* 1.838421711736915568716495184619e-226 */
    0xb0000000, 0x0f8bac7e,     /* 8.703667723606696709863058939347e-234 */
    0x60000000, 0x0dfcbe2f,     /* 2.694103195416866075798826976351e-241 */
    0xe0000000, 0x0c5e839c,     /* 4.261910659625422488907525913878e-249 */
    0x40000000, 0x0adbc529,     /* 2.311863005796137109331170265312e-256 */
    0xf0000000, 0x0942ea6b,     /* 4.693063677320775368899397790663e-264 */
    0x30000000, 0x07b6bf62,     /* 1.681974275807482061903460283707e-271 */
    0x20000000, 0x062e3574,     /* 6.656831249574116326758421181758e-279 */
    0x00000000, 0x046580cc,     /* 1.765206356024202931243687098117e-287 */
    0xc0000000, 0x02e1bf1e,     /* 8.683374018275684447976280051869e-295 */
    0x20000000, 0x016aeafc,     /* 7.850470542671836431397175402126e-302 */
};

 /*
 /   x - ÉÓÈÏÄÎÙÊ ÁÒÇÕÍÅÎÔ
 /   lo - ÕËÁÚÁÔÅÌØ ÎÁ ÍÌÁÄÛÕÀ ÞÁÓÔØ ×ÏÚ×ÒÁÝÁÅÍÏÇÏ ÒÅÚÕÌØÔÁÔÁ;
 /
 /   nseg - ÕËÁÚÁÔÅÌØ ÎÁ ÃÅÌÏÅ, ÓÏÄÅÒÖÁÝÅÅ 2 ÍÌÁÄÛÉÈ ÒÁÚÒÑÄÁ
 /         ÃÅÌÏÇÏ ÞÉÓÌÁ, ÂÌÉÖÁÊÛÅÇÏ Ë ÚÎÁÞÅÎÉÀ [x*pi/2].
 /
 /   æÕÎËÃÉÑ ×ÏÚ×ÒÁÝÁÅÔ ÓÔÁÒÛÕÀ ÞÁÓÔØ ÐÒÉ×ÅÄÅÎÎÏÇÏ ÁÒÇÕÍÅÎÔÁ.
*/

double __rem_pio2 (double xx,
                   double * lo,
                   int * nseg)
{
    double /*volatile*/ tv, y_hi;
    double x = xx, x_hi, x_lo, t, z, s, tt, y, y_lo;
    double z0, z1, z2, z3, z4;
    double t1, t2, t3;
    bits_64f *ptx = (bits_64f *) & x;
    bits_64f *pty = (bits_64f *) & y;
    int i, j, k, bitpos, exp, sign;

    sign = ptx->sign;
    ptx->sign = 0;

    if (ptx->exponent < BIAS_64F + 30) {    /* 0<=|x|<2^30: Cody & Waite */
        /* k = (int) trunc(x*(4/Pi)) */
        y = x * PI04_INV;
        k = (((0x100000 | pty->hi_significand) << 11) |
             (pty->lo_significand >> (32 - 11))) >> (BIAS_64F + 31 - pty->exponent);
        j = k + (k & 1);
        k = (((k + 1) >> 1) * (1 - (sign << 1))) & 3;

        /* y_hi+y_lo = x - k*(Pi/4) */
        s = (double) j;

        if (ptx->exponent < BIAS_64F + 23) {        /* 0 <= |x| < 2^23 */
            z0 = s * PI04_29x4[0];
            y_hi = (x - z0);
            z1 = s * PI04_29x4[1];
            t1 = y_hi;
            y_hi -= z1;
            t1 -= y_hi;
            t1 -= z1;
            y_lo = t1;
            z2 = s * PI04_29x4[2];
            t2 = y_hi;
            y_hi -= z2;
            t2 -= y_hi;
            t2 -= z2;
            y_lo += t2;
            z3 = s * PI04_29x4[3];
            y_lo -= z3;
        }
        else {                  /* 2^23 <= |x| < 2^30 */
            z0 = s * PI04_21x5[0];
            y_hi = (x - z0);
            z1 = s * PI04_21x5[1];
            t1 = y_hi;
            y_hi -= z1;
            t1 -= y_hi;
            t1 -= z1;
            y_lo = t1;
            z2 = s * PI04_21x5[2];
            t2 = y_hi;
            y_hi -= z2;
            t2 -= y_hi;
            t2 -= z2;
            y_lo += t2;
            t = y_hi;
            y_hi += y_lo;
            t -= y_hi;
            y_lo += t;
            z3 = s * PI04_21x5[3];
            t3 = y_hi;
            y_hi -= z3;
            t3 -= y_hi;
            t3 -= z3;
            y_lo += t3;
            z4 = s * PI04_21x5[4];
            y_lo -= z4;
        }

        /* set sign */
        s = ones[sign];
        y_hi = s * y_hi;
        y_lo = s * y_lo;

        /* resplit result */
        t = y_hi + y_lo;
        tt = y_hi - t;
        *lo = tt + y_lo;
        *nseg = k;
        return t;
    }
    else {                      /* |x| >= 2^30: use Payne & Hanek algorithm */

        exp = ptx->exponent;
        if (exp == 2047) {
            if (xx == xx) /* x is inf */
                __set_errno (EDOM);
            *nseg = 8;
            *lo = xx-xx;
            return xx-xx; /* NaN */
        }
        /* scale by 2^(-200), to extend the number */
        /* of chunks available in 4/Pi table */
        ptx->exponent -= 200;

        /* split x into hi+lo pair (use truncation instead of rounding!) */
        x_hi = x;
        LO (x_hi) &= 0xf8000000;
        x_lo = (x - x_hi);

        /* find integer part of x*(4/Pi); 0x000147ae = trunc((2^21)/25) */
        bitpos = exp - (29 + BIAS_64F);
        j = (bitpos * 0x000147ae) >> 21;
        bitpos = bitpos - j * 25;

        /* y_hi+y_lo: y = (x_lo*DP[j+0] + x_hi*DP[j+1]); */
        y_hi = x_lo * DP[j + 0];
        z = x_hi * DP[j + 1];
        t = y_hi;
        y_hi += z;
        t -= y_hi;
        y_lo = (t + z);
        y = (y_hi + y_lo);
        LIBM_64U (y) &= ((SHIFT_MASK) << (38 - bitpos));
        y_hi -= y;

        /* y_hi+y_lo: y += (x_lo*DP[j+1] + x_hi*DP[j+2]); */
        z = x_lo * DP[j + 1];
        t = y_hi;
        y_hi += z;
        t -= y_hi;
        t += z;
        y_lo += t;
        z = x_hi * DP[j + 2];
        t = y_hi;
        y_hi += z;
        t -= y_hi;
        t += z;
        y_lo += t;
        y = (y_hi + y_lo);

        /* y_hi+y_lo: y += (z1=x_lo*DP[j+2] + z2=x_hi*DP[j+3]); */
        z1 = x_lo * DP[j + 2];
        y += z1;
        z2 = x_hi * DP[j + 3];
        y += z2;

        /* k = (int) trunc(y) */
        i = (BIAS_64F + 52 - pty->exponent);
        LIBM_64U (y) >>= i;
        k = LO (y);
        LIBM_64U (y) <<= i;

        /* y_hi+y_lo: y -= k; resplit y_hi+y_lo */
        y_hi -= y;
        t = y_hi;
        y_hi += y_lo;
        t -= y_hi;
        y_lo += t;

        /* j += 3; y_hi+y_lo: y += sum(x_lo*DP[j+0] + x_hi*DP[j+1],j=0..5) */
        y_hi += zero_none[k & 1];
        k = (((k + 1) >> 1) * (1 - (sign << 1))) & 3;

        /* y_hi+y_lo: y += (z1 + z2); */
        t = y_hi;
        y_hi += z1;
        t -= y_hi;
        t += z1;
        y_lo += t;
        t = y_hi;
        y_hi += z2;
        t -= y_hi;
        t += z2;
        y_lo += t;

        /* y_hi+y_lo: y += (x_lo*DP[j+3] + x_hi*DP[j+4]); */
        z = x_lo * DP[j + 3];
        t = y_hi;
        y_hi += z;
        t -= y_hi;
        t += z;
        y_lo += t;
        z = x_hi * DP[j + 4];
        t = y_hi;
        y_hi += z;
        t -= y_hi;
        t += z;
        y_lo += t;

        /* y_hi+y_lo: y += (x_lo*DP[j+4] + x_hi*DP[j+5]); */
        z = x_lo * DP[j + 4];
        t = y_hi;
        y_hi += z;
        t -= y_hi;
        t += z;
        y_lo += t;
        z = x_hi * DP[j + 5];
        t = y_hi;
        y_hi += z;
        t -= y_hi;
        t += z;
        y_lo += t;

        /* y_hi+y_lo: y += (x_lo*DP[j+5] + x_hi*DP[j+6]); */
        z = (x_lo * DP[j + 5] + x_hi * DP[j + 6]);
        t = y_hi;
        y_hi += z;
        t -= y_hi;
        t += z;
        y_lo += t;

        /* y_hi+y_lo: y += (x_lo*DP[j+6] + x_hi*DP[j+7]); */
        z = (x_lo * DP[j + 6] + x_hi * DP[j + 7]);
        t = y_hi;
        y_hi += z;
        t -= y_hi;
        t += z;
        y_lo += t;

        /* y_hi+y_lo: y += (x_lo*DP[j+7] + x_hi*DP[j+8]); resplit y_hi+y_lo */
        z = (x_lo * DP[j + 7] + x_hi * DP[j + 8]);
        t = y_hi;
        y_hi += z;
        s = y_hi * TWO_26H;
        tv = (y_hi + s);
        y_hi = (tv - s);
        t -= y_hi;
        t += z;
        y_lo += t;

        /* y_hi+y_lo *= Pi/4, extended; resplit y_hi+y_lo */
        y_lo = PI04_25x2[0] * y_lo + PI04_25x2[1] * (y_hi + y_lo);
        y_hi = PI04_25x2[0] * y_hi;
        t = y_hi;
        y_hi += y_lo;
        t -= y_hi;
        y_lo += t;

        /* return result */
        s = ones[sign];
        *nseg = k;
        *lo = s * y_lo;
        return s * y_hi;
    }
}
