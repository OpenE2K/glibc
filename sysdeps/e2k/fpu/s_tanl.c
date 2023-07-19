/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     נעןחעבםםב  קש‏יףלסופ  תמב‏ומיו  זץמכדיי  'פבמחומף'.%
%  בעחץםומפ  ק  עבהיבמבט.                                %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define RS      1.38350580552821637120e+19  /* 3*2^62 = 0x43e8000000000000 */
#define PIO2_1  1.57079632679233327508e+00  /* ÐÅÒ×ÙÅ 36 ÂÉÔ pi/2 = 0x3ff921fb54440000 */
#define PIO2_2  2.56334415158395578912e-12  /* ×ÔÏÒÙÅ 36 ÂÉÔ pi/2 = 0x3d868c234c4c0000 */
#define PIO2_2T 1.05629990669874271122e-23L /* ÏÓÔÁ×ÛÉÅÓÑ 64 ÂÉÔ pi/2 = 3fb2 0xcc51701b839a2520 */

#define KP3  1.0000000000000000000000e+00L
#define KP2 -1.3761780813849336814889e+04L
#define KP1  1.2061424820064407129297e+06L
#define KP0 -1.8755273140009310405695e+07L
#define KQ3  1.4388138194254051238346e+04L
#define KQ2 -1.3814833314777879009485e+06L
#define KQ1  2.6124755214030494446390e+07L
#define KQ0 -5.6265819420027931216353e+07L

LD __tanl (LD X)
{
    _type_long_double_bits arg, arg1;
    LD xl, xv2, xv4, ax, fn0, a0, b0, w0;
    DB y0, y1;
    int k, sign;

    ax = ldfabs (X);

    if (ax <= (PION2 / 2)) {     /* |x| <= pi/4 */

        if (ax == 0) { /* |x| = 0 */
            return X;
        }
        if (ax > 0.625) {     /* |x| > 0.625 */
            sign = (X < 0);
            X = (PION2 * 0.5) - ax + (PION2_lo * 0.5); /* x = pi/4 - |x| */
        }
        xv2 = X * X;
        xv4 = xv2 * xv2;
        b0 = KQ0 + KQ1 * xv2 + xv4 * (KQ2 + KQ3 * xv2);
        a0 = (KP0 + KP1 * xv2 + xv4 * (KP2 + xv2)) * (X * xv2);
        a0 = a0 / b0;
        w0 = X + a0;
        if (ax > 0.625) {     /* |x| > 0.625 */
            w0 = (1 - 2 * (X - (w0 * w0 / (w0 + 1) - a0)));
            if (sign) {
                w0 = -w0;
            }
        }
        return w0;
    }
    if (ax > 8.43314856532626152039e8) { /* |x| > 2^28 * pi = 0x41c921fb54442d18 */
        y0 = __rem_pio2l (X, &y1, &k);
        X = y0 + (LD) y1;
        xl = y0 - X + y1;
    }
    else {     /* |x| > pi/4, ÎÏ |x| <= 2^28 * pi */
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
    ax = ldfabs (X);

    if (ax > 0.625) {     /* |x| > 0.625 */
        sign = (X < 0);
        X = (PION2 * 0.5) - ax; /* x = pi/4 - |x| */
        xl = (sign) ? (PION2_lo * 0.5) + xl : (PION2_lo * 0.5) - xl;
        X += xl;
        xl = 0;
    }
    xv2 = X * X;
    xv4 = xv2 * xv2;
    b0 = KQ0 + KQ1 * xv2 + xv4 * (KQ2 + KQ3 * xv2);
    a0 = (KP0 + KP1 * xv2 + xv4 * (KP2 + xv2)) * xv2;
    a0 = a0 / b0;
    a0 = xl + 3 * xl * a0 + X * a0;
    w0 = X + a0;
    if (ax > 0.625) {     /* |x| > 0.625 */
        b0 = (LD) ((k & 1) ? -1 : 1);
        w0 = (b0 - 2 * (X - (w0 * w0 / (w0 + b0) - a0)));
        if (sign) {
            w0 = -w0;
        }
        return w0;
    }
    if (!(k & 1)) {
        return w0;
    }
    else { /*  compute -1.0/(x+r) accurately */
      arg1.value = w0;
      arg1.twolong.long1 &= 0xffffffff00000000LL;
      b0 = a0 - (arg1.value - X);         /* arg1+b0 = a0+x */
      fn0 = -1 / w0;
      arg.value = fn0;
      arg.twolong.long1 &= 0xffffffff00000000LL;
      ax = 1 + arg.value * arg1.value;
      return arg.value + fn0 * (ax + arg.value * b0);
    }
}

weak_alias (__tanl, tanl)
