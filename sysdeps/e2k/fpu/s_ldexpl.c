/* Copyright (c) 2018 ZAO "MCST". All rights reserved. */

#include <math.h>
#include <math_private.h>
#include <errno.h>

#include "f2c.h"

LD __ldexpl(LD value, int exp)
{
  _type_long_double_bits X;
  LL k;

  if (value == 0)
    return value;                                  /* +-0 */
  X.value = value;
  k = X.twolong.short0 & 0x7fff;                     /* extract exponent */
  if (__glibc_unlikely (k == 0x7fff))
    return value + value;                          /* NaN or Inf */

# if __iset__ <= 3

  _type_long_double_bits tmp;

  if (__glibc_unlikely (k == 0))                   /* subnormal x */
    {
      X.value *= DVAIN63;
      k = (X.twolong.short0 & 0x7fff) - 63;
    }
  X.twolong.short0 &= 0x8000; /* обнуляем порядок */
  k = k + exp;
  if (__glibc_unlikely (k <= -65))
    {
      tmp.value = LD_MIN;
      tmp.twolong.short0 |= X.twolong.short0;
      __set_errno (ERANGE);
      return LD_MIN * tmp.value;           /*underflow*/
    }
  if (__glibc_unlikely (k > 0x7ffe))
    {
      tmp.value = LD_MAX;
      tmp.twolong.short0 |= X.twolong.short0;
      __set_errno (ERANGE);
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
  __asm ("fxscalesx %0,%1,%0" : "+r" (value) : "r" (exp));
  X.value = value;
  k = X.twolong.short0 & 0x7fff;                     /* extract exponent */
  if (__glibc_unlikely (k == 0 || k == 0x7fff))
    {
      __set_errno (ERANGE);                      /*underflow or overflow*/
    }
  return value;

# endif /* __iset__ <= 3 */
}
weak_alias (__ldexpl, ldexpl)
weak_alias (__ldexpl, scalbnl)
