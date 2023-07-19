/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/* s_ldexpf.c -- float version of s_ldexp.c.
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

#if defined(LIBM_SCCS) && !defined(lint)
static char rcsid[] = "$NetBSD: s_ldexpf.c,v 1.3 1995/05/10 20:47:42 jtc Exp $";
#endif

#include <math.h>
#include <math_private.h>
#include <errno.h>

#include "f2c.h"
#include <e2kintrin.h>

#define twom25 2.9802322388e-08F        /* 0x33000000 */

float __ldexpf(float value, int exp)
{
  _type_float_bits X;
  int k;

  if (value == 0)
    return value;                                  /* +-0 */
  X.value = value;
  k = (X.int0 >> 23) & 0xff;                     /* extract exponent */
  if (__glibc_unlikely (k == 0xff))
    return value + value;                          /* NaN or Inf */

# if __iset__ <= 3

  _type_float_bits tmp;

  if (__glibc_unlikely (k == 0))                   /* 0 or subnormal x */
    {
      X.value *= DVAIN23;
      k = ((X.int0 >> 23) & 0xff) - 23;
    }
  X.int0 &= ~0x7f800000;                          /* обнуляем порядок */
  if (__glibc_unlikely ((k + (LL) exp) <= -25))
    {
      tmp.value = FL_MIN;
      X.int0 |= tmp.int0;
      __set_errno (ERANGE);
      return FL_MIN * X.value;           /*underflow*/
    }
  if (__glibc_unlikely ((k + (LL) exp) > 0xfe))
    {
      tmp.value = FL_MAX;
      X.int0 |= tmp.int0;
      __set_errno (ERANGE);
      return FL_MAX * X.value;           /* overflow */
    }
  k = k + exp;
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
  __asm ("fscales %0,%1,%0" : "+r" (value) : "r" (exp));
  X.value = value;
  k = X.int0 & 0x7f800000;                       /* extract exponent */
  if (__glibc_unlikely (k == 0 || k >= 0x7f800000))
    {
      __set_errno (ERANGE);                      /*underflow or overflow*/
    }
  return value;

# endif /* __iset__ <= 3 */
}
weak_alias (__ldexpf, ldexpf)
weak_alias (__ldexpf, scalbnf)
