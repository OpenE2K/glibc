/* Copyright (c) 2018 ZAO "MCST". All rights reserved. */

#include <math.h>
#include <math_private.h>

#include "f2c.h"
#include <e2kintrin.h>

LD __scalblnl (LD x, long int n)
{
#ifndef __ptr32__
  if (n > 50000) n = 50000;
  else if (n < -50000) n = -50000;
#endif /* __ptr32__ */

# if __iset__ <= 3

  _type_long_double_bits X, tmp;
  LL k;

  X.value = x;
  k = X.twolong.short0 & 0x7fff;                   /* extract exponent */
  if (__glibc_unlikely (k == 0))                   /* 0 or subnormal x */
    {
      if (x == 0)
        return x;                                  /* +-0 */
      X.value *= DVAIN63;
      k = (X.twolong.short0 & 0x7fff) - 63;
    }
  if (__glibc_unlikely (k == 0x7fff))
    return x + x;                                       /* NaN or Inf */
  X.twolong.short0 &= 0x8000;                     /* обнуляем порядок */
  k = k + n;
  if (__glibc_unlikely (k <= -65))
    {
      tmp.value = LD_MIN;
      tmp.twolong.short0 |= X.twolong.short0;
      return LD_MIN * tmp.value;           /*underflow*/
    }
  if (__glibc_unlikely (k > 0x7ffe))
    {
      tmp.value = LD_MAX;
      tmp.twolong.short0 |= X.twolong.short0;
      return LD_MAX * tmp.value;           /* overflow */
    }
  if (__glibc_likely (k > 0))                    /* normal result */
    {
      X.twolong.short0 |= k;
      return X.value;
    }
  k += 110;                                      /* subnormal result */
  X.twolong.short0 |= k;
  return X.value * K2IN110;

# else /* __iset__ <= 3 */

#pragma asm_inline
  __asm ("fxscalesx %0,%1,%0" : "+r" (x) : "r" (n));
  return x;

# endif /* __iset__ <= 3 */
}
