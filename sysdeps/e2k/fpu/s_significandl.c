/* Copyright (c) 2016 AO MCST. All rights reserved.
 * Distributed under the terms of MIT License.
 */

/*
 * significandl(x) computes just
 * 	scalbl(x, (long double) -ilogbl(x)),
 * for exercising the fraction-part(F) IEEE 754-1985 test vector.
 */

#include <math.h>
#include <math_private.h>

#include "f2c.h"

long double __significandl(long double x)
{
  _type_long_double_bits X;
  LL k;

  X.value = x;
  k = X.twolong.short0 & 0x7fff;

  if (__glibc_unlikely (k >= 0x7fff))
    return x * LD_MIN_UN;                                /* NaN or Inf */
  if (__glibc_unlikely (k == 0))                   /* 0 or subnormal x */
    {
      if (x == 0)
        return x;                                  /* +-0 */
      X.value *= DVAIN63;
    }
  X.twolong.short0 &= ~0x7fff;               /* обнуляем порядок */
  X.twolong.short0 |= 0x3fff;               /* вставляем порядок */
  return X.value;
}
weak_alias (__significandl, significandl)
