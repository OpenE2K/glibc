/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ðòïçòáííá  ÷ùþéóìñåô  çìá÷îïå  úîáþåîéå  æõîëãéé   %
%  'áòëôáîçåîó'.  òåúõìøôáô  ÷  òáäéáîáè.                %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define KB7 -1.4285650707649766162808e-01
#define KB5  1.9999999999909050939855e-01
#define KB3 -3.3333333333333333314059e-01L
#define KA17  5.5641662446175787495265e-02
#define KA15 -6.6586761646430189911225e-02
#define KA13  7.6922026111629768700334e-02
#define KA11 -9.0909083235993354765825e-02
#define KA9  1.1111111108064892858049e-01L
#define KA7 -1.4285714285708342569591e-01L
#define KA5  1.9999999999999995580503e-01L
#define KA3 -3.3333333333333333332794e-01L
#define KP4 -6.4034900123119266769403e-02L
#define KP2 -3.3748002317137816948685e-01L
#define KP0 -3.3333333333333333269894e-01L
#define KQ6  8.0885975543677403390740e-02L
#define KQ4  7.3099731351302139685295e-01L
#define KQ2  1.6124400695141256955943e+00L

LD __atanl (LD X)
{
    _type_long_double_bits arg, f;
    _type_double_bits con, darg;
    LD y, yv2, yv4, yv8, c, s;
    LI ind;

    arg.value = ldfabs (X);
    y = 1 / arg.value;
    if ((arg.twolong.short0 & 0x7fff) > 0x4017) {  /* |X| > 2^24 ÉÌÉ NaN */
        y -= PION2_lo;
        return (X > 0) ? PION2 - y : y - PION2;
    }
    yv2 = X * X;
    yv4 = yv2 * yv2;
    if (arg.value < 0.125) {  /* |X| < 0.125 */
        yv8 = yv4 * yv4;
        return (KA5 * yv2 * (KA3 / KA5 + yv2) + KA7 * yv2 * yv4 + KA11 *
              (KA9 / KA11 + yv2) * yv8 + (KA13 + KA15 * yv2 + KA17 * yv4) *
              (yv4 * yv8)) * X + X;
    }
    c = arg.value;

    if (arg.value > 8) {  /* |X| > 8 */
        arg.twolong.long1 &= 0xffffffff00000000ULL;
        c -= arg.value;
        f.value = y;  f.twolong.long1 &= 0xffffffff00000000ULL;
        s = PION2_lo - (1 - arg.value * f.value - c * f.value) * y;
        y *= (KP4 + KP2 * yv2 + KP0 * yv4) / (KQ6 + KQ4 * yv2 + yv4 * (KQ2 + yv2));
        arg.value = PION2 - f.value;
        y -= s;
    }
    else {
        darg.value = (DB) arg.value;
        darg.llong += 0x100000000000LL;
        ind = (darg.llong - 0x3fc0000000000000LL) >> 45;
        darg.llong &= 0x7fffe00000000000LL;
        y = (darg.value - c) / (1 + c * darg.value);
        yv2 = y * y;
        y = y - constarctg_lo[ind] + KB5 * y * yv2 * (KB3 / KB5 + yv2) +
            KB7 * y * yv2 * (yv2 * yv2);
        con.llong = ((LL) constarctg[ind]) << 32;
        arg.value = (LD) con.value;
    }
    return (X > 0) ? arg.value - y : y - arg.value;
}

weak_alias (__atanl, atanl)
