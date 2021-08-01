/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/* s_scalbnf.c -- float version of s_scalbn.c.
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
 */

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

#include <math.h>
#include <math_private.h>

#include "f2c.h"
#include <e2kintrin.h>

#define twom25 2.9802322388e-08F        /* 0x33000000 */

float
__scalblnf (float x, long int n)
{
#ifndef __ptr32__
  if (n > 500) n = 500;
  if (n < -500) n = -500;
#endif /* __ptr32__ */

# if __iset__ <= 3

  _type_float_bits X, tmp;
  int k;

  X.value = x;
  k = (X.int0 >> 23) & 0xff;                     /* extract exponent */
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
  if (__glibc_unlikely ((k + (LL) n) <= -25))
    {
      tmp.value = FL_MIN;
      X.int0 |= tmp.int0;
      return FL_MIN * X.value;           /*underflow*/
    }
  if (__glibc_unlikely ((k + (LL) n) > 0xfe))
    {
      tmp.value = FL_MAX;
      X.int0 |= tmp.int0;
      return FL_MAX * X.value;           /* overflow */
    }
  k = k + n;
  if (__glibc_likely (k > 0))                    /* normal result */
    {
      X.int0 |= (k << 23);
      return X.value;
    }
  k += 25;                                      /* subnormal result */
  X.int0 |= (k << 23);
  return X.value * twom25;

# else /* __iset__ <= 3 */

#pragma asm_inline
  __asm ("fscales %0,%1,%0" : "+r" (x) : "r" (n));
  return x;

# endif /* __iset__ <= 3 */
}
