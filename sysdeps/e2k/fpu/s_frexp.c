/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/* @(#)s_frexp.c 5.1 93/09/24 */
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
static char rcsid[] = "$NetBSD: s_frexp.c,v 1.9 1995/05/10 20:47:24 jtc Exp $";
#endif

/*
 * for non-zero x
 *	x = frexp(arg,&exp);
 * return a double fp quantity x such that 0.5 <= |x| <1.0
 * and the corresponding binary exponent "exp". That is
 *	arg = x*2^exp.
 * If arg is inf, 0.0, or NaN, then frexp(arg,&exp) returns arg
 * with *exp=0.
 */

#include <math.h>
#include <math_private.h>

#include "f2c.h"

double
__frexp (double x, int *eptr)
{
  _type_double_bits X;
  LL ix;

  X.value = x;
  ix = X.llong & 0x7fffffffffffffffLL;
  if (__glibc_unlikely (ix >= 0x7ff0000000000000LL || ix == 0))
    {
      *eptr = 0;
      return x;                                           /* 0,inf,nan */
    }
  if (__glibc_unlikely (ix < 0x0010000000000000LL))              /* subnormal */
    {
      X.value *= DVAIN52;
      ix = X.llong & 0x7fffffffffffffffLL;
      *eptr = (ix >> 52) - 1074;                          /* extract exponent */
      X.llong &= ~0x7ff0000000000000LL; /* �������� ������� */
      X.llong |= 0x3fe0000000000000LL; /* ��������� 0-� ������� */
      return X.value;
    }
  *eptr = (ix >> 52) - 1022;
  X.llong &= ~0x7ff0000000000000LL; /* �������� ������� */
  X.llong |= 0x3fe0000000000000LL; /* ��������� 0-� ������� */
  return X.value;
}
weak_alias (__frexp, frexp)
#ifdef NO_LONG_DOUBLE
strong_alias (__frexp, __frexpl)
weak_alias (__frexp, frexpl)
#endif
