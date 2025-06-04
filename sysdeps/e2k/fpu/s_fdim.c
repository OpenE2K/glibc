/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/* Return positive difference between arguments. */

#include <errno.h>
#include "f2c.h"

double
__fdim (double x, double y)
{
  _type_double_bits X, Y;
  LL ix, iy;

  X.value = x;
  ix = X.llong & 0x7fffffffffffffffLL;
  Y.value = y;
  iy = Y.llong & 0x7fffffffffffffffLL;

  if (ix > 0x7ff0000000000000LL || iy > 0x7ff0000000000000LL) /* x or y is NaN */
      /* Raise invalid flag for signaling but not quiet NaN.  */
      return x - y;

  if (x <= y)
    return 0.0;

  X.value = x - y;
  if ((X.llong & 0x7fffffffffffffffLL) == 0x7ff0000000000000LL &&
    ix != 0x7ff0000000000000LL && iy != 0x7ff0000000000000LL)
    __set_errno (ERANGE);

  return X.value;
}

weak_alias (__fdim, fdim)
#ifdef NO_LONG_DOUBLE
strong_alias (__fdim, __fdiml)
weak_alias (__fdim, fdiml)
#endif
