/* Copyright (c) 2016 AO MCST. All rights reserved.
 * Distributed under the terms of MIT License.
 */

#include <math.h>
#include <math_private.h>

#include "f2c.h"

double
__ieee754_scalb (double x, double fn)
{
  _type_double_bits X, E, tmp;
  LL k, exp;

  X.value = x;
  E.value = fn;
  k = X.llong & 0x7fffffffffffffffLL;
  exp = E.llong & 0x7fffffffffffffffLL;

  if (__glibc_unlikely (k > 0x7ff0000000000000LL))
    return x * fn;
  if (__glibc_unlikely (exp >= 0x7ff0000000000000LL)) {
      if (exp > 0x7ff0000000000000LL || fn > 0)
        return x * fn;
      if (k == 0)
        return x;
      return x / -fn;
  }
#if __iset__ <= 2
  if (__glibc_unlikely (exp < 0x4330000000000000LL && ((DB) (LL) fn) != fn))
#else /*  __iset__ <= 2 */
_Pragma ("asm_inline")
  __asm ("fdtoifd 0x4,%1,%0" : "=r" (tmp.value): "r" (fn));
  if (__glibc_unlikely (tmp.value != fn))
#endif /*  __iset__ <= 2 */
    return 0 / 0.0; /* nan with invalid exception */

  /* далее почти scalbn() */
# if __iset__ <= 3

  k >>= 52;                                        /* extract exponent */
  if (__glibc_unlikely (k == 0)) {                 /* 0 or subnormal x */
      if (x == 0)
        return x;                                  /* +-0 */
      X.value *= DVAIN52;
      k = ((X.llong >> 52) & 0x7ff) - 52;
  }
  if (__glibc_unlikely (k == 0x7ff))
    return x + x;                                       /* NaN or Inf */
  X.llong &= ~0x7ff0000000000000LL;               /* обнуляем порядок */

  k += (LL) fn;
  if (__glibc_unlikely (fn <= -5000 || k <= -54)) {
      tmp.value = DB_MIN;
      X.llong |= tmp.llong;
      return DB_MIN * X.value;           /*underflow*/
  }
  if (__glibc_unlikely (fn >= 5000 || k > 0x7fe)) {
      tmp.value = DB_MAX;
      X.llong |= tmp.llong;
      return DB_MAX * X.value;           /* overflow */
  }
  if (__glibc_likely (k > 0)) {                 /* normal result */
      X.llong |= (k << 52);
      return X.value;
  }
  k += 54;                                      /* subnormal result */
  X.llong |= (k << 52);
  return X.value * K2IN54;

# else /* __iset__ <= 3 */

  if (fn >= 5000) k = 5000;
  else if (fn <= -5000) k = -5000;
  else k = (int) fn;
#pragma asm_inline
  __asm ("fscaled %0,%1,%0" : "+r" (x) : "r" (k));
  return x;

# endif /* __iset__ <= 3 */
}
strong_alias (__ieee754_scalb, __scalb_finite)
