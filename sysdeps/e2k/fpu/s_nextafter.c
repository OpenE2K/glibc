/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/* Ugly hack so that the aliasing works.  */
#define __nexttoward __internal___nexttoward
#define nexttoward __internal_nexttoward

#include <errno.h>
#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%    ðòïçòáííá  ÷ùþéóìñåô  úîáþåîéå,  ñ÷ìñàýååóñ  íáûéî- %
%  îùí  ðòåäóôá÷ìåîéåí  þéóìá  óìåäõàýåçï  ðïóìå  X  ÷   %
%  îáðòá÷ìåîéé  Y.                                       %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

DB __nextafter(DB x, DB y)
{
    _type_double_bits X, Y;
    LL hx;

    if (x != x || y != y)
        return x + y;       /* X ÉÌÉ Y = NaN */
    if (x == y)
        return y;
    if (x == 0) {           /* return minsubnormal */
        Y.value = y;
        X.llong = (Y.llong & 0x8000000000000000LL) | 1;
        return X.value;
    }
    X.value = x;
    if ((x > y && X.llong >= 0) || (!(x > y) && X.llong < 0))
        X.llong -= 1;
    else
        X.llong += 1;
    hx = X.llong & 0x7ff0000000000000LL;
    if (hx == 0x7ff0000000000000LL) {
        Y.llong = 0x7fe0000000000000LL;    /* ÕÓÔÁÎÏ×ËÁ overflow */
        __set_errno (ERANGE);
        Y.value *= Y.value;
    }
    else if (hx < 0x0010000000000000LL) {
        Y.llong = 0x0010000000000000LL;      /* ÕÓÔÁÎÏ×ËÁ underflow */
        __set_errno (ERANGE);
        Y.value *= Y.value;
    }
    return X.value;
}

weak_alias (__nextafter, nextafter)
#ifdef NO_LONG_DOUBLE
strong_alias (__nextafter, __nextafterl)
weak_alias (__nextafter, nextafterl)
strong_alias (__nextafter, __nexttowardl)
weak_alias (__nexttowardl, nexttowardl)
#undef __nexttoward
strong_alias (__nextafter, __nexttoward)
#undef nexttoward
weak_alias (__nextafter, nexttoward)
#endif
