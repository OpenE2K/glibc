/* Copyright (c) 2018 ZAO "MCST". All rights reserved. */

#include <math.h>
#include <math_private.h>

#include "f2c.h"

LD
__ieee754_scalbl (LD x, LD fn)
{
  _type_long_double_bits X, E;
#if __iset__ <= 3
  _type_long_double_bits tmp;
#endif
  LL k, exp;

  X.value = x;
  E.value = fn;
  k = X.twolong.short0 & 0x7fff;
  exp = E.twolong.short0 & 0x7fff;

  if (__glibc_unlikely (x != x))
    return x * fn;
  if (__glibc_unlikely (exp == 0x7fff))
    {
      if (fn != fn || fn > 0)
        return x * fn;
      if (x == 0)
        return x;
      return x / -fn;
    }

  if (__glibc_unlikely (exp < 0x403e && ((LD) (LL) fn) != fn))
    return 0 / 0.0; /* nan with invalid exception */

  /* далее почти scalbnl() */
# if __iset__ <= 3

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

  k += (LL) fn;
  if (__glibc_unlikely (fn <= -50000 || k <= -65))
    {
      tmp.value = LD_MIN;
      tmp.twolong.short0 |= X.twolong.short0;
      return LD_MIN * tmp.value;           /*underflow*/
    }
  if (__glibc_unlikely (fn >= 50000 || k > 0x7ffe))
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

  if (fn >= 50000) k = 50000;
  else if (fn <= -50000) k = -50000;
  else k = (int) fn;
#pragma asm_inline
  __asm ("fxscalesx %0,%1,%0" : "+r" (x) : "r" (k));
  return x;

# endif /* __iset__ <= 3 */
}
strong_alias (__ieee754_scalbl, __scalbl_finite)
