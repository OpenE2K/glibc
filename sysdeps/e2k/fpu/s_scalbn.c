/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

/*
 * scalbn (double x, int n)
 * scalbn(x,n) returns x* 2**n  computed by  exponent
 * manipulation rather than by actually performing an
 * exponentiation or a multiplication.
 */

#include "f2c.h"
#include <e2kintrin.h>

double
__scalbn (double x, int n)
{
# if __iset__ <= 3

  _type_double_bits X, tmp;
  LL k;

  X.value = x;
  k = (X.llong >> 52) & 0x7ff;                     /* extract exponent */
  if (__glibc_unlikely (k == 0))                   /* 0 or subnormal x */
    {
      if (x == 0)
        return x;                                  /* +-0 */
      X.value *= DVAIN52;
      k = ((X.llong >> 52) & 0x7ff) - 52;
    }
  if (__glibc_unlikely (k == 0x7ff))
    return x + x;                                       /* NaN or Inf */
  X.llong &= ~0x7ff0000000000000LL; /* обнуляем порядок */
  k = k + n;
  if (__glibc_unlikely (k <= -54))
    {
      tmp.value = DB_MIN;
      X.llong |= tmp.llong;
      return DB_MIN * X.value;           /*underflow*/
    }
  if (__glibc_unlikely (k > 0x7fe))
    {
      tmp.value = DB_MAX;
      X.llong |= tmp.llong;
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
  __asm ("fscaled %0,%1,%0" : "+r" (x) : "r" (n));
  return x;

# endif /* __iset__ <= 3 */
}

#ifdef NO_LONG_DOUBLE
strong_alias (__scalbn, __scalbnl)
#endif
