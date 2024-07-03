/* Copyright (c) 2016 AO MCST. All rights reserved.
 * Distributed under the terms of MIT License.
 */

#include <math.h>
#include <math_private.h>

#include "f2c.h"

float __frexpf(float x, int *eptr)
{
  _type_float_bits X;
  int ix;

  X.value = x;
  ix = X.int0 & 0x7fffffff;
  if (__glibc_unlikely (ix >= 0x7f800000 || ix == 0))
    {
      *eptr = 0;
      return x;                                           /* 0,inf,nan */
    }
  if (__glibc_unlikely (ix < 0x00800000))                 /* subnormal */
    {
      X.value *= DVAIN23;
      ix = X.int0 & 0x7fffffff;
      *eptr = (ix >> 23) - 149;                          /* extract exponent */
      X.int0 &= ~0x7f800000; /* обнуляем порядок */
      X.int0 |= 0x3f000000; /* вставляем 0-й порядок */
      return X.value;
    }
  *eptr = (ix >> 23) - 126;
  X.int0 &= ~0x7f800000; /* обнуляем порядок */
  X.int0 |= 0x3f000000; /* вставляем 0-й порядок */
  return X.value;
}
weak_alias (__frexpf, frexpf)
