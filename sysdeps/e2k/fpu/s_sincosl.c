/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     πςοηςαννα  χωώισμρετ  ϊξαώεξιε  ζυξλγικ  'σιξυσ' ι %
%  'λοσιξυσ'. αςηυνεξτ  χ  ςαδιαξαθ.                     %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#undef KC4
#undef KC2
#undef KS5
#undef KS3
#define RS      1.38350580552821637120e+19  /* 3*2^62 = 0x43e8000000000000 */
#define PIO2_1  1.57079632679233327508e+00  /* ΠΕÒΧΩΕ 36 ΒΙΤ pi/2 = 0x3ff921fb54440000 */
#define PIO2_2  2.56334415158395578912e-12  /* ΧΤΟÒΩΕ 36 ΒΙΤ pi/2 = 0x3d868c234c4c0000 */
#define PIO2_2T 1.05629990669874271122e-23L /* ΟΣΤΑΧΫΙΕΣΡ 64 ΒΙΤ pi/2 = 3fb2 0xcc51701b839a2520 */

//(sinl(sqrtl(y))-sqrtl(y))/y/sqrtl(y); b = DPION4 * DPION4; a = 0.0L; o=6; MAX=0.85506 or 0.22589
#define KS7 -7.5866971177069181028716e-13
#define KS6  1.6058531618986146839027e-10
#define KS5 -2.5052106232447577328029e-08
#define KS4  2.7557319219339166517547e-06
#define KS3 -1.9841269841265064946528e-04L
#define KS2  8.3333333333333314920845e-03L
#define KS1 -1.6666666666666666665508e-01L

//(cosl(sqrtl(y)) - 1.0L)/y; b = DPION4 * DPION4; a = 0.0L; o=7; MAX=0.0036623 or 8.093
#define KC8  4.7410866985075003361486e-14
#define KC7 -1.1470361263661413773148e-11
#define KC6  2.0876754983065436482112e-09
#define KC5 -2.7557319218191050044576e-07
#define KC4  2.4801587301578204650565e-05L
#define KC3 -1.3888888888888881875461e-03L
#define KC2  4.1666666666666666646067e-02L

void __sincosl (LD X,
                LD *SI,
                LD *CO)
{
    _type_long_double_bits arg;
    LD xl, xv2, xv4, fn0, a0, b0, w0, si, co;
    DB y0, y1;
    int k;

    arg.value = ldfabs (X);

    if (arg.value <= (PION2 / 2)) {     /* |x| <= pi/4 */

        if (arg.value == 0) { /* |x| = 0 */
	    *SI = X;
            *CO = 1;
            return;
        }
        xv2 = X * X;
        xv4 = xv2 * xv2;
        *SI = KS7 * X * xv4 * xv4 * (KS4 / KS7 + KS5 / KS7 * xv2 + xv4 * (KS6 / KS7 + xv2)) +
            ((xv2 * KS3 + KS2) * xv2 + KS1) * (xv2 * X) + X;
        *CO = KC8 * xv2 * xv4 * xv4 * (KC5 / KC8 + KC6 / KC8 * xv2 + xv4 * (KC7 / KC8 + xv2)) +
            ((KC4 * xv2 + KC3) * xv2 + KC2) * xv4 - 0.5 * xv2 + 1;
        return;
    }
    if (arg.value > 8.43314856532626152039e8) { /* |x| > 2^28 * pi = 0x41c921fb54442d18 */
        y0 = __rem_pio2l (X, &y1, &k);
        X = y0 + (LD) y1;
        xl = y0 - X + y1;
    }
    else {     /* |x| > pi/4, ΞΟ |x| <= 2^28 * pi */
//        arg.value = X * K2ONPI + RS;
//        k = arg.twolong.long1;
//        fn0 = arg.value - RS;
        arg.value = X * K2ONPI + ((X < 0) ? -0.5 : 0.5);
        k = (int) arg.value;
        fn0 = (LD) k;
        a0 = X - fn0 * PIO2_1;
        w0 = fn0 * PIO2_2;
        b0 = a0 - w0;
        xl = (a0 - b0) - w0 - fn0 * PIO2_2T;
        X = b0 + xl;
        xl = b0 - X + xl;
    }
    xv2 = X * X;
    xv4 = xv2 * xv2;
    si = KC8 * xv2 * xv4 * xv4 * (KC5 / KC8 + KC6 / KC8 * xv2 + xv4 * (KC7 / KC8 + xv2)) +
        X * xl + ((KC4 * xv2 + KC3) * xv2 + KC2) * xv4 - 0.5 * xv2 + 1;
    w0 = KS7 * X * xv4 * xv4 * (KS4 / KS7 + KS5 / KS7 * xv2 + xv4 * (KS6 / KS7 + xv2)) +
        xl + ((xv2 * KS3 + KS2) * xv2 + KS1) * (xv2 * X) + X;
    if (k & 1) {
	co = w0;
    }
    else {
	co = si;
	si = w0;
    }
    if (k & 2) si = -si;
    if ((k + 1) & 2) co = -co;
    *SI = si;
    *CO = co;
}

weak_alias (__sincosl, sincosl)
