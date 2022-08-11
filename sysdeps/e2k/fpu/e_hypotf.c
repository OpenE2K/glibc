/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ðòïçòáííá  ÷ùþéóìñåô  áâóïìàôîïå  úîáþåîéå  ëïíð-  %
%  ìåëóîïçï  þéóìá.                                      %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

FL __hypotf (FL re, FL im)
{
    _type_float_bits fx, fy;
    DB x, y;

    x = (DB) re;  fx.value = ffabs (re);
    y = (DB) im;  fy.value = ffabs (im);
    if (fx.int0 == 0x7f800000 || fy.int0 == 0x7f800000)
        return Inf_F;               /* |re| ÉÌÉ |im| == Inf */
    if (fx.int0 > 0x7f800000 || fy.int0 > 0x7f800000)
        return re + im;             /* |re| ÉÌÉ |im| == NaN */

    x = x * x + y * y;
    if (x > 1.15792082335569939e77) /* 0x4fefffffe0000007 */
        return SVID_libm_err (re, im, 104);
    return (FL) dsqrt (x);
}

strong_alias (__hypotf, __hypotf_finite)
weak_alias (__hypotf, hypotf)
weak_alias (__hypotf, __ieee754_hypotf)
