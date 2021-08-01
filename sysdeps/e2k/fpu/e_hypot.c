/* Copyright (c) 2016-2018 ZAO "MCST". All rights reserved. */

#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     נעןחעבםםב  קש‏יףלסופ  כןעומר כקבהעבפמשך ית ףץםםש   %
%  כקבהעבפןק ט י Y.                                      %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

DB __hypot (DB X, DB Y)
{
    DB fx, fy, max;
    LD ldr;

    fx = dfabs (X);
    fy = dfabs (Y);
    if (fx == Inf || fy == Inf)
        return Inf;               /* |X| ֹֹּ |Y| == Inf */
    if (X != X || Y != Y)
        return X + Y;               /* |X| ֹֹּ |Y| == NaN */
    max = (fy < fx) ? fx : fy;
    fy = dfabs (fx - fy);
    if (fy == max)
        return max;
    ldr = X * (LD) X + Y * (LD) Y;
    if (ldr >= 3.2317006071311003713e+616L) /* sqr(DBL_MAX+0.5ulp) */
        return SVID_libm_err (X, Y, 4);
    return (DB) __inline_sqrtl (ldr);
}

strong_alias (__hypot, __hypot_finite)
weak_alias (__hypot, hypot)
weak_alias (__hypot, __ieee754_hypot)
