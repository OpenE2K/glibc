/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/* Copyright (C) 2011-2015 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Ulrich Drepper <drepper@gmail.com>, 2011.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#include <math.h>
#include <math_private.h>

#include "f2c.h"

#define twom25 2.9802322388e-08F        /* 0x33000000 */

float
__ieee754_scalbf (float x, float fn)
{
  _type_float_bits X, E, tmp;
  int k, exp;

  X.value = x;
  E.value = fn;
  k = X.int0 & 0x7fffffff;
  exp = E.int0 & 0x7fffffff;

  if (__glibc_unlikely (k > 0x7f800000))
    return x * fn;
  if (__glibc_unlikely (exp >= 0x7f800000))
    {
      if (exp > 0x7f800000 || fn > 0)
        return x * fn;
      if (k == 0)
        return x;
      return x / -fn;
    }
#if __iset__ <= 2
  if (__glibc_unlikely (exp < 0x4b000000 && ((FL) (int) fn) != fn))
#else /*  __iset__ <= 2 */
_Pragma ("asm_inline")
  __asm ("fstoifs 0x4,%1,%0" : "=r" (tmp.value): "r" (fn));
  if (__glibc_unlikely (tmp.value != fn))
#endif /*  __iset__ <= 2 */
    return 0 / 0.0F; /* nan with invalid exception */

  /* далее почти scalbn() */
# if __iset__ <= 3

  k >>= 23;                                        /* extract exponent */
  if (__glibc_unlikely (k == 0))                   /* 0 or subnormal x */
    {
      if (x == 0)
        return x;                                  /* +-0 */
      X.value *= DVAIN23;
      k = ((X.int0 >> 23) & 0xff) - 23;
    }
  if (__glibc_unlikely (k == 0xff))
    return x + x;                                       /* NaN or Inf */
  X.int0 &= ~0x7f800000;                          /* обнуляем порядок */

  if (__glibc_unlikely (fn <= -500 || (k + (LL) fn) <= -25))
    {
      tmp.value = FL_MIN;
      X.int0 |= tmp.int0;
      return FL_MIN * X.value;           /*underflow*/
    }
  if (__glibc_unlikely (fn >= 500 || (k + (LL) fn) > 0xfe))
    {
      tmp.value = FL_MAX;
      X.int0 |= tmp.int0;
      return FL_MAX * X.value;           /* overflow */
    }
  k += (LL) fn;
  if (__glibc_likely (k > 0))                    /* normal result */
    {
      X.int0 |= (k << 23);
      return X.value;
    }
  k += 25;                                      /* subnormal result */
  X.int0 |= (k << 23);
  return X.value * twom25;

# else /* __iset__ <= 3 */

  if (fn >= 500) k = 500;
  else if (fn <= -500) k = -500;
  else k = (int) fn;
#pragma asm_inline
  __asm ("fscales %0,%1,%0" : "+r" (x) : "r" (k));
  return x;

# endif /* __iset__ <= 3 */
}
strong_alias (__ieee754_scalbf, __scalbf_finite)
