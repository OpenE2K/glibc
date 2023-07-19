/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

#include <errno.h>
#include "f2c.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                        %
%    ðòïçòáííá  ÷ùþéóìñåô  úîáþåîéå,  ñ÷ìñàýååóñ  íáûéî- %
%  îùí  ðòåäóôá÷ìåîéåí  þéóìá  óìåäõàýåçï  ðïóìå  X  ÷   %
%  îáðòá÷ìåîéé  Y.                                       %
%                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

FL __nextafterf(FL x, FL y)
{
    _type_float_bits X, Y;
    int hx;

    if (x != x || y != y)
        return x + y;       /* X ÉÌÉ Y = NaN */
    if (x == y)
        return y;
    if (x == 0) {           /* return minsubnormal */
        Y.value = y;
        X.int0 = (Y.int0 & 0x80000000) | 1;
        return X.value;
    }
    X.value = x;
    if ((x > y && X.int0 >= 0) || (!(x > y) && X.int0 < 0))
        X.int0 -= 1;
    else
        X.int0 += 1;
    hx = X.int0 & 0x7f800000;
    if (hx == 0x7f800000) {
        Y.int0 = 0x7f000000;    /* ÕÓÔÁÎÏ×ËÁ overflow */
        __set_errno (ERANGE);
        Y.value *= Y.value;
    }
    else if (hx < 0x00800000) {
        Y.int0 = 0x800000;      /* ÕÓÔÁÎÏ×ËÁ underflow */
        __set_errno (ERANGE);
        Y.value *= Y.value;
    }
    return X.value;
}

weak_alias (__nextafterf, nextafterf)
