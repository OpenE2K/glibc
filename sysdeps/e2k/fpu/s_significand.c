/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/* @(#)s_signif.c 5.1 93/09/24 */
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
static char rcsid[] = "$NetBSD: s_significand.c,v 1.6 1995/05/10 20:48:11 jtc Exp $";
#endif

/*
 * significand(x) computes just
 * 	scalb(x, (double) -ilogb(x)),
 * for exercising the fraction-part(F) IEEE 754-1985 test vector.
 */

#include <math.h>
#include <math_private.h>

#include "f2c.h"

double __significand(double x)
{
  _type_double_bits X;
  LL k;

  X.value = x;
  k = X.llong & 0x7fffffffffffffffLL;

  if (__glibc_unlikely (k >= 0x7ff0000000000000LL))
    return x * DB_MIN_UN;                                /* NaN or Inf */
  if (__glibc_unlikely (k < 0x10000000000000LL))   /* 0 or subnormal x */
    {
      if (x == 0)
        return x;                                  /* +-0 */
      X.value *= DVAIN52;
    }
  X.llong &= ~0x7ff0000000000000LL;               /* обнуляем порядок */
  X.llong |= 0x3ff0000000000000LL;               /* вставляем порядок */
  return X.value;
}
weak_alias (__significand, significand)
#ifdef NO_LONG_DOUBLE
strong_alias (__significand, __significandl)
weak_alias (__significand, significandl)
#endif
