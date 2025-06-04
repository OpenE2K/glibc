/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/* Return positive difference between arguments. */

#include <errno.h>
#include "f2c.h"

float
__fdimf (float x, float y)
{
  _type_float_bits X, Y;
  int ix, iy;

  X.value = x;
  ix = X.int0 & 0x7fffffff;
  Y.value = y;
  iy = Y.int0 & 0x7fffffff;

  if (ix > 0x7f800000 || iy > 0x7f800000) /* x or y is NaN */
      /* Raise invalid flag for signaling but not quiet NaN.  */
      return x - y;

  if (x <= y)
    return 0;

  X.value = x - y;
  if ((X.int0 & 0x7fffffff) == 0x7f800000 &&
    ix != 0x7f800000 && iy != 0x7f800000)
    __set_errno (ERANGE);

  return X.value;
}

weak_alias (__fdimf, fdimf)
