/* Copyright (c) 2016-2018 ZAO "MCST". All rights reserved. */

#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ðòïçòáííá  ÷ùþéóìñåô  úîáþåîéå  æõîëãéé  'ôáîçåîó'.%
%  áòçõíåîô  ÷  òáäéáîáè.                                %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#if 0
static DB CONSTD[] = {
    -6.2593235559168460531776e+02,      /* KP6 */
    5.1518336523158058070291e+04,       /* KP4 */
    3.5854378114407966500304e+01,       /* KQ6 */
    -6.8751596694549312505389e+03,      /* KQ4 */
    6.75539944105574400e+15,            /* RS = 3*2^51 = 0x4338000000000000 */
};

static LD CONSTLD[] = {
    -9.3690855364069496489147e+05L,     /* KP2 */
    2.0074475576989649632459e+06L,      /* KP0 */
    -2.6775936774103997314008e+05L,     /* KQ2 */
    2.0074475576989649632264e+06L,      /* KQ0 */
    -3.1415926535897931159980 / 2,      /* PIO2_1  */
    1.2246467991473531772015e-16L / 2,  /* PIO2_1T */
};
    DB KP6 = CONSTD[0];           /* -6.2593235559168460531776e+02    */
    DB KP4 = CONSTD[1];           /* 5.1518336523158058070291e+04     */
    DB KQ6 = CONSTD[2];           /* -3.5854378114407966500304e+01    */
    DB KQ4 = CONSTD[3];           /* 6.8751596694549312505389e+03     */
    DB RS = CONSTD[4];            /* 6.75539944105574400e+15 = 3*2^51 */
    LD KP2 = CONSTLD[0];     /* -9.3690855364069496489147e+05    */
    LD KP0 = CONSTLD[1];     /* 2.0074475576989649632459e+06     */
    LD KQ2 = CONSTLD[2];     /* -2.6775936774103997314008e+05    */
    LD KQ0 = CONSTLD[3];     /* 2.0074475576989649632264e+06     */
    LD PIO2_1 = CONSTLD[4];  /* -3.1415926535897931159980/2 53bits */
    LD PIO2_1T = CONSTLD[5]; /* 1.2246467991473531772015e-16L/2  */
#endif

DB __tan (DB X)

#define KP6 -6.2593235559168460531776e+02
#define KP4  5.1518336523158058070291e+04
#define KQ6  3.5854378114407966500304e+01
#define KQ4 -6.8751596694549312505389e+03
#define LKP6 0xc0838f7576d9343aLL
#define LKP4 0x40e927cac4cc36c7LL
#define LKQ6 0x4041ed5c4315e672LL
#define LKQ4 0xc0badb28e018ef1aLL

#define TWOONPI 0.636619772367581343075535 /* 0x3fe45f306dc9c883 */
#define RS      6.75539944105574400e+15    /* 3*2^51 = 0x4338000000000000 */

#define KP2 -9.3690855364069496489147e+05L
#define KP0  2.0074475576989649632459e+06L
#define KQ2 -2.6775936774103997314008e+05L
#define KQ0  2.0074475576989649632264e+06L
#define PIO2_1  (-3.1415926535897931159980 * 0.5)  /* 53bits */
#define LPIO2_1 0xbff921fb54442d18LL
#define PIO2_1T (1.2246467991473531772015e-16L * 0.5)  /* PIO2_1T */
{
    type_union_64f darg0, darg1;
    DB t0, y0, chis2, xv2, xv4;
    LD ldx, ldxv2, ldchis1, ldznam;
    int k0;
#if __iset__ >= 6
    _type_double_bits lyv2, lyv4, lyv8, lres, lres1;
#endif /* __iset__ < 6 */

    darg0.db = X;
    darg0.ll &= ~0x8000000000000000LL;

    if (darg0.ll <= 0x3fe921fb00000000LL) { /* pi / 4 */
        ldx = (LD) X;
        ldxv2 = ldx * ldx;
        xv2 = X * X;
        xv4 = xv2 * xv2;
        ldchis1 = KQ2 * ldx * (KQ0 / KQ2 + ldxv2);
        ldznam = KP0 + KP2 * ldxv2;
#if __iset__ >= 6
        lyv2.value = xv2;
        lres.llong = __builtin_e2k_fmad (LKQ6, lyv2.llong, LKQ4);
        chis2 = X * xv4 * lres.value;
        lyv4.value = xv4;
        lyv8.value = xv4 * xv4;
        lres.llong = __builtin_e2k_fmad (LKP6, lyv2.llong, LKP4);
        lres.llong = __builtin_e2k_fmad (lres.llong, lyv4.llong, lyv8.llong);
        ldznam += lres.value;
#else /* __iset__ < 6 */
        chis2 = KQ6 * X * xv4 * (KQ4 / KQ6 + xv2);
        ldznam += xv4 * (KP4 + xv2 * (KP6 + xv2));
#endif /* __iset__ < 6 */
        return (DB) ((ldchis1 - chis2) / ldznam);
    }
    darg1.db = X * TWOONPI;
#if __iset__ <= 2
    darg1.db += RS;
    t0 = darg1.db - RS;
    k0 = darg1.two_int.i1;
#else /*  __iset__ <= 2 */
#pragma asm_inline
    __asm ("fdtoifd 0x0,%1,%0" : "=r" (t0): "r" (darg1.db)); /* rc=0 - to nearest */
    k0 = (int) t0;
#endif /*  __iset__ <= 2 */
    if (unlikely(darg0.ll > 0x40d921fb00000000LL)) {      /* 2^13*pi */
        X = __rem_pio2 (X, &y0, &k0);
        ldx = (LD) X + (LD) y0;
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
    ldchis1 = KQ2 * ldx * (KQ0 / KQ2 + ldxv2);
    ldznam = KP0 + KP4 * ldxv2 * (KP2 / KP4 + ldxv2);
#if __iset__ >= 6
    lyv2.value = xv2;
    lres.llong = __builtin_e2k_fmad (LKQ6, lyv2.llong, LKQ4);
    chis2 = X * xv4 * lres.value;
    lyv2.value = xv2 * xv4;
    lyv4.value = xv4 * xv4;
    lres.llong = __builtin_e2k_fmad (LKP6, lyv2.llong, lyv4.llong);
    ldznam += lres.value;
#else /* __iset__ < 6 */
    chis2 = KQ6 * X * xv4 * (KQ4 / KQ6 + xv2);
    ldznam += xv2 * xv4 * (KP6 + xv2);
#endif /* __iset__ < 6 */

    if (k0 & 1) {
        return (DB) (ldznam / (chis2 - ldchis1));
    }
    else {
        return (DB) ((ldchis1 - chis2) / ldznam);
    }
}

weak_alias (__tan, tan)
