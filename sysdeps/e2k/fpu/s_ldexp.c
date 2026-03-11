/* Copyright (c) 2016 AO MCST. All rights reserved.
 * Distributed under the terms of MIT License.
 */

#include <math.h>
#include <math_private.h>
#include <errno.h>

#include "f2c.h"
#include <e2kintrin.h>

double __ldexp(double value, int exp)
{
  _type_double_bits X;
  LL k;

  if (value == 0)
    return value;                                  /* +-0 */
  X.value = value;
  k = (X.llong >> 52) & 0x7ff;                     /* extract exponent */
  if (__glibc_unlikely (k == 0x7ff))
    return value + value;                          /* NaN or Inf */

# if __iset__ <= 3

  _type_double_bits tmp;

  if (__glibc_unlikely (k == 0))                   /* subnormal x */
    {
      X.value *= DVAIN52;
      k = ((X.llong >> 52) & 0x7ff) - 52;
    }
  X.llong &= ~0x7ff0000000000000LL; /* обнуляем порядок */
  k = k + exp;
  if (__glibc_unlikely (k <= -54))
    {
      tmp.value = DB_MIN;
      X.llong |= tmp.llong;
      __set_errno (ERANGE);
      return DB_MIN * X.value;           /* underflow */
    }
  if (__glibc_unlikely (k > 0x7fe))
    {
      tmp.value = DB_MAX;
      X.llong |= tmp.llong;
      __set_errno (ERANGE);
      return DB_MAX * X.value;           /* overflow */
    }
  if (__glibc_likely (k > 0))                    /* normal result */
    {
      X.llong |= (k << 52);
      return X.value;
    }
  k += 54;                                      /* subnormal result */
  X.llong |= (k << 52);
  return X.value * K2IN54;

# else /* __iset__ <= 3 */

#pragma asm_inline
  __asm ("fscaled %0,%1,%0" : "+r" (value) : "r" (exp));
  X.value = value;
  k = X.llong & 0x7ff0000000000000LL;            /* extract exponent */
  if (__glibc_unlikely (k == 0 || k >= 0x7ff0000000000000LL))
    {
      __set_errno (ERANGE);                      /* underflow or overflow */
    }
  return value;

# endif /* __iset__ <= 3 */
}
weak_alias (__ldexp, ldexp)
weak_alias (__ldexp, scalbn)
#ifdef NO_LONG_DOUBLE
strong_alias (__ldexp, __ldexpl)
weak_alias (__ldexp, ldexpl)
weak_alias (__ldexp, scalbnl)
#endif
