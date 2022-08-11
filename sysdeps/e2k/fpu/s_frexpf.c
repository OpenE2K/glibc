/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/* s_frexpf.c -- float version of s_frexp.c.
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
static char rcsid[] = "$NetBSD: s_frexpf.c,v 1.5 1995/05/10 20:47:26 jtc Exp $";
#endif

#include <math.h>
#include <math_private.h>

#include "f2c.h"

float __frexpf(float x, int *eptr)
{
  _type_float_bits X;
  int ix;

  X.value = x;
  ix = X.int0 & 0x7fffffff;
  if (__glibc_unlikely (ix >= 0x7f800000 || ix == 0))
    {
      *eptr = 0;
      return x;                                           /* 0,inf,nan */
    }
  if (__glibc_unlikely (ix < 0x00800000))                 /* subnormal */
    {
      X.value *= DVAIN23;
      ix = X.int0 & 0x7fffffff;
      *eptr = (ix >> 23) - 149;                          /* extract exponent */
      X.int0 &= ~0x7f800000; /* обнуляем порядок */
      X.int0 |= 0x3f000000; /* вставляем 0-й порядок */
      return X.value;
    }
  *eptr = (ix >> 23) - 126;
  X.int0 &= ~0x7f800000; /* обнуляем порядок */
  X.int0 |= 0x3f000000; /* вставляем 0-й порядок */
  return X.value;
}
weak_alias (__frexpf, frexpf)
