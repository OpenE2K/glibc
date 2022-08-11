/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/* s_logbf.c -- float version of s_logb.c.
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

float
__logbf (float x)
{
  __extension__ union { float __f; int __i; } __xx = { __f: x };
  int __ix = __xx.__i & 0x7fffffff;

  if (__ix == 0)
    return -1.0f / ffabs (x); /* -inf */
  if (__glibc_unlikely (__ix < 0x00800000))
    {
      /* POSIX specifies that denormal number is treated as
         though it were normalized.  */
       return (float) (9 - __builtin_clz (__ix) - 127);
    }
  if (__glibc_unlikely (__ix >= 0x7f800000))
    return x * x; /* +inf or nan */
  return (float) ((__ix >> 23) - 127);
}
weak_alias (__logbf, logbf)
