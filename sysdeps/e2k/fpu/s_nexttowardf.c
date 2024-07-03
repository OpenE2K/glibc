/* Copyright (c) 2016 AO MCST. All rights reserved.
 * Distributed under the terms of MIT License.
 */

#include <errno.h>
#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%    ПРОГРАММА  ВЫЧИСЛЯЕТ  ЗНАЧЕНИЕ,  ЯВЛЯЮЩЕЕСЯ  МАШИН- %
%  НЫМ  ПРЕДСТАВЛЕНИЕМ  ЧИСЛА  СЛЕДУЮЩЕГО  ПОСЛЕ  X  В   %
%  НАПРАВЛЕНИИ  Y.                                       %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

FL __nexttowardf(FL x, LD y)
{
    _type_float_bits X, C;
    _type_long_double_bits Y;
    int hx;

    if (x != x || y != y)
        return x + y;       /* X или Y = NaN */
    if (x == y)
        return y;
    if (x == 0) {           /* return minsubnormal */
        Y.value = y;
        X.int0 = ((Y.twolong.short0 & 0x8000) << 16) | 1;
        return X.value;
    }
    X.value = x;
    if ((x > y && X.int0 >= 0) || (!(x > y) && X.int0 < 0))
        X.int0 -= 1;
    else
        X.int0 += 1;
    hx = X.int0 & 0x7f800000;
    if (hx == 0x7f800000) {
        C.int0 = 0x7f000000;    /* установка overflow */
        __set_errno (ERANGE);
        C.value *= C.value;
    }
    else if (hx < 0x00800000) {
        C.int0 = 0x800000;      /* установка underflow */
        __set_errno (ERANGE);
        C.value *= C.value;
    }
    return X.value;
}

weak_alias (__nexttowardf, nexttowardf)

