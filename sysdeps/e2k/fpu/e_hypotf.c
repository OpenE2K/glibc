/* Copyright (c) 2016 AO MCST. All rights reserved.
 * Distributed under the terms of MIT License.
 */

#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%     ПРОГРАММА  ВЫЧИСЛЯЕТ  АБСОЛЮТНОЕ  ЗНАЧЕНИЕ  КОМП-  %
%  ЛЕКСНОГО  ЧИСЛА.                                      %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

FL __hypotf (FL re, FL im)
{
    _type_float_bits fx, fy;
    DB x, y;

    x = (DB) re;  fx.value = ffabs (re);
    y = (DB) im;  fy.value = ffabs (im);
    if (fx.int0 == 0x7f800000 || fy.int0 == 0x7f800000)
        return Inf_F;               /* |re| или |im| == Inf */
    if (fx.int0 > 0x7f800000 || fy.int0 > 0x7f800000)
        return re + im;             /* |re| или |im| == NaN */

    x = x * x + y * y;
    if (x > 1.15792082335569939e77) /* 0x4fefffffe0000007 */
        return SVID_libm_err (re, im, 104);
    return (FL) dsqrt (x);
}

strong_alias (__hypotf, __hypotf_finite)
weak_alias (__hypotf, hypotf)
weak_alias (__hypotf, __ieee754_hypotf)
