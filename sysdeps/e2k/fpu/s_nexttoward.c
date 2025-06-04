/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

#include <errno.h>
#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%    ПРОГРАММА  ВЫЧИСЛЯЕТ  ЗНАЧЕНИЕ,  ЯВЛЯЮЩЕЕСЯ  МАШИН- %
%  НЫМ  ПРЕДСТАВЛЕНИЕМ  ЧИСЛА  СЛЕДУЮЩЕГО  ПОСЛЕ  X  В   %
%  НАПРАВЛЕНИИ  Y.                                       %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

DB __nexttoward(DB x, LD y)
{
    _type_double_bits X, C;
    _type_long_double_bits Y;
    LL hx;

    if (x != x || y != y)
        return x + y;       /* X или Y = NaN */
    if (x == y)
        return y;
    if (x == 0) {           /* return minsubnormal */
        Y.value = y;
        X.llong = ((Y.twolong.short0 & 0x8000LL) << 48) | 1;
        return X.value;
    }
    X.value = x;
    if ((x > y && X.llong >= 0) || (!(x > y) && X.llong < 0))
        X.llong -= 1;
    else
        X.llong += 1;
    hx = X.llong & 0x7ff0000000000000LL;
    if (hx == 0x7ff0000000000000LL) {
        C.llong = 0x7fe0000000000000LL;    /* установка overflow */
        __set_errno (ERANGE);
        C.value *= C.value;
    }
    else if (hx < 0x0010000000000000LL) {
        C.llong = 0x0010000000000000LL;      /* установка underflow */
        __set_errno (ERANGE);
        C.value *= C.value;
    }
    return X.value;
}

weak_alias (__nexttoward, nexttoward)
#ifdef NO_LONG_DOUBLE
strong_alias (__nexttoward, __nexttowardl)
weak_alias (__nexttoward, nexttowardl)
#endif
