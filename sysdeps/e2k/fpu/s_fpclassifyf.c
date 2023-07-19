/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/*
 * fpclassifyf(x) returns classification value corresponding to argument;
 */

/* To avoid inline definitions.  */
#undef __LIBC_INTERNAL_MATH_INLINES

#include <math.h>
#include <math_private.h>

int
__fpclassifyf (float x)
{
  __extension__ union { float __f; int __i; } __xx = { __f: x };
  int __ex, __m, __retval = FP_NORMAL;

  __xx.__i &= ~0x80000000;
  __ex = __xx.__i >> 23;
  __m = __xx.__i & 0x7fffffLL;
  if (__ex == 0)
    __retval = __m == 0 ? FP_ZERO : FP_SUBNORMAL;
  else if (__ex == 0xff)
    __retval = __m == 0 ? FP_INFINITE : FP_NAN;
  return __retval;
}

hidden_def (__fpclassifyf)
