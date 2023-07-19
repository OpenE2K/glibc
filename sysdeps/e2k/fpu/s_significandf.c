/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/* s_significandf.c -- float version of s_significand.c.
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
static char rcsid[] = "$NetBSD: s_significandf.c,v 1.3 1995/05/10 20:48:13 jtc Exp $";
#endif

#include <math.h>
#include <math_private.h>

#include "f2c.h"

float __significandf(float x)
{
  _type_float_bits X;
  int k;

  X.value = x;
  k = X.int0 & 0x7fffffff;

  if (__glibc_unlikely (k >= 0x7f800000))
    return x * FL_MIN_UN;                                /* NaN or Inf */
  if (__glibc_unlikely (k < 0x00800000))           /* 0 or subnormal x */
    {
      if (x == 0)
        return x;                                  /* +-0 */
      X.value *= DVAIN23;
    }
  X.int0 &= ~0x7f800000;               /* обнуляем порядок */
  X.int0 |= 0x3f800000;               /* вставляем порядок */
  return X.value;
}
weak_alias (__significandf, significandf)
