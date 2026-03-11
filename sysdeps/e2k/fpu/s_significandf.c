/* Copyright (c) 2016 AO MCST. All rights reserved.
 * Distributed under the terms of MIT License.
 */

#include <math.h>
#include <math_private.h>

#include "f2c.h"

float __significandf(float x)
{
  _type_float_bits X;
  int k;

  X.value = x;
  k = X.int0 & 0x7fffffff;

  if (__glibc_unlikely (k >= 0x7f800000))
    return x * FL_MIN_UN;                                /* NaN or Inf */
  if (__glibc_unlikely (k < 0x00800000))           /* 0 or subnormal x */
    {
      if (x == 0)
        return x;                                  /* +-0 */
      X.value *= DVAIN23;
    }
  X.int0 &= ~0x7f800000;               /* обнуляем порядок */
  X.int0 |= 0x3f800000;               /* вставляем порядок */
  return X.value;
}
weak_alias (__significandf, significandf)
